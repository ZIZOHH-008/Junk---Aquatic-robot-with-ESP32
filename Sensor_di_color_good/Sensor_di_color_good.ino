//90 grados en el código giran 45 grados del servomotor no c pq

#include <ESP8266WiFi.h>       //El wifi pa la node
#include <WiFiClient.h>        //El wifi para conectarse a un servidor (el de la node)
#include <ESP8266WebServer.h>  //Para crear el servidor en la node

#include <ThingSpeak.h>
//prototype
int getRojo();
int getAzul();
int getVerde();
int redValue;
int blueValue;
int greenValue;
int rojo;
int azul;
int verde;
int minRedFreq = 556;
int minGreenFreq = 676;
int minBlueFreq = 231;
int maxRedFreq = 6542;
int maxGreenFreq = 7291;
int maxBlueFreq = 2024;
int luz;
const int s0 = 5;    //Frecuencia salida  //D1
const int s1 = 4;    //Frecuencia salida  //D2
const int s2 = 14;   //Fotodiodo - Intensidad luz o densidad del humo  //D5
const int s3 = 12;   //Fotodiodo - Intensidad luz o densidad del humo  //D6
const int out = 13;  //Salida para el sensor y entrada para arduino  //D7
String _status;

String command;     //Esto es para almanecar el valor dado cuando se presione algo
int initPWM = 150;  //tener en cuenta que el maximo es 255  = HIGH


ESP8266WebServer server(80);  //Creamos un objeto servidor WEB que escuchará en el puerto 80


//El nombre y la clave para robot
const char ssid[] = "Princesa";
const char password[] = "0322865496";

unsigned long myChannelNumber = 2713240;
const char* myWriteApiKey = "OY02GL156S2NIQIL";


WiFiClient client;

void setup() {
  Serial.begin(9600);  //activar el monitor
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);  //Salida para el sensor y entrada para arduino

  //Trabajando con toda la potencia de frecuencia
  digitalWrite(s0, LOW);
  digitalWrite(s1, HIGH);

  //Definido para medir
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);



  // Wifi gratis :D
  WiFi.mode(WIFI_STA);          //Un solo punto de acceso
  WiFi.softAP(ssid, password);  //esto es para crear el wifi teniendo en cuenta lo valores que le di

  ThingSpeak.begin(client);
}


//FUNCIONES PARA EL SENSOR
int getRojo() {
  //leer color rojo
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  rojo = pulseIn(out, LOW);
  redValue = map(rojo, minRedFreq, maxRedFreq, 255, 0);
  return redValue;
  }


int getAzul() {
  //leer color azul
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH);
  azul = pulseIn(out, LOW);
  blueValue = map(azul, minBlueFreq, maxBlueFreq, 255, 0);
  return blueValue;

}


int getVerde() {
  //leer color verde
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);
  verde = pulseIn(out, LOW);
  greenValue = map(verde, minGreenFreq, maxGreenFreq, 255, 0);
  return greenValue;
 

}



void loop() {

 if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println("Princesa");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, password); 
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  int R = getRojo();
  delay(200);
  int G = getVerde();
  delay(200);
  int B = getAzul();
  delay(200);

 // int luz = 255 - (0.2126 * R + 0.7152 * G + 0.0722 * B);
 int luz = (0.2126 * R + 0.7152 * G + 0.0722 * B);

  Serial.print("Nivel de brillo: ");
  Serial.print(luz);

  Serial.print("\t");

  Serial.print("R:");
  Serial.print(R);

  Serial.print("\t");

  Serial.print("G:");
  Serial.print(G);

  Serial.print("\t");

  Serial.print("B:");
  Serial.println(B);

  if (luz >= 80 && luz <= 255) {
  _status = "Agua clara";
  } 
  else if (luz >=  65 && luz <= 79) {
    _status = "Ligeramente turbia";
  } 
  else if (luz >= 55 && luz <= 64) {
    _status = "Moderadamente turbia";
  } 
  else if (luz >= 50 && luz <= 54) {
    _status = "Altamente turbia";
  } 
  else if (luz >= 0 && luz <= 49) {
    _status = "Extremadamente turbia";
  } 
  else {
    _status = "Fuera de rango";
  }

  Serial.println(_status);


  ThingSpeak.setField(1, luz);
  ThingSpeak.setStatus(_status);

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteApiKey);

  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(25000); 
}


void Range(int minRange, int maxRange, String status) {
  if (luz >= minRange && luz <= maxRange) {
    _status = status;
    Serial.println(status);
  }
}
