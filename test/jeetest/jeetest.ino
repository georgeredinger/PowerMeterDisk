#include <JeeLib.h>
#include <OneWire.h>
#include <DallasTemperature.h>



long counter;

#define ONE_WIRE_BUS 7


OneWire oneWire(ONE_WIRE_BUS);


DallasTemperature sensors(&oneWire);

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

void setup () {
  // node 1, group 212, 915 MHz
 // Sleepy::loseSomeTime(32);
  rf12_initialize(1, RF12_915MHZ, 212);
  rf12_control(0xC68F);
  sensors.begin(); // default 9 bit measurement
  // 12 bit example: https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/examples/WaitForConversion2/WaitForConversion2.pde
  
}
void loop () {
  struct  {
    int c,t,b;
  } mess;
  int batt;
  ++counter;
  sensors.requestTemperatures(); // Send the command to get temperatures
  int t=int(sensors.getTempCByIndex(0)*10);
  batt = map(analogRead(1), 0, 1023, 0,6600) ;
  mess.c=counter;
  mess.t=t;
  mess.b=batt;
  while (!rf12_canSend())
    rf12_recvDone();
  rf12_sendStart(0, &mess, sizeof mess);

  //rf12_sleep(RF12_SLEEP);
//  Sleepy::loseSomeTime(10000);
delay(10000);//
 // rf12_sleep(RF12_WAKEUP);
//delay(1000);
}



