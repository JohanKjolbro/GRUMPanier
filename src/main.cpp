#include <Arduino.h>
#include <Wifi.h>
#include <LiquidCrystal.h>


//Declaration de variables relatifs au wifi
const char* ssidHome;
const char* passwordHome; 

WiFiClient client;

const char* host = "10.0.0.96";
uint16_t port = 1500;

//Declaration de variables relatifs au lcd
const int rs=13, en=12, d4=14, d5=27 , d6=26, d7=25;
LiquidCrystal lcd(rs,en, d4, d5, d6, d7);

int t;

// @ xavier why pass pointers?
bool connexionReseau(char * ssid, char * password )
{
   //Connexion au reseau
  Serial.println("\nConnexion au reseau");
  lcd.print("Connexion WIFI");
  WiFi.begin(ssid,password); // how is this a value? i dont get it
  int count = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    if(count > 1000);
    {
      return 0;
    }
    Serial.print(".");
    delay(100);
    count++;
  }
  Serial.print("Connecte au reseau");
  lcd.print("Connecte WIFI");
  return 1;
}

bool connexionServeur(const char * ip, uint16_t port) {
{
  Serial.println("\nConnexion au serveur");
  lcd.println("\nConnexion server");
  while(!client.connect(ip, port))
  {
    Serial.print(".");
    delay(100);
  }
  Serial.print("Connecté au serveur");
  Serial.print("Connecté server");
  client.print("test1\n");
}


void setup() {

  //moniteur
  Serial.begin(115200);
  Serial.println("Test serial");
  //lcd
  lcd.begin(16,2);
  

  
  //Connexion au reseau
  connexionReseau(ssidHome, passwordHome);



  //Connexion au serveur
  connexionServeur(host, port);

}


void loop() {

  client.println(t++);
  
  lcd.setCursor(0,0);
  lcd.print("hello loop");
  lcd.setCursor(0,1);
  lcd.print("hello loop 2");


}