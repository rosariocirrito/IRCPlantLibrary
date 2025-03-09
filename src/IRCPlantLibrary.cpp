#include "IRCPlantLibrary.h"

/**
TimerSE
*/
TimerSE::TimerSE(String _name, bool _trig, unsigned long _PT){
  name = _name;
  trig = _trig;
  PT = _PT;
  startTime = millis();
}
//
void TimerSE::setTrig(bool _trig){trig = _trig;}
//
bool TimerSE::q(){
  if (!oldTrig && trig) startTime = millis();
  oldTrig = trig;
  return (trig && ((millis() - startTime) > PT));
}
//
unsigned long TimerSE::ct(){return millis() - startTime;}
//
String TimerSE::toString() {
  String str = "Il timerSE "+name;
  str+= ", trig="+String(trig);
  str+= ", timeset="+String(PT)+"ms";
  str+= ", elapsedTime="+String(ct())+"ms";
  str+= ", out="+ String(q());
  return str;
}

/**
TimerSV
*/
TimerSV::TimerSV(String _name, bool _trig, unsigned long _PT){
  name = _name;
  trig = _trig;
  PT = _PT;
  startTime = millis();
}
//
void TimerSV::setTrig(bool _trig){trig = _trig;}
//
bool TimerSV::q(){
  if (!oldTrig && trig){
    startTime = millis();
    out = true;
  }  
  if ((millis() - startTime) > PT) out=false;
  oldTrig = trig;
  return out;
}
unsigned long TimerSV::ct(){return millis() - startTime;}
String TimerSV::toString() {
  String str = "Il timerSV "+name;
  str+= ", trig="+String(trig);
  str+= ", timeset="+String(PT)+"ms";
  str+= ", elapsedTime="+String(ct())+"ms";
  str+= ", out="+ String(q());
  return str;
}

/**
TNs
*/
TNs::TNs(unsigned int _s){
  s=_s;
  startTime = millis();
  }
//
bool TNs::q(){
  bool out = ((millis() - startTime) >= (s*1000));
  if (out) startTime = millis();
  return out;
}
String TNs::toString() {
  String str = "T01s elapsedTime="+String(millis() - startTime)+"ms";
  return str;
}

/**
T01s
*/
T01s::T01s(){startTime = millis();}
//
bool T01s::q(){
  bool out = ((millis() - startTime) >= 1000);
  if (out) startTime = millis();
  return out;
}
String T01s::toString() {
  String str = "T01s elapsedTime="+String(millis() - startTime)+"ms";
  return str;
}

/**
T01min
*/
T01min::T01min(){startTime = millis();}
//
bool T01min::q(){
  bool out = ((millis() - startTime) >= (60*1000));
  if (out) startTime = millis();
  return out;
}
String T01min::toString() {
  String str = "T01min elapsedTime="+String(millis() - startTime)+"ms";
  return str;
}

/**
T15min
*/
T15min::T15min(){startTime = millis();}
//
bool T15min::q(){
  bool out = ((millis() - startTime) >= (15*60*1000));
  if (out) startTime = millis();
  return out;
}
String T15min::toString() {
  String str = "T15min elapsedTime="+String(millis() - startTime)+"ms";
  return str;
}

/**
T_Task
*/
T_Task::T_Task(long _ms){
  PT = max(10,_ms);
  startTime = millis();
}
//
bool T_Task::q(){
  bool out = ((millis() - startTime) >= PT);
  if (out) startTime = millis();
  return out;
}
String T_Task::toString() {
  String str = "Task with timeSet= "+String(PT);
  str+= " ms has been executed at "+String(millis())+" ms";
  return str;
}

/*
  WatchDog
*/
WatchDog::WatchDog(String _name, unsigned long _timeSet){
  name = _name;
  enabled = false;
  reset = false;
  twd.PT =  _timeSet;
} 
//
void WatchDog::setEnabled(bool up){
  enabled = up;
  twd.setTrig(up);
  }
void WatchDog::resetWD(){
  if (enabled){
    twd.setTrig(false);
    bool done = !twd.q();
    if (done) twd.setTrig(true);
    }
  }
  //
  bool WatchDog::isEnabled(){return enabled;}
  bool WatchDog::q(){return twd.q();}
  String WatchDog::toString(){
  String str = "WatchDog with timeSet= "+String(twd.PT);
  str+= " ms has elapsedTime="+String(twd.ct())+"ms";
  str+= " and out="+String(twd.q());
  return str;
  }


