#include <Arduino.h>
#include <iostream>
#include <sstream>
#include <Wifi.h>
#include <LiquidCrystal.h>
#include <HX711.h>


//Declaration de variables relatifs au WIFI
//Les donnees ici devront etre ceux du modem portable, sinon ils seront a changer
char* ssid = "Grum";
char* password = "grumgrum"; 



// Serveur
// Il faudra remplacer quelques variables dans cette section
WiFiClient client;

char* host = "0.0.0.0"; // <- Remplacer 0.0.0.0 avec IP du serveur
int port = 0000; // <- Remplacer avec port du serveur




// HX711

HX711 scale;
int uneCerise = 1;

void hx711Init(int LOADCELL_DOUT_PIN, int LOADCELL_SCK_PIN)
{
  lcd.clear();
  lcd.print("Taring... ");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale();
  scale.tare();

  delay(3000);  

  lcd.clear();
  lcd.print("Placer masse");
  delay(5000);

  long calibration = scale.get_units(10);

  scale.set_scale(calibration/uneCerise);
}






//Declaration de variables relatifs au lcd

const int rs=13, en=12, d4=14, d5=27 , d6=26, d7=25;
LiquidCrystal lcd(rs,en, d4, d5, d6, d7);




// Message definition
// Si jamais vous aviez une d/finition de message differente, vous pourriez la modifier ici
// et le code enverra le bon format
// il faudra changer l'affectation dans le loop du hx711 si un changement a lieu
// J'ai pris pour acquis que vous utilisiez le format qu'a ete mentionn/

struct MSG
{
    int x = 0;
    int y = 0;
    int z = 0;
    char c = 'p';
};

struct MSG msg;







bool connexionReseau(char * ssid, char * password )
{
   //Connexion au reseau
  Serial.println("\nConnexion au reseau");
  lcd.print("Connexion WIFI");
  WiFi.begin(ssid,password);

  int count = 0;
  while(WiFi.status() != WL_CONNECTED)
  {
    if(count > 10000);
    {
      return 0;
    }
    Serial.print(".");
    delay(100);
    count++;
  }

  Serial.print("Connecter au reseau");
  lcd.print("Connexion reussi");
  return 1;
}










bool connexionServeur(char * host, int port)
{
  Serial.println("\nConnexion au serveur");
  lcd.println("\nConnexion server");

  int count = 0;
  while(!client.connected())
  {
    if(count > 10000)
    {
      return 0;
    }
    Serial.print(".");
    delay(100);
    count++;
  }
  return 1;
}











void setup() {



  //moniteur

  Serial.begin(115200);
  Serial.println("Test serial");



  //lcd

  lcd.begin(16,2);
  lcd.clear();





  
  //Connexion au reseau

  if(connexionReseau(ssid, password))
  {
    Serial.print("Connecter au reseau");
    lcd.print("Connexion reussi");
  }
  else
  {
    Serial.print("Erreur connexion");
    lcd.print("Erreur connexion");
  }
  




  //Connexion au serveur

  if(connexionServeur(host, port))
  {
    Serial.print("Connecté au serveur");
    lcd.print("Serveur reussi");
  }
  else
  {
    Serial.print("Erreur de connexion au serveur");
    lcd.print("Serveur erreur");
  }
  




  // HX711

  hx711Init(5, 18);

  
  


}


void loop() {
  
  
  if (scale.wait_ready_timeout(1000)) {
    long reading = scale.get_units(10);
    lcd.clear();
    lcd.print("Cerises : ");
    lcd.print(reading);


    // Ici, le nombre de cerises va dans msg.x
    msg.x = reading;

    // Serialization of the struct
    std::ostringstream oss;
    oss.write(reinterpret_cast<const char*>(&msg), sizeof(msg));
    std::__cxx11::string serialized = oss.str();        
    String ardSerialized = String(serialized.c_str());  // convert std::string to arduino string
    client.print(ardSerialized);



  } else {
    lcd.clear();
    lcd.print("Erreur HX711.");
    lcd.setCursor(1,0);
    lcd.print("hx711 not found");
  }
  
  delay(100);

}