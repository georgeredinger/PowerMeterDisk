int QRE1113_Pin = 0; 
void setup() {
  Serial.begin(9600);
}
void loop() {
  int QRE_Value = analogRead(QRE1113_Pin);

    Serial.println(QRE_Value);
  
}


