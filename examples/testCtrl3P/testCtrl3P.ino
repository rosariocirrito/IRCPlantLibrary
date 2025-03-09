#include "IRCPlantLibrary.h"

Ctrl3P ctrl1 = Ctrl3P("ctrl_1", false, 500, 1000, 1000, 2000);
TNs t1 = TNs(15);
T01min t2 = T01min();
bool flag1 = false;
bool flag2 = true;

void setup() {
  Serial.begin(115200);
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  ctrl1.setTrigPlus(false);
  ctrl1.setTrigMinus(false);
}

void loop() {
    if(t1.q()) flag1 = !flag1;
    if(t2.q()) flag2 = !flag2;
    Serial.println(t1.toString() + " flag1="+String(flag1)+" " + " flag2="+String(flag2)+" "+ctrl1.toString());
    digitalWrite(LED_D0, ctrl1.qPlus());
    digitalWrite(LED_D1, ctrl1.qMinus());
    //
    ctrl1.setTrig(flag2);
    //
    if (flag1) {
      ctrl1.setTrigPlus(true);
      ctrl1.setTrigMinus(false);
    }
    else {
      ctrl1.setTrigPlus(false);
      ctrl1.setTrigMinus(true);
    }
  
}
