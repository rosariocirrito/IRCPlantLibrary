#include "IRCPlantLibrary.h"

WatchDog wd1 = WatchDog("wd1", 3000);
int count = 0;
T_Task tmr = T_Task(1000);

void setup() {
  Serial.begin(115200);
  pinMode(Q0_0, OUTPUT);
}

void loop() {
  if (tmr.q()) {
    count++;
    digitalWrite(Q0_0, wd1.q());
    //
    Serial.println("Count="+String(count)+", " +wd1.toString());
    // On from 11 to 30 (dopo 10 s)
    if (count == 5) wd1.setEnabled(true);
    if (count == 25) wd1.resetWD();    //
    if (count == 45) wd1.setEnabled(false);
    //
    if (count > 50) count = 0;
  }
}
