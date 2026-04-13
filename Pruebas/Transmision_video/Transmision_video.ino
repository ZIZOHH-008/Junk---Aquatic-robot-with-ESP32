#include <ESP8266WiFi.h> //El wifi pa la node
#include <WiFiClient.h>  //El wifi para conectarse a un servidor (el de la node)
#include <ESP8266WebServer.h> //Para crear el servidor en la node


const char* ssid="Camara2";
const char* password="12345678";

ESP8266WebServer server(80); 


void setup() {
  WiFi.mode(WIFI_AP);           //Un solo punto de acceso
  WiFi.softAP(ssid, password);  //esto es para crear el wifi teniendo en cuenta lo valores que le di

}


//El pc y un celular se conectan a la red de la node
//Se instala "IP webcam", se inicia servidor, y nos da dos direcciones, las cuales se deben poner en google




void loop() {
  // put your main code here, to run repeatedly:

}
