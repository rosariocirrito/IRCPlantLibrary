#ifndef IRCPlantLibrary_h
#define IRCPlantLibrary_h

#include <Arduino.h>

/**
TimerSE class --> On delay
*/
class TimerSE {
  //
  private:
  String name;
  bool oldTrig, trig;
  unsigned long startTime;
  //
  public:
  unsigned long PT;
  // 
  // public methods
  TimerSE(String _name, bool _trig, unsigned long _PT);
  void setTrig(bool up);
  bool q();
  unsigned long ct();
  String toString();
};

/**
TimerSV class --> pulse
*/
class TimerSV {
  //
  private:
  String name;
  bool oldTrig, trig, out;
  unsigned long startTime;
  //
  public:
  unsigned long PT;

  // public methods
  TimerSV(String _name, bool _trig, unsigned long _PT);
  void setTrig(bool trig);
  bool q();
  unsigned long ct();
  String toString();
};

/**
TNs one pulse every N s
*/
class TNs {
  private:
  //
  unsigned int s;
  unsigned long startTime;
  //
  public:
  // public methods
  TNs(unsigned long s);
  bool q();
  String toString();
};

/**
T01s one pulse every 01 s
*/
class T01s {
  private:
  //
  unsigned long startTime;
  //
  public:
  // public methods
  T01s();
  bool q();
  String toString();
};

/**
T01min one pulse every 01 min
*/
class T01min {
  private:
  unsigned long startTime;
  public:
  
  // public methods
  T01min();
  bool q();
  String toString();
};

/**
T15min one pulse every 15 min
*/
class T15min {
  private:
  unsigned long startTime;
  public:
  
  // public methods
  T15min();
  bool q();
  String toString();
};

/**
  T_Task job to be executed at prefixed time intervals
*/
class T_Task {
  private:
  //
  unsigned long startTime;
  unsigned long PT;
  public:
  
  // public methods
  T_Task(unsigned long _PT);
  bool q();
  String toString();
};


/**
WatchDog
*/
class WatchDog {
  private:
  String name;
  bool enabled, reset;
  TimerSE twd = TimerSE("twd", false, 0);
  //
  public:  
  unsigned long timeWD;

  // public methods
  WatchDog(String name, unsigned long _PT);
  void setEnabled(bool up);
  void resetWD();
  //
  bool isEnabled();
  bool q();
  unsigned long getElapsedTime();
  String toString();
};


/**
  FlipFlop
*/
class FlipFlop {
  private:
  String name;
  bool oldTrig, trig;
  bool oldOut, out;
  //
  TimerSV tmrFlip = TimerSV("tmrFlip",false, 1000);
  TimerSE tmrFlop = TimerSE("tmrFlop",false, 1000);

  public:
    // costruttore
  FlipFlop(String name, bool trig, unsigned long timeSetFlip, unsigned long timeSetFlop);
  //
  void setTrig(bool trig);
  void setTimeFlip(unsigned long timeSetFlip);
  void setTimeFlop(unsigned long timeSetFlop);
  bool q();
  String toString();
};

/**
  Ctrl3P
*/
class Ctrl3P {
  private:
  String name;
  bool trig, trigPlus, trigMinus;
  unsigned long timeSetFlipPlus, timeSetFlopPlus, timeSetFlipMinus, timeSetFlopMinus;
  FlipFlop ffPlus = FlipFlop("ffPlus", false, 500, 1000);
  FlipFlop ffMinus = FlipFlop("ffMinus", false, 500, 1000);
  public:
  Ctrl3P(String name, bool trig,
    unsigned long timeSetFlipPlus, unsigned long timeSetFlopPlus, 
    unsigned long timeSetFlipMinus, unsigned long timeSetFlopMinus
  );
  void setTrig(bool trig);
  void setTrigPlus(bool trig);
  void setTrigMinus(bool trig);
  void setTimeFlipPlus(unsigned long time);
  void setTimeFlopPlus(unsigned long time);
  void setTimeFlipMinus(unsigned long time);
  void setTimeFlopMinus(unsigned long time);
  bool qPlus();
  bool qMinus();
  String toString();
};

/**
  Equip --> Equipment monitoring and control
*/
class Equip {
  private:
  String name="???";
  int stato;
  String status;
  int cmdRem, cmdLoc;
  bool extLock, oldOn, trm, start;
  int pinOn, pinTrm, pinOut;
  bool fScan, lastStart, debug, hhReset; 
  bool trigTmrInterd = false;
  bool trigTmrFbNok = false;
  unsigned long setIntTmr = 10000;
  unsigned long setFBTmr = 5000;
  int nr, hh; 

  // private methods
  void virtualDI();
  void startEquip();
  void virtualDO();

  // internal objects
  TimerSV tmrInterd = TimerSV("tmrInt",trigTmrInterd, setIntTmr);
  TimerSE tmrFbNok = TimerSE("tmrFB",trigTmrFbNok, setFBTmr);
  
  public:
  bool go, on, interd, fbNok, ready;
  //
  String toString();
  String getName();

   // constructor
  Equip(
    String name,
    int cmdRem, 
    int cmdLoc,  
    bool extLock,
    int pinOn,
    int pinTrm,
    bool go,
    unsigned long setIntTmr,
    unsigned long setFBTmr,
    int nr,
    int hh,
    bool hhReset,
    int pinOut,
    bool debug
  );

  //
  void updateMotor();
  //
  String getStatus();
  bool isReady();
  bool isInterd();
  bool isFBNok();
  bool isOn();
  bool isGo();
  bool isStart();
  //
  void setGo(bool go);
  void setCmdRem(int cmdRem);
  void setCmdLoc(int cmdLoc);
};

/**
  Sensor
*/
class Sensor {
  private:
  String name = "noName";
  int bitsADC = 16;
  int pinAI = 0;
  int x0=0;
  int x1=65536; // 12 , 4096
  float y0 = 0.0;
  float y1 = 100.0;
  int reading = 0;
  float value = 0.0;
  String um = "um";
  bool debug = false;
  float setPoint = 0.0;
  float plusMinus = 0.01;
  float highOp = 0.1;
  float lowOp = 0.5;
  float db = plusMinus / 10.0;
  float highOpDB = highOp - db;
  float lowOpDB = lowOp + db;
  int status = 0;
  String strStatus = "???";

  public:
  Sensor(
    String name,
    int bitsADC,
    int pinAI,
    float y0,
    float y1,
    String um,
    bool debug
  );
  Sensor(
    String name,
    int bitsADC,
    int pinAI,
    float y0,
    float y1,
    String um,
    bool debug,
    float setPoint,
    float plusMinus 
  );
  int getReading();
  void setReading(int reading);
  float getValue();
  void setValue(float value);
  int getStatus();
  String getStrStatus();
  String toString();
};

/**
  TwinSeq
*/
class TwinSeq {
  private:
  String name;
  unsigned long setIntTmr = 10000;
  TimerSV tmrInterd = TimerSV("tmrIntSeq",false, setIntTmr);
  bool pt1 = true;
  bool bothStart, bothStop, seqStart; 
  int wordSeq=0;
  

  public:
  Equip eq[2] = {
    Equip("Eq1",0, 0, false, 1, 2, false, 2000, 5000, 0, 0, false, 1, false),
    Equip("Eq2",0, 0, false, 3, 4, false, 2000, 5000, 0, 0, false, 2, false)
  };
  TwinSeq(String name, Equip eq[], long setIntTmr, bool bothStart, bool bothStop, bool start);
  void seq();
  void setTimerInterd(long tmr);
  void setSeqStart(bool t);
  void setBothStart(bool t);
  void setBothStop(bool t);
  String toString();
};

/**
  ParallelSeq
*/
class ParallelSeq {
  private:
  String name;
  unsigned long setIntTmr1 = 5000;
  unsigned long setIntTmr2 = 1000;
  TimerSV tmrInterd = TimerSV("tmrIntSeq",false, setIntTmr1);
  
  // parametri ingresso
  public:
  bool allStart, allStop, more, less, start; 
  int wordSeq=0;
  int nrEq = 4;
  int nrEq_1 = nrEq-1;
  bool ptOn[4], ptOff[4];
  Equip eq[4] = {
    Equip("Eq1",0, 0, false, 1, 2, false, 2000, 5000, 0, 0, false, 1, false),
    Equip("Eq2",0, 0, false, 3, 4, false, 2000, 5000, 0, 0, false, 2, false),
    Equip("Eq3",0, 0, false, 5, 6, false, 2000, 5000, 0, 0, false, 3, false),
    Equip("Eq4",0, 0, false, 7, 8, false, 2000, 5000, 0, 0, false, 4, false)
  };
  ParallelSeq(String name, int nrEq, Equip eq[],bool allStart, bool allStop, bool more, bool less, bool start);
  //
  void seq();
  void setTimerInterd1(long tmr1);
  void setTimerInterd2(long tmr1);
  String toString();
  void setAllStart(bool b);
  void setAllStop(bool b);
  void setMore(bool b);
  void setLess(bool b);
  void setStart(bool b);
};

#endif