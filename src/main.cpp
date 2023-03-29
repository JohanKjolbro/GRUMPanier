#include <Arduino.h>
#include <HX711.h>

  // HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 15;
const int LOADCELL_SCK_PIN = 2;

const int testWeight = 23;

HX711 scale;

void setup() {

  Serial.begin(115200);

  Serial.print("Taring... Enlever toute masse");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  scale.set_scale();
  scale.tare();

  delay(3000);

  Serial.print("Placer masse");
  delay(5000);

  long calibration = scale.get_units(10);

  scale.set_scale(calibration/testWeight);


  

}

void loop() {

  if (scale.wait_ready_timeout(1000)) {
    long reading = scale.get_units(10);
    Serial.print("Masse : ");
    Serial.println(reading);
    Serial.print("g");
  } else {
    Serial.println("HX711 not found.");
  }

  delay(1500);
  
}