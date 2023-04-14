// a ajouter : communication avec serveur, taring mode button, eeprom memory calibration
//
//
#include <Arduino.h>
#include <liquidCrystal.h>
#include <HX711.h>
#include <Wifi.h>

// lcd ////////////////////////////////////////////////////
const int rs=13, en=12, d4=14, d5=27 , d6=26, d7=25;
LiquidCrystal lcd(rs,en, d4, d5, d6, d7);

// Variables /////////////////////////////////////////////////////

float masse = 0; // masse lu par scale.get_units(10)

int tareModePin = 33; // Definir sur quel pin se retrouve le taring mode button (boot option a 1 pour aller dans taring mode)

// les donnees suivant seront envoyé au serveur 
struct donnees
{
  int cerises;
  int placeholder1;
  int placeholder2;
  char command = 5;
};

struct donnees envoie;


// WIFI /////////////////////////////////////////////////
char * ssid = "GRUM";
char * pass = "grumgrum";

WiFiClient client;

const char* host = "192.168.8.230"; // host johan pc
uint16_t port = 1500; // Hercules

bool connexionReseau(char * ssid, char * password )
{
   //Connexion au reseau
  lcd.print("Connexion WIFI");
  WiFi.begin(ssid,password);
  int count = 0;
  
  while(WiFi.status() != WL_CONNECTED)
  {
    lcd.setCursor(0,1);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
  lcd.print("Connexion WIFI");
  lcd.setCursor(0, 1);
  lcd.print("Success");
  delay(1000);
  return 1;
  
}



bool connexionServeur(const char * host, uint16_t port)
{
  lcd.print("Connexion server");
  int t = 0;
  while(!client.connect(host, port))
  {
    lcd.setCursor(0,1);
    lcd.print(".");
    delay(500);
    lcd.print(".");
    delay(500);
    lcd.print(".");
  };
  
  lcd.clear();
  lcd.print("Connexion server");
  lcd.setCursor(0,1);
  lcd.print("Success");
  client.print("test connect");
  return 1;
}

// hx711 /////////////////////////////////////////////

const int LOADCELL_DOUT_PIN = 18;
const int LOADCELL_SCK_PIN = 19;


// la masse de une cerise = 2300mg
// TODO : Auto calibration, peu importe la masse des cerises actuels le hx711 pourra les compter, il faut juste faire un tare avec une des cerises
const float testWeight = 2300;
const float masseCerise = 2300;

HX711 scale;
///////////

// Taring / calibration de hx711. Il faut enlever la masse de la balance d'abord 
// et puis une masse test doit etre placer sur la balance pour la calibrer apres.
// l'afficheur marque quoi faire et l'etape
void hx711Init(int pinDout, int pinSck, int testWeight)
{
  scale.begin(pinDout, pinSck);
  lcd.clear();
  lcd.print("Hold bouton for");
  lcd.setCursor(0,1);
  lcd.print("taring mode");
  delay(3000);
  bool tareMode =  digitalRead(tareModePin);
  
  // a ajouter : write next block into eeprom
  if(tareMode)
  {    
    lcd.clear();
    lcd.print("Tare mode");
    lcd.setCursor(0, 1);
    lcd.print("Enlever masse");
    delay(3000);
  
    scale.set_scale();
    scale.tare();
  
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Tare mode");
    lcd.setCursor(0,1);
    lcd.print("Placer masse");
    delay(3000);
    long calibration = scale.get_units(10);
    scale.set_scale(calibration/testWeight);
    // TODO : Write to eeprom
  }

  // TODO : else take data from eeprom

}

// cette fonction sera appeller a chaque repetition de la fonction loop(). Elle lit la valeur lu
// et affiche cet valeur
// 
float hx711Loop()
{
  if (scale.wait_ready_timeout(1000)) {
    return scale.get_units(10);
  } else {
    Serial.println("HX711 not found.");
    return 0;
  }
}
//////////////////////////////////////////////////////////////


void setup() {

  //moniteur init
  Serial.begin(115200);
  Serial.println("Test serial");

  //lcd initialisation
  lcd.begin(16,2);
  lcd.clear();

  // hx711
  //initialiser et calibrer le hx711
  //inclu un "taring mode" qui sera controlé par un bouton
  pinMode(tareModePin,INPUT);
  
  hx711Init(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, testWeight);

  lcd.clear();

  // connexion aux reseaux et serveur
  connexionReseau(ssid, pass);

  lcd.clear();
  connexionServeur(host, port);


}




void loop() {


  // lire la masse et stocker dans le struct
  Serial.print("lire la masse et stocker dans le struct\n");
  masse = hx711Loop();
  envoie.cerises = (int)round(masse / masseCerise);

  // envoyer les donnees
  Serial.print("envoyer les donnees\n");
  client.print(envoie.cerises); // TODO : remplace envoie.cerises par le struct serialized

  // print le poids
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print(masse);


  // print le nombre de cerise (round to nearest int)
  lcd.setCursor(0,1);
  lcd.print(envoie.cerises);
  
}
