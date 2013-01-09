#if not defined(ARDUINO)
  #error not an arduino compiler
#endif
int QRE1113_Pin = 0; 
void setup() {
  Serial.begin(9600);
}
#include "detector.cpp"
void loop() {
  int QRE_Value = analogRead(QRE1113_Pin);

    Serial.println(QRE_Value);
  
}


