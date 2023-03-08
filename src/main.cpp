#include <Arduino.h>
#include <Wifi.h>


//Declaration de variables relatifs au wifi
const char* ssid = "DEATHSTAR";
const char* password = "lespaiens27";

WiFiClient client;

const char* host = "10.0.0.84";
const int port = 1500;



void setup() {

  //moniteur
  Serial.begin(115200);
  Serial.println("testing");

  //Connexion au reseau
  Serial.println("\nConnexion au réseau");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connecté au réseau");



  //Connexion au serveur
  Serial.println("\nConnexion au serveur");
  while(!client.connect(host,port))
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connecté au serveur");
  client.print("Hello testing");

}

void loop() {
  
  client.print("hello \n");

}