#include "IRCPlantLibrary.h"

//
Equip eq[2] = {
  Equip("Eq1",0, 0, false, A0, A1, false, 2000, 5000, 0, 0, false, D0, false),
  Equip("Eq2",0, 0, false, A2, A3, false, 2000, 5000, 0, 0, false, D1, false)
};

// TwinSeq(String name, Equip eq[], setIntTmr, bool bothStart, bool bothStop, bool start);
TwinSeq seq1 = TwinSeq("seq1", eq, 2000, false, false, false);

T_Task t1s = T_Task(1000);
int count = 0;

void setup() {
  Serial.begin(115200);
  //
  // Initialize Digital Input
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  // Initialize Relays outputs
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  // Initialize Opta LEDs
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
    
  if (t1s.q()){
    seq1.seq();
    Serial.println("count="+String(count)+" "+seq1.toString());
    //    
    digitalWrite(LED_D0, seq1.eq[0].isStart());
    digitalWrite(LED_D1, seq1.eq[1].isStart());
    count++;  
    if (count == 5) seq1.setSeqStart(true);
    if (count == 20) seq1.setBothStart(true);
    if (count == 40) seq1.setBothStart(false);
    if (count == 60) seq1.setBothStop(true);
    if (count == 80) seq1.setBothStop(false);
    if (count == 100) seq1.setSeqStart(false);
    if (count == 120) count = 0;
  }
}