/**
  FlipFlop
*/
FlipFlop::FlipFlop(String _name, bool _trig, unsigned long _timeSetFlip, unsigned long _timeSetFlop){
  name = _name;
  trig = _trig;
  tmrFlip.PT=_timeSetFlip;
  tmrFlop.PT=_timeSetFlop;
}
void FlipFlop::setTrig(bool _trig){trig = _trig;}
void FlipFlop::setTimeFlip(unsigned long _timeSetFlip){tmrFlip.PT=_timeSetFlip;}
void FlipFlop::setTimeFlop(unsigned long _timeSetFlop){tmrFlop.PT=_timeSetFlop;}
//
bool FlipFlop::q(){
  if (!oldTrig && trig) tmrFlip.setTrig(true);  
  out=(trig && tmrFlip.q()); 
  if (out) tmrFlip.setTrig(false);
  if (oldOut && !out) tmrFlop.setTrig(true);  
  if(tmrFlop.q()) {  
    tmrFlop.setTrig(false);
    tmrFlip.setTrig(true);
  }
  oldTrig = trig;
  oldOut = out;
  return out;
}
String FlipFlop::toString() {
  String str = "FlipFlop "+name + ", trig="+String(trig)+", out="+ String(q());
  str+= ", setTimeFlip="+String(tmrFlip.PT)+"ms" + String(tmrFlip.ct())+"ms";
  str+= ", setTimeFoip="+String(tmrFlop.PT)+"ms" + String(tmrFlop.ct())+"ms";
  return str;
}

/**
Ctrl3P
*/

Ctrl3P::Ctrl3P(
  String _name, bool _trig, 
  unsigned long _timeSetFlipPlus, unsigned long _timeSetFlopPlus, 
  unsigned long _timeSetFlipMinus, unsigned long _timeSetFlopMinus){
  name = _name;
  trig = _trig;
  trigPlus = false;
  trigMinus = false;
  ffPlus.setTimeFlip(_timeSetFlipPlus);
  ffPlus.setTimeFlop(_timeSetFlopPlus);
  ffMinus.setTimeFlip(_timeSetFlipMinus);
  ffMinus.setTimeFlop(_timeSetFlopMinus);
}
void Ctrl3P::setTrig(bool _trig){trig = _trig;}
void Ctrl3P::setTrigPlus(bool _trig){trigPlus = _trig;}
void Ctrl3P::setTrigMinus(bool _trig){trigMinus = _trig;}
void Ctrl3P::setTimeFlipPlus(unsigned long _timeSetFlip){ffPlus.setTimeFlip(_timeSetFlip);}
void Ctrl3P::setTimeFlopPlus(unsigned long _timeSetFlop){ffPlus.setTimeFlop(_timeSetFlop);}
void Ctrl3P::setTimeFlipMinus(unsigned long _timeSetFlip){ffMinus.setTimeFlip(_timeSetFlip);}
void Ctrl3P::setTimeFlopMinus(unsigned long _timeSetFlop){ffMinus.setTimeFlop(_timeSetFlop);}
//
bool Ctrl3P::qPlus(){
  ffPlus.setTrig(trig && trigPlus);
  ffMinus.setTrig(trig && trigMinus);
  return (trig && ffPlus.q() && !ffMinus.q());
}
bool Ctrl3P::qMinus(){
  ffMinus.setTrig(trig && trigMinus);
  ffPlus.setTrig(trig && trigPlus);
  return (trig && !ffPlus.q() && ffMinus.q());
}
String Ctrl3P::toString() {
  String str = "Ctrl3p "+name + ", trig="+String(trig)+ ", trigPlus="+String(trigPlus)+ ", trigMinus="+String(trigMinus);
  str+= ", outPlus="+ String(qPlus()) + ", outMinus="+ String(qMinus());  
  return str;
}

/**
  Equip
*/
// public methods
String Equip::toString() {
  String str = "name/cmdRem/cmdLoc/extLock/pinOn/pinTrm/go/setIntTmr/setFBTmr/nr/hh/hhReset/pinOut/debug/start/status";
  str+='\n' +name;
  str+= "/"+String(cmdRem); 
  str+= "/"+String(cmdLoc);
  str+= "/"+String(extLock);
  str+= "/"+String(pinOn);
  str+= "/"+String(pinTrm);
  str+= "/"+String(go);
  str+= "/"+String(setIntTmr);
  str+= "/"+String(setFBTmr);
  str+= "/"+String(nr);
  str+= "/"+String(hh);
  str+= "/"+String(hhReset);
  str+= "/"+String(pinOut);
  str+= "/"+String(debug);
  str+= "/"+String(start);
  str+= "/"+getStatus();
  return str;
}

