#include <Arduino.h>

const int pinBeam1 = 5;
const int pinLed = 2;
bool beamCurrentState = 0, beamLastState = 0;

void setup() {

  //moniteur

  Serial.begin(115200);

  Serial.println("Test serial");

  pinMode(pinLed, OUTPUT);
  pinMode(pinBeam1, INPUT);
  digitalWrite(pinBeam1,HIGH); //pourquoi initialiser le pullup ?

}




void loop() {

  beamCurrentState = digitalRead(pinBeam1);
  if(beamCurrentState == HIGH)
  {
    digitalWrite(pinLed, LOW);
  }
  else
  {
    digitalWrite(pinLed, HIGH);
  }

  if(beamLastState && !beamCurrentState)
  {
    Serial.print("unbroken");
  }
  if(!beamLastState && beamCurrentState)
  {
    Serial.print("broken");
  }

  beamLastState = beamCurrentState;

}
