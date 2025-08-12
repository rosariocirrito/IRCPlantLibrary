#include "IRCPlantLibrary.h"

//
int cmdRem = 0;
int cmdLoc = 0;  
bool extLock = false;
int pinOn = I0_5;
int pinTrm = I0_6; 
bool go = false;
unsigned long setIntTmr = 2000;
unsigned long setFBTmr = 10000;
int nr = 0;
int hh = 0;
bool hhReset = false;
int pinOut = R0_0;
bool debug = false;
//
Equip eq1 = Equip("Eq1",cmdRem, cmdLoc, extLock, pinOn, pinTrm, go, setIntTmr, setFBTmr, nr, hh, hhReset, pinOut, debug);
//
T_Task t1s = T_Task(1000);

void setup() {
  Serial.begin(115200);
  //
  // Initialize Digital Input
  pinMode(I0_4, INPUT);
  pinMode(I0_5, INPUT);
  pinMode(I0_6, INPUT);
  
  // Initialize Relays outputs
  pinMode(R0_0, OUTPUT);
  
}

void loop() {
  // 1. legge il DI che è in logica negata, il contatto NO chiude a GND
  if (t1s.q()){
    eq1.setGo(digitalRead(I0_4));
    //
    eq1.updateMotor(); 
    //
    //String str = eq1.getName() +" stato= "+eq1.getStatus();
    String str = eq1.toString();
    Serial.println(str);

  }
}
