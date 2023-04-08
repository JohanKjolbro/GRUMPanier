// a ajouter : communication avec serveur, taring mode button, eeprom memory calibration
//
//
//
//
//
#include <Arduino.h>
#include <liquidCrystal.h>
#include <HX711.h>

// Variables
float masse = 0;
// lcd ////////////////////////////////////////////////////
const int rs=13, en=12, d4=14, d5=27 , d6=26, d7=25;
LiquidCrystal lcd(rs,en, d4, d5, d6, d7);
// HX711 //////////////////////////////////////////////////
const int LOADCELL_DOUT_PIN = 18;
const int LOADCELL_SCK_PIN = 19;


// la masse de une cerise = 2300mg
const float testWeight = 2300;
const float masseCerise = 2300;

HX711 scale;
///////////

// Taring / calibration de hx711. Il faut enlever la masse de la balance d'abord 
// et puis une masse test doit etre placer sur la balance pour la calibrer apres
// l'afficheur marque quoi faire et l'etape
void hx711Init(int pinDout, int pinSck, int testWeight)
{
  lcd.setCursor(0, 0);
  lcd.print("Enlever masse");
  delay(3000);
  
  scale.begin(pinDout, pinSck);
  scale.set_scale();
  scale.tare();
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Placer masse");
  delay(3000);
  
  long calibration = scale.get_units(10);
  scale.set_scale(calibration/testWeight);
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
  hx711Init(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN, testWeight);

  lcd.clear();

}




void loop() {

  masse = hx711Loop();
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print(masse, 4);
  Serial.println(masse);


  // print le nombre de cerise (round to nearest int)
  lcd.setCursor(0,1);
  lcd.print((int)round(masse / masseCerise));
  
  
}
