#include "IRCPlantLibrary.h"

// parameters
long setT= 10000;

// timer objects
// TimerSE(String name, bool trig, long timeSet);
TimerSE tse = TimerSE("T_SE",false,setT);
//  TimerSV(String name, bool trig, long timeSet);
TimerSV tsv = TimerSV("T_SV",false,setT);
int count = 0;
T01s tsec = T01s();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (tsec.isOut()) {
    count++;
    digitalWrite(LED_D0, tse.isOut());
    digitalWrite(LED_D1, tsv.isOut());
    //
    Serial.println("Count="+String(count)+", " +tse.toString()+", " +tsv.toString());
    // tse On from 11 to 30 (dopo 10 s)
    if (count > 1) tse.setTrig(true);
    if (count > 30) tse.setTrig(false);
    // tse On never from 41 to 45 (less than 10 s)
    if (count > 41) tse.setTrig(true);
    if (count > 45) tse.setTrig(false);
    // tsv On from 25 to 35 (10 s)
    if (count > 25) tsv.setTrig(true);
    if (count > 30) tsv.setTrig(false);
    //
    if (count > 60) count = 0;
  }
}
