const char* ssid = "catamaran";
const char* password = "12345678";

const int agua_1 = 12; //D6
const int agua_2 = 13; //D7
const int motor1_dere = 5; //D1
const int motor1_izqui = 4; //D2

int initPWM = 150;  //tener en cuenta que el maximo es 255  = HIGH


void setup() {
  Serial.begin(9600);
  pinMode(agua_1, OUTPUT);
  pinMode(agua_2, OUTPUT);
  pinMode(motor1_dere, OUTPUT);
  pinMode(motor1_izqui, OUTPUT);
  delay(1000);
}


void loop() {
  digitalWrite(agua_1, LOW);
  digitalWrite(agua_2, HIGH);
  digitalWrite(motor1_dere, LOW);
  digitalWrite(motor1_izqui, HIGH);
}