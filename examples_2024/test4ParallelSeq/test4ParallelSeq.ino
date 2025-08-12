#include "IRCPlantLibrary.h"


//
Equip eq[4] = {
  Equip("Eq1",0, 0, false, A0, A1, false, 2000, 5000, 0, 0, false, D0, false),
  Equip("Eq2",0, 0, false, A2, A3, false, 2000, 5000, 0, 0, false, D1, false),
  Equip("Eq3",0, 0, false, A4, A5, false, 2000, 5000, 0, 0, false, D2, false),
  Equip("Eq4",0, 0, false, A6, A7, false, 2000, 5000, 0, 0, false, D3, false)
};

// ParallelSeq(String name, int nrEq, Equip eq[],bool allStart, bool allStop, bool more, bool less, bool start);
ParallelSeq seq1 = ParallelSeq("seq1", 4, eq,false, false, false, false, true);
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
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  // Initialize Relays outputs
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  // Initialize Opta LEDs
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
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
    digitalWrite(LED_D0, seq1.eq[0].isStart());
    digitalWrite(LED_D1, seq1.eq[1].isStart());
    digitalWrite(LED_D2, seq1.eq[2].isStart());
    digitalWrite(LED_D3, seq1.eq[3].isStart());
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
