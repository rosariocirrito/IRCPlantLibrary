#include "IRCPlantLibrary.h"

int count = 0;
T15min tmin = T15min();
bool ledOn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_D0, OUTPUT); 
  Serial.println("Count="+String(count)+" millis()"+String(millis())+", "+tmin.toString());
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tmin.isOut()) {
    count++;
    digitalWrite(LED_D0, ledOn);
    ledOn = !ledOn;
    //
    Serial.println("Count="+String(count)+" millis()"+String(millis())+", "+tmin.toString());
    //
    if (count > 60) count = 0;
  }
}
