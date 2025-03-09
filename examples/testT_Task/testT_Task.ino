#include "IRCPlantLibrary.h"

T_Task t2s = T_Task(2000);
T_Task t5s = T_Task(5000);

void setup() { Serial.begin(115200); }

void loop() {
  if (t2s.q()) task1();
  if (t5s.q()) task2();
}

void task1(){ Serial.println(t2s.toString()); }
void task2(){ Serial.println(t5s.toString()); }
