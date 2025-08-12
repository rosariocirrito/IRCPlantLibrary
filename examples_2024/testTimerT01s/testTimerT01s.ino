#include "IRCPlantLibrary.h"

int count = 0;
T01s tsec = T01s();
bool ledOn = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_D0, OUTPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tsec.isOut()) {
    count++;
    digitalWrite(LED_D0, ledOn);
    ledOn = !ledOn;
    //
    Serial.println("Count="+String(count)+"millis()"+String(millis()));
    //
    if (count > 60) count = 0;
  }
}
