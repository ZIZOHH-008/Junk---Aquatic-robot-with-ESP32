// motor3 - CENTRAL
// motor2 - IZQUIERDO
// motor1 - DERECHO


#include <ESP8266WiFi.h> //El wifi pa la node
#include <WiFiClient.h>  //El wifi para conectarse a un servidor (el de la node)
#include <ESP8266WebServer.h> //Para crear el servidor en la node


//El nombre y la clave de el robot
const char* ssid = "catamaran";
const char* password = "12345678";


const int agua_1 = 12; //D6 (GPI12)
const int agua_2 = 13; //D7 (GPI13)
const int motor3_dere = 5; //D1 (GPIO5)
const int motor3_izqui = 4; //D2 (GPIO4)

const int motor1_speed = 5;  // D1 - DERECHA
const int motor1_direction = 0;    // D3 - DERECHA
const int motor2_speed = 4;  // D2 - IZQUIERDA
const int motor2_direction = 2;    // D4 - IZQUIERDA


String command;  //Esto es para almanecar el valor dado cuando se presione algo


ESP8266WebServer server(80);  //Creamos un objeto servidor WEB que escuchará en el puerto 80

void setup() {
  Serial.begin(9600);
  pinMode(motor3_dere, OUTPUT);
  pinMode(motor3_izqui, OUTPUT);
  pinMode(agua_1, OUTPUT);
  pinMode(agua_2, OUTPUT);
  pinMode(motor2_speed, OUTPUT);
  pinMode(motor2_direction, OUTPUT);
  delay(2000);


  
// Wifi gratis :D
  WiFi.mode(WIFI_AP);           //Un solo punto de acceso
  WiFi.softAP(ssid, password);  //esto es para crear el wifi teniendo en cuenta lo valores que le di

  IPAddress myIP = WiFi.softAPIP(); //retorna la direccion IP que tiene la node
  Serial.print("AP IP address: "); //para imprimir esa string
  Serial.println(myIP); //para imprimir mi IP
 
 // Empezar a recibir información por medio de la web
     server.on ( "/", HTTP_handleRoot );  //Para manejar los INPUT
     server.onNotFound ( HTTP_handleRoot ); //por si alguien daña la IP en la app
     server.begin();   // Inicia el servidor para escuchar los INPUT
}


void HTTP_handleRoot(void) {
  if( server.hasArg("State") ){  //solo sucederá esto si es correcta la conexión
        Serial.println(server.arg("State"));
    }
    server.send ( 200, "text/html", "" );  //Enviar estado HTTP 200 (Ok) y enviar texto al navegador/cliente - Según StackOverfloow, es para decir QUE SÍ TA BIEN
    delay(100);
  }


void adelante() {
  digitalWrite(motor3_dere, LOW);
  digitalWrite(motor3_izqui, HIGH);

  analogWrite(motor1_speed, 255); 
  analogWrite(motor2_speed, 255);
  digitalWrite(motor1_direction, LOW);
  digitalWrite(motor2_direction, LOW);
  delay(500);
}


void atras(){
  digitalWrite(motor3_dere, LOW);
  digitalWrite(motor3_izqui, HIGH);
  
  analogWrite(motor1_speed, 255);
  analogWrite(motor2_speed, 255);
  digitalWrite(motor1_direction, HIGH);
  digitalWrite(motor2_direction, HIGH);
  delay(500);
}


void derecha(){
  digitalWrite(motor3_dere, HIGH);
  digitalWrite(motor3_izqui, LOW);
  
  analogWrite(motor1_speed, 255);
  analogWrite(motor2_speed, 255);
  digitalWrite(motor1_direction, LOW);
  digitalWrite(motor2_direction, HIGH);
  delay(500);
}


void izquierda(){
  digitalWrite(motor3_dere, HIGH);
  digitalWrite(motor3_izqui, LOW);
  
  analogWrite(motor1_speed, 255);
  analogWrite(motor2_speed, 255);
  digitalWrite(motor1_direction, LOW);
  digitalWrite(motor2_direction, HIGH);
  delay(500);
}


void stop(){
  digitalWrite(motor3_dere, LOW);
  digitalWrite(motor3_izqui, LOW);

  digitalWrite(agua_1, LOW);
  digitalWrite(agua_2, LOW);

  analogWrite(motor1_speed, LOW); 
  analogWrite(motor2_speed, LOW);
  digitalWrite(motor1_direction, LOW);
  digitalWrite(motor2_direction, LOW);
  delay(500);
}



void agua() {
  digitalWrite(agua_1, LOW);
  digitalWrite(agua_2, HIGH);
  delay(500);
}


void loop() {
      server.handleClient();      //escucha las conexiones entrantes para tenerlas en cuenta
          
      command = server.arg("State"); //lee el input y hacer según el valor de ese input
      if (command == "F") adelante();
      else if (command == "B") atras();
      else if (command == "L") izquierda();
      else if (command == "R") derecha();
      else if (command == "S") stop();
      else if (command == "W") agua();
}
