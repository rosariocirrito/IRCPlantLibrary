#include "IRCPlantLibrary.h"


//
Equip eq[3] = {
  //Equip eq1 = Equip("Eq1",cmdRem, cmdLoc, extLock, pinOn, pinTrm, go, setIntTmr, setFBTmr, nr, hh, hhReset, pinOut, debug);
  Equip("Eq1",0, 0, false, I0_3, I0_4, false, 2000, 5000, 0, 0, false, Q0_0, false),
  Equip("Eq2",0, 0, false, I0_5, I0_6, false, 2000, 5000, 0, 0, false, Q0_1, false),
  Equip("Eq3",0, 0, false, I0_7, I0_8, false, 2000, 5000, 0, 0, false, Q0_2, false)
};

// ParallelSeq(String name, int nrEq, Equip eq[],bool allStart, bool allStop, bool more, bool less, bool start);
ParallelSeq seq1 = ParallelSeq("seq1", 3, eq,false, false, false, false, true);
T_Task t1s = T_Task(1000);
int count = 0;
  
void setup() {
  Serial.begin(115200);
  //
  // Initialize Digital Input
  pinMode(I0_3, INPUT);
  pinMode(I0_4, INPUT);
  pinMode(I0_5, INPUT);
  pinMode(I0_6, INPUT);
  pinMode(I0_7, INPUT);
  pinMode(I0_8, INPUT);
  // Initialize Digital outputs
  pinMode(Q0_0, OUTPUT);
  pinMode(Q0_1, OUTPUT);
  pinMode(Q0_2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  seq1.start = true; //digitalRead(A7);
  if (t1s.q()){
    count++;
    //
    seq1.seq();
    Serial.println("count="+String(count)+" "+seq1.toString());
    //    
    digitalWrite(Q0_0, seq1.eq[0].isStart());
    digitalWrite(Q0_1, seq1.eq[1].isStart());
    digitalWrite(Q0_2, seq1.eq[2].isStart());
    //
    if (count == 15) seq1.setMore(true);
    if (count == 40) seq1.setMore(false);
    if (count == 40) seq1.setLess(true);
    if (count == 60) seq1.setLess(false);
    if (count == 90) {
      seq1.setMore(true);
      seq1.setAllStart(true);
    } 
    if (count == 90) {
      seq1.setMore(true);
      seq1.setAllStart(true);
    } 
    if (count == 120) {
      seq1.setMore(false);
      seq1.setAllStart(false);
    } 
    if (count == 140) {
      seq1.setLess(true);
      seq1.setAllStop(true);
    } 
    if (count == 160) {
      seq1.setLess(false);
      seq1.setAllStop(false);
    } 

    if (count == 200) count = 0;

  //
  }
}
