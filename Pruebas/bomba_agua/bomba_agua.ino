

void setup() {
  pinMode(pinSalida, OUTPUT); 
}

void loop() {
  digitalWrite(pinSalida, HIGH); 
  delay(3000);                   
  digitalWrite(pinSalida, LOW);  
}
