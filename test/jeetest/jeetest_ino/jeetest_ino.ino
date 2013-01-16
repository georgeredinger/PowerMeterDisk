/**
 * Jeenode remote Temperature sensor for Temperature Data Logger.
 * Remote sensors use dallas DS18B20 or DS18S20.
 *
 * Each Jeenode sends data over RF12 to central datalogger module.
 * See code & comment of Temperature Data Logger project.
 *
 * Gérard Chevalier, Jan 2011
 */

// RF12 frame format: Byte 0 = channel, Byte 1 = temp (bit 7 == sign), Byte 2 = decitemp

// Channel number. In a next release, should be read from config jumpers.
#define THIS_NODE_CHANNEL	1

#include <OneWire.h>
#include <RF12.h>
#include <Ports.h>

//#define DS1820_DEBUG_LOW
#define DS1820_DEBUG
#define ONE_MN_INTERVAL 60000L

// if B_MODEL defined, temp reading will use 18B20 model scheme, otherwise 18S20 is assumed
#define B_MODEL

OneWire  ds(4);

byte HighByte, LowByte, TReading, SignBit, Whole, Fract;
byte TempRead, CountRemain;
word Tc_100;
char StrBuff[50];
char TempAsString[10];
byte TempAsStringLen;
byte TempBinFormat[3];

boolean ANewMinute = false;
unsigned long previousMillis;
// all time calculation done in mn with 8 bits, having so a roll-over of about 4 h
byte Minutes = 0, LastSend = 0;
byte SendPeriod;

boolean SendTemp;

void setup() {
  Serial.begin(115200);
  
  SendPeriod = 2;
  SendTemp = false;
  previousMillis = ONE_MN_INTERVAL;  // Will fire a measure immediatly
  rf12_initialize(1, RF12_868MHZ, 33);
  Serial.println("Temp transmiter startup");
}

void Get1820Tmp() {
  byte i;
  byte present = 0;
  byte OneWData[12];
  
  ds.reset();
  ds.skip();
  ds.write(0x44,1);         // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.skip();
  ds.write(0xBE);         // Read Scratchpad

  for ( i = 0; i < 9; i++) {           // we need 9 bytes
#ifdef DS1820_DEBUG_LOW
    Serial.print(" SP_"); Serial.print(i, DEC);
#endif
    OneWData[i] = ds.read();
  }

#ifdef DS1820_DEBUG_LOW
  Serial.print("\nP=");
  Serial.print(present,HEX);
  Serial.print(", ");

  Serial.print("FRAME: ");
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    Serial.print(OneWData[i], HEX);
    Serial.print(" ");
  }

  Serial.print(" CRC=");
  Serial.print(OneWire::crc8(OneWData, 8), HEX);
  Serial.print(" (");
  if (ds.crc8(OneWData, 8) == OneWData[8]) {  // Checksum verification
    Serial.println ("OK)"); 
  }
  else {
    Serial.println ("Not OK)"); 
  }
#endif

#ifdef B_MODEL
  // 18B20 default to 12 bits resolution at power up
  LowByte = OneWData[0];
  HighByte = OneWData[1];
  Whole = (HighByte << 4) | (LowByte >> 4);
  Fract = ((LowByte & 0b1111) * 10) / 16;
  // A little bit to heavy to go through a bollean for sign in that case...
  // but keeped as is for compatibility with 18S20 code.
  SignBit = (Whole & 0b10000000) != 0;
  Whole &= 0b01111111;
#else
  // TEMPERATURE = TEMP_READ -0.25 + (COUNT_PER_C - COUNT_REMAIN) / COUNT_PER_C
  //   Multiplied by 100 to avoid floating arithmetic ==>
  //     100 * TEMP_READ -25 + (100 * COUNT_PER_C - 100*COUNT_REMAIN) / 100 * COUNT_PER_C
  //     From datasheet: Note that the COUNT PER °C register is hard-wired to 16 (10h).
  //     100 * TEMP_READ -25 + (1600 - 100*COUNT_REMAIN) / 16
  LowByte = OneWData[0];
  HighByte = OneWData[1];
  TempRead = LowByte;  // Only 8 significant bits (-55 .. +85)
  CountRemain = OneWData[6];
  
  SignBit = HighByte != 0;
  if (SignBit != 0)
    TReading = -TReading;

  Tc_100 = ((word)(TempRead >> 1)) * 100 - 25 + ((((word)(16 - CountRemain)) * 100) / 16);

  Whole = Tc_100 / 100;  // separate off the whole and fractional portions
  Fract = Tc_100 % 100;
  if (Fract % 10 >= 5)
    Fract = Fract / 10 + 1;
  else
    Fract = Fract / 10;

#endif
#ifdef DS1820_DEBUG
  sprintf(StrBuff, "1Wire: %c%d.%d",SignBit != 0 ? '-' : '+', Whole, Fract);
  Serial.println(StrBuff);
#endif

  if (ds.crc8(OneWData, 8) != OneWData[8]) Serial.println("BAD CRC !");
    TempAsStringLen = sprintf(TempAsString, "%c%d.%d",SignBit != 0 ? '-' : '+', Whole, Fract);
}

void loop() {
  // Check if a new minute elapsed
  if (millis() - previousMillis > ONE_MN_INTERVAL) {
    previousMillis = millis();
    Minutes++; ANewMinute = true;
  }
  
  // Note that even if you only want to send out packets, you still have to call rf12 recvDone periodically, because
  // it keeps the RF12 logic going. If you don't, rf12_canSend() will never return true.
  rf12_recvDone();
  
  // If one minute elapsed, check what we have to do
  if (ANewMinute) {
    ANewMinute = false;

    Get1820Tmp();

    if ((byte)(Minutes - LastSend) >= SendPeriod) {
      LastSend = Minutes;
      
      Serial.println("New Sending ");
      SendTemp = true;
    }
  }
  
  if (SendTemp && rf12_canSend()) {
    SendTemp = false;
    TempBinFormat[0] = THIS_NODE_CHANNEL;
    TempBinFormat[1] = Whole; TempBinFormat[2] = Fract;
    if (SignBit != 0) TempBinFormat[1] |= 0x80;
    rf12_sendStart(0, TempBinFormat, sizeof(TempBinFormat));
  }
}

