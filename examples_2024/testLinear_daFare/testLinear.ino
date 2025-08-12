#include "IRCPlantLibrary.h"

// FlipFlop(String name, bool trig, long timeSetFlip, long timeSetFlop);
//  objects
FlipFlop ff1 = FlipFlop("ff1", false, 500, 1000);
FlipFlop ff2 = FlipFlop("ff2", false, 1000, 1000);
FlipFlop ff3 = FlipFlop("ff3", false, 1500, 2000);
FlipFlop ff4 = FlipFlop("ff4", false, 2000, 2000);

int count = 0;
T_Task tmr = T_Task(100);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tmr.isOut()) {
    count++;
    digitalWrite(LED_D0, ff1.isOut());
    digitalWrite(LED_D1, ff2.isOut());
    digitalWrite(LED_D2, ff3.isOut());
    digitalWrite(LED_D3, ff4.isOut());
    //
    //Serial.println("Count="+String(count)+", " +ff4.toString());
    // tse On from 11 to 30 (dopo 10 s)
    if (count == 10) ff1.setTrig(true);
    if (count == 20) ff2.setTrig(true);
    if (count == 30) ff3.setTrig(true);
    if (count == 40) ff4.setTrig(true);
    if (count == 100) ff1.setTrig(false);
    if (count == 101) ff2.setTrig(false);
    if (count == 102) ff3.setTrig(false);
    if (count == 103) ff4.setTrig(false);
    //

    //
    if (count > 200) count = 0;
  }
}
