#include "IRCPlantLibrary.h"

//
int cmdRem = 0;
int cmdLoc = 0;  
bool extLock = false;
int pinOn = A0;
int pinTrm = A1; 
bool go = false;
long setIntTmr = 2000;
long setFBTmr = 10000;
int nr = 0;
int hh = 0;
bool hhReset = false;
//int pinOut = LED_D0;
int pinOut = D0;
bool debug = false;
//
Equip eq1 = Equip("Eq1",cmdRem, cmdLoc, extLock, pinOn, pinTrm, go, setIntTmr, setFBTmr, nr, hh, hhReset, pinOut, debug);
//
T_Task t1s = T_Task(1000);

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
  //pinMode(D1, OUTPUT);
  //pinMode(D2, OUTPUT);
  //pinMode(D3, OUTPUT);
  // Initialize Opta LEDs
  pinMode(LED_D0, OUTPUT);
  pinMode(LED_D1, OUTPUT);
  pinMode(LED_D2, OUTPUT);
  pinMode(LED_D3, OUTPUT);
  pinMode(LED_RESET, OUTPUT);
  pinMode(LED_USER, OUTPUT);
  pinMode(BTN_USER, INPUT);
}

void loop() {
  // 1. legge il DI che è in logica negata, il contatto NO chiude a GND
  if (t1s.q()){
    eq1.setGo(digitalRead(A7));
    //
    eq1.updateMotor(); 
    //
    //String str = eq1.getName() +" stato= "+eq1.getStatus();
    String str = eq1.toString();
    Serial.println(str);

  }
}