Equip::Equip(
  String _name, int _cmdRem, int _cmdLoc, bool _extLock,
  int _pinOn, int _pinTrm,
  bool _go,
  long _setIntTmr, long _setFBTmr,
  int _nr, int _hh, bool _hhReset,
  int _pinOut,
  bool _debug
){
  //
  name = _name;
  cmdRem = _cmdRem;
  cmdLoc = _cmdLoc;
  extLock = _extLock;
  pinOn = _pinOn;
  pinTrm = _pinTrm;
  go = _go;
  setIntTmr = _setIntTmr;
  setFBTmr = _setFBTmr;
  nr = _nr;
  hh = _hh;
  hhReset = _hhReset;
  pinOut = _pinOut;
  debug = _debug;
  //
  interd = false;
  fbNok = false;
  ready = false;
  trigTmrInterd = false;
  trigTmrFbNok = false;
  fScan = true;
}


void Equip::updateMotor(){
  virtualDI();
  startEquip();  
  virtualDO();     
  fScan = false;
}

String Equip::getName(){ return name;}
String Equip::getStatus(){  
  //
  status="???";
  if (on){
    if (start) status="ON";
    if (!start) status="ON_SEL";
  }
  if (!on) {
    if (ready && !interd && !fbNok) status="OFF";
    if(cmdRem==2) status="REM_ZERO";
    if(cmdLoc==2) status="LOC_ZERO";
    if(trm && (cmdLoc<2 )) status="TERM_PROT";
    if(fbNok && (cmdLoc<2 )) status="FB_NOK";
    if(interd && (cmdLoc<2 )) status="INTERD";
    if(extLock && (cmdLoc<2 )) status="EXT_LOCK";
  }
  //  
  return status; 
}

bool Equip::isReady(){  return ready;  }
bool Equip::isInterd(){  return interd;  }
bool Equip::isFBNok(){  return fbNok;  }
bool Equip::isOn(){  return on;  }
bool Equip::isGo(){  return go;  }
bool Equip::isStart(){  return start;  }
//
void Equip::setGo(bool _go){ go = _go;}
void Equip::setCmdRem(int _cmdRem){ cmdRem = _cmdRem; }	
void Equip::setCmdLoc(int _cmdLoc) { cmdLoc = _cmdLoc; }

// ****************** private methods **********
void Equip::virtualDI(){
  if (!debug) on = digitalRead(pinOn);
  else on = start;
  if (!debug) trm = digitalRead(pinTrm);
  else trm = true;
}

void Equip::startEquip(){
  //
  if (!fScan && on) oldOn = on;
  if (!on && oldOn) tmrInterd.setTrig(true);
  //if (tmrInterd.isOut()) tmrInterd.setTrig(false);
  if (tmrInterd.q()) tmrInterd.setTrig(false);
  oldOn = on;
  //interd = tmrInterd.isOut();
  interd = tmrInterd.q();
  //
  if (!on && start) tmrFbNok.setTrig(true);
  if (on && start) tmrFbNok.setTrig(false);
  //fbNok = tmrFbNok.isOut();
  fbNok = tmrFbNok.q();
  //
  ready = (!trm && !extLock && !interd && (cmdRem <2) && (cmdLoc <2) );
  if (ready ){
    if (go) start = true;
    else start = false;
    if ((cmdRem == 1) || (cmdLoc == 1)) start = true;
  }
  else {
    start = false;
  }
}

void Equip::virtualDO(){
  if (!debug) {
    if (start) digitalWrite(pinOut, HIGH);
    else digitalWrite(pinOut, LOW);
  }  
  //
  if (start != lastStart) {
    lastStart = start;
  }
}

/**
  Sensor
*/
Sensor::Sensor(
  String _name, int _bitsADC, int _pinAI,
  float _y0, float _y1, String _um, bool _debug
){
  name = _name;
  bitsADC = _bitsADC;
  pinAI = _pinAI;
  y0 = _y0;
  y1 = _y1;
  um = _um;
  debug = _debug;
  analogReadResolution(bitsADC);
}
//
Sensor::Sensor(
  String _name, int _bitsADC, int _pinAI,
  float _y0, float _y1, String _um, bool _debug,
  float _setPoint, float _plusMinus 
){
  name = _name;
  bitsADC = _bitsADC;
  pinAI = _pinAI;
  y0 = _y0;
  y1 = _y1;
  um = _um;
  debug = _debug;
  setPoint = _setPoint;
  plusMinus = _plusMinus;
  analogReadResolution(bitsADC);
}
//
int Sensor::getReading(){
  if (!debug) reading = analogRead(pinAI);
  return reading;
}
void Sensor::setReading(int _reading){
  reading = _reading;
}
float Sensor::getValue(){
  if (bitsADC==12) x1= 4095;
  if (bitsADC==16) x1= 65536;
  float m = (y1-y0)/(x1-x0);
  float n =0 ;
  value = getReading()*m+n;
  if (setPoint > 0.0){
    highOp = setPoint + plusMinus;
    lowOp = setPoint - plusMinus;
    db = plusMinus / 10.0;
    highOpDB = highOp + db;
    lowOpDB = lowOp + db;
  }
  return value;
}
//
int Sensor::getStatus(){
  float val = getValue();
  if((val < highOpDB ) && (val > lowOpDB)) status = 1;
  if(val > highOp) status = 2;
  if(val < lowOp ) status = 3;
return status;
}  
//
String Sensor::getStrStatus(){
  int st = getStatus();
  if(st == 1) strStatus = "Ok";
  if(st == 2) strStatus = "HIGH";
  if(st == 3) strStatus = "LOW";
  return strStatus;
}  
//
void Sensor::setValue(float _value){ value = _value; }
//
String Sensor::toString(){
  String str = "name=" + name;
  str += ", bitsADC ="+String(bitsADC);
  str += ", pinAI ="+String(pinAI);
  str += ", y0 ="+String(y0);
  str += ", y1 ="+String(y1);
  str += ", reading ="+String(reading);
  str += ", value ="+String(getValue());
  str += ", um ="+um;
  str += ", debug ="+String(debug);
  if (setPoint > 0.0){
    str += ", setPoint ="+String(setPoint);
    str += ", +/- ="+String(plusMinus);
    str += ", highOp ="+String(highOp);
    str += ", lowOp ="+String(lowOp);
    str += ", status ="+getStrStatus();
  }  
  return str;
}


/**
  TwinSeq
*/

TwinSeq::TwinSeq(String _name, Equip _eq[], long _setIntTmr, bool _bothStart, bool _bothStop, bool _seqStart)  {
  name = _name;
  eq[0] = _eq[0];
  eq[1] = _eq[1];
  setIntTmr = _setIntTmr;
  bothStart = _bothStart;
  bothStop = _bothStop;
  seqStart = _seqStart;
}

void TwinSeq::setTimerInterd(long tmr){ tmrInterd.PT=tmr;}
void TwinSeq::setSeqStart(bool t){seqStart=t;}
void TwinSeq::setBothStart(bool t){bothStart=t;}
void TwinSeq::setBothStop(bool t){bothStop=t;}

void TwinSeq::seq() {
  eq[0].updateMotor();
  eq[1].updateMotor();
  //  
  if (seqStart){
    // test interd
    //if (tmrInterd.isOut()){
    if (tmrInterd.q()){
      tmrInterd.setTrig(false);
      wordSeq = -1;
      return;
    }
    bool eqAlmostOneOk = (eq[0].isOn() && !eq[0].isFBNok()) || (eq[1].isOn() && !eq[1].isFBNok());
    bool eqBothOn = eq[0].isOn() && eq[1].isOn();
    bool eqBothOff = !eq[0].isOn() && !eq[1].isOn();
    // bothStart
    if(bothStart && !eqBothOn){
      eq[0].setGo(true);
      eq[1].setGo(true);
      wordSeq = 3;
      return;
    } 
    // bothStop
    if(bothStop && !eqBothOff){
      eq[0].setGo(false);
      eq[1].setGo(false);
      wordSeq = 4;
      return;
    }   
    //
    if (!bothStart && eqBothOn) {
      if (pt1) eq[1].setGo(false);
      else eq[0].setGo(false);
      tmrInterd.setTrig(true);
      return;
    }
    // station OK 
    if (eqAlmostOneOk) {
      wordSeq = 1;
      return;
    }
    // start one equip
    if (!eqAlmostOneOk) {
      wordSeq = 2;
      if (pt1){
        pt1 = false;
        if(eq[0].isReady() && !eq[0].isInterd() && !eq[0].isFBNok()){
          eq[0].setGo(true);
          eq[1].setGo(false);            
          tmrInterd.setTrig(true);
          return;
        }    
      } 
      //
      else {
        pt1 = true;
        if(eq[1].isReady() && !eq[1].isInterd() && !eq[1].isFBNok()){
          eq[1].setGo(true);
          eq[0].setGo(false);          
          tmrInterd.setTrig(true);
          return;
        } 
      }  
    }
  }
  // !seqStart
  else {
    eq[0].setGo(false);
    eq[1].setGo(false); 
  } 
}

String TwinSeq::toString() { return "wordSeq="+String(wordSeq)+" pt1="+String(pt1)+" interd="+String(tmrInterd.q()); }


/**
  ParallelSeq
*/

ParallelSeq::ParallelSeq(
  String _name, 
  int _nrEq, 
  Equip _eq[],
  bool _allStart, 
  bool _allStop, 
  bool _more,
  bool _less,
  bool _start){
  name = _name;
  nrEq = _nrEq;
  nrEq_1 = nrEq-1;
  allStart = _allStart;
  allStop = _allStop;
  more = more;
  less = less;
  start = _start;
  //
  for (int i=0; i < nrEq; i++){
    eq[i] = _eq[i];
    ptOn[i] = false;
    ptOff[i] = false;
    ptOn[0] = true;
    ptOff[0] = true;
  }
}

void ParallelSeq::setAllStart(bool b) {allStart = b;}
void ParallelSeq::setAllStop(bool b) {allStop = b;}
void ParallelSeq::setMore(bool b) {more = b;}
void ParallelSeq::setLess(bool b) {less = b;}
void ParallelSeq::setStart(bool b) {start = b;}
void ParallelSeq::setTimerInterd1(long tmr){ setIntTmr1 =tmr;}
void ParallelSeq::setTimerInterd2(long tmr){ setIntTmr2 =tmr;}

void ParallelSeq::seq() {
  // update equipments
  for (int i=0; i < nrEq; i++){
    eq[i].updateMotor();
  }
  //  
  bool stationOk = (!allStart && !allStop && !more && !less);
  if (stationOk) wordSeq = 1;
  //  
  //if (tmrInterd.isOut()){
  if (tmrInterd.q()){
    tmrInterd.setTrig(false);
    wordSeq = -1;
  }  
  
  //if (!tmrInterd.isOut()){
  if (!tmrInterd.q()){
    //
    if (!allStop && !allStart) tmrInterd.PT=setIntTmr1;
    if(allStart || allStop) tmrInterd.PT=setIntTmr2;
    // 
    if ((more || allStart)  && start){
      wordSeq = 2;
      //
      for (int i=0; i < nrEq; i++){
        if(ptOn[i]){
          if(!eq[i].isOn() && eq[i].isReady() && !eq[i].isInterd() && !eq[i].isFBNok()){
            eq[i].setGo(true);
            ptOn[i]=false;
            if (i < nrEq_1) ptOn[i+1]=true;
            else ptOn[0]=true;
            tmrInterd.setTrig(true);
            return;
          }  
          else {
            ptOn[i]=false;
            if (i < nrEq_1) ptOn[i+1]=true;
            else ptOn[0]=true;
          }
        }
      }
    }
    if (less || allStop){
      wordSeq = 3;
      //
      for (int i=0; i < nrEq; i++){
        if(ptOff[i]){
          if(eq[i].isReady() && eq[i].isOn()){
            eq[i].setGo(false);
            ptOff[i]=false;
            if (i < nrEq_1) ptOff[i+1]=true;
            else ptOff[0]=true;
            tmrInterd.setTrig(true);
            return;
          }  
          else {
            ptOff[i]=false;
            if (i < nrEq_1) ptOff[i+1]=true;
            else ptOff[0]=true;
          }
        }
      }
    }
  }   
}

String ParallelSeq::toString() { 
  String str="Seq ";
  for (int i=0; i < nrEq; i++){
    str += eq[i].toString();
  }
  return "wordSeq="+String(wordSeq)+", Setinterd="+String(tmrInterd.PT)+"ms, interd="+String(tmrInterd.q()); 
}