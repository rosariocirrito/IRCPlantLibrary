#include "IRCPlantLibrary.h"

//  objects
FlipFlop ff1 = FlipFlop("ff1", false, 500, 1000);
FlipFlop ff2 = FlipFlop("ff2", false, 1000, 2000);
int count = 0;
T_Task tmr = T_Task(100);

void setup() {
  Serial.begin(115200);
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
}

void loop() {
  if (tmr.q()) {
    count++;
    digitalWrite(LED_D0, ff1.q());
    digitalWrite(LED_D1, ff2.q());
    //
    if (count == 10) ff1.setTrig(true);
    if (count == 20) ff2.setTrig(true);
    if (count == 100) ff1.setTrig(false);
    if (count == 101) ff2.setTrig(false);
    //
    if (count > 110) count = 0;
  }
}
