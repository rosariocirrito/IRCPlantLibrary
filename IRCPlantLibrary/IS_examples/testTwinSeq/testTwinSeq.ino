#include "IRCPlantLibrary.h"

//
Equip eq[2] = {
  // Equip eq1 = Equip("Eq1",cmdRem, cmdLoc, extLock, pinOn, pinTrm, go, setIntTmr, setFBTmr, nr, hh, hhReset, pinOut, debug);
  Equip("Eq1",0, 0, false, I0_2, I0_3, false, 2000, 5000, 0, 0, false, Q0_0, false),
  Equip("Eq2",0, 0, false, I0_4, I0_5, false, 2000, 5000, 0, 0, false, Q0_1, false)
};

// TwinSeq(String name, Equip eq[], setIntTmr, bool bothStart, bool bothStop, bool start);
TwinSeq seq1 = TwinSeq("seq1", eq, 2000, false, false, false);

T_Task t1s = T_Task(1000);
int count = 0;

void setup() {
  Serial.begin(115200);
  //
  // Initialize Digital Input
  pinMode(I0_2, INPUT);
  pinMode(I0_3, INPUT);
  pinMode(I0_4, INPUT);
  pinMode(I0_5, INPUT);
  // Initialize Digital outputs
  pinMode(Q0_0, OUTPUT);
  pinMode(Q0_1, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
    
  if (t1s.q()){
    seq1.seq();
    Serial.println("count="+String(count)+" "+seq1.toString());
    //    
    digitalWrite(Q0_0, seq1.eq[0].isStart());
    digitalWrite(Q0_1, seq1.eq[1].isStart());
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
