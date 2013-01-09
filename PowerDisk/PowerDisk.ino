int QRE1113_Pin = 0; 
void setup() {
  Serial.begin(9600);
}
void loop() {
  int QRE_Value = analogRead(QRE1113_Pin);
  if(QRE_Value < 100) {
    Serial.println(QRE_Value);
  }
}


