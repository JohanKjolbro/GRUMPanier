#include <Arduino.h>
#include <Wifi.h>
#include <LiquidCrystal.h>


//Declaration de variables relatifs au wifi
const char* ssid = "DEATHSTAR";
const char* password = "lespaiens27";

WiFiClient client;

const char* host = "10.0.0.96";
const int port = 1500;

//Declaration de variables relatifs au lcd
const int rs=13, en=12, d4=14, d5=27 , d6=26, d7=25;
LiquidCrystal lcd(rs,en, d4, d5, d6, d7);

int t;

class afficheur
{

};

class connexion
{

};



void setup() {

  //moniteur
  Serial.begin(115200);
  Serial.println("testing23");
  //lcd
  lcd.begin(16,2);
  
  
  

  
  //Connexion au reseau
  Serial.println("\nConnexion au réseau");
  lcd.print("Connexion WIFI");
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connecté au réseau");
  lcd.print("Connecté WIFI");


  //Connexion au serveur
  Serial.println("\nConnexion au serveur");
  lcd.println("\nConnexion server");
  while(!client.connect(host,port))
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connecté au serveur");
  Serial.print("Connecté server");
  client.print("test1\n");

}


void loop() {

  client.println(t++);
  
  lcd.setCursor(0,0);
  lcd.print("hello loop");
  lcd.setCursor(0,1);
  lcd.print("hello loop 2");


}