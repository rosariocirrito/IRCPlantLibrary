/*
   Copyright (c) 2025 Rosario Cirrito All rights reserved

   
   
 */

#include <LoRa.h>
#include "IRCPlantLibrary.h"

# define APPLNAME "WellPump_xxx"

//
byte localAddress = 0x03;     // address of this device
byte destination = 0xFF;      // destination to send to 0xFF = broadcast

byte destVasca = 0x01;        // destination to send to Livello Vasca
byte destPozzoBagari = 0x02;  // destination to send to Pozzo Bagari
byte destPozzo3Fiumi = 0x03;  // destination to send to Pozzo TreFiumi
byte destHouse = 0x04;        // destination to send to House Gateway
//
String outgoing;              // outgoing message
byte msgCount = 0;            // count of outgoing messages
ulong lastSendTime = 0;        // last send time
ulong intervalSend = 120000L;    // interval between sends for HB
ulong lastReceiveTime = 0;     // last receive time
ulong intervalReceive = 180000L; // interval between receive for watchdog

byte goByteCmd = 0;
byte goByteCmdLast = 0;
byte byteStatus = 0;
byte byteStatusLast = 0;

/* global variables for Monitor */
boolean toDisplayBySend = false;
boolean toDisplayByReceive = false;
boolean motorON = false;

// debug
boolean verbose = true;

// dataStruct & union for the tank
typedef struct tankDataStruct {
  byte sender;
  byte command;
  byte count;
};
const int tankUnionSize = sizeof(tankDataStruct);
typedef union tankDataUnion {
  tankDataStruct myTankDataStruct;
  uint8_t tx[tankUnionSize];
};
tankDataUnion myTankData;

// dataStruct & union for the pump
typedef struct pumpDataStruct {
  byte sender;
  byte status;
  byte count;
};
const int pumpUnionSize = sizeof(pumpDataStruct);
typedef union pumpDataUnion {
  pumpDataStruct myPumpDataStruct;
  uint8_t tx[tankUnionSize];
};
pumpDataUnion myPumpData;

//

// Equip eq1 = Equip("Eq1",cmdRem, cmdLoc, extLock, pinOn, pinTrm, go, setIntTmr, setFBTmr, nr, hh, hhReset, pinOut, debug);
Equip myPump = Equip("WellPump",0, 0, false, I0_4, I0_5, false, 2000L, 10000L, 0, 0, false, Q0_0, false);
WatchDog wdReceive = WatchDog("WDReceive", intervalReceive);
WatchDog wdSend = WatchDog("WDSend", intervalSend);
unsigned long setTInterd= 15000L;
// TimerSE(String name, bool trig, long timeSet);
bool fInterd= false;
TimerSE tInterd = TimerSE("T_SE",fInterd,setTInterd);

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200L);
  //
  pinMode(I0_4, INPUT);   // Well pump on feedback
  //
  pinMode(Q0_1, OUTPUT);  // WatchDog receive signal
  pinMode(Q0_0, OUTPUT);  // Well pump start command
  //
  wdReceive.setEnabled(true);
  wdSend.setEnabled(true);

  // Begin LoRa hardware
  if (!LoRa.begin()) {
    Serial.println("LoRa begin error: is LoRa module connected?");
    while (true);
  }

  // Default LoRa values after begin for EU ESP32-based PLCs:
  // Frequency: 868.0MHz
  // Modulation LoRa 4/5, BW 125, SF12, Preamble Length 8
  // TX power: +8dBm
  //
  Serial.println("LoRa init succeeded");
  delay (5000);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // 1. receive
  doReceive();
  //
  tInterd.setTrig(fInterd);
  if (tInterd.q()) fInterd=false;
  // 2. update the go command to Equip
  if(myTankData.myTankDataStruct.sender == 0x01){
    goByteCmd= myTankData.myTankDataStruct.command;
    // 3. EquipCtrl
    if (goByteCmd == 0) myPump.setGo(false);
    else if (goByteCmd == 1) myPump.setGo(true);
    //
    myPump.updateMotor();
    //
    motorON = myPump.isOn();
    byteStatus = myPump.getStatusByte();
    
    // 4. force Q0_0 output
    digitalWrite(Q0_0, myPump.isStart());
  
    //
    // 5. Send by COS 
    if (byteStatus != byteStatusLast){
      String why = "by_COS";
      doSend(why);
      byteStatusLast = byteStatus;
    }

    // 6. HB WatchDog on Send
    if (wdSend.q()) doSend("by_HB");

    // 7. WatchDog on receive
    digitalWrite(Q0_1, wdReceive.q());
  }

  // 8. Serial Monitor
  doDisplay();

  
}

/*  metodo doSend()       */
void doSend(String _why){
  if(!fInterd){
    myPumpData.myPumpDataStruct.sender=localAddress;
    myPumpData.myPumpDataStruct.status=byteStatus;
    myPumpData.myPumpDataStruct.count=msgCount;
    // Send 
    LoRa.send(myPumpData.tx, pumpUnionSize);
    //
    Serial.println("doSend() -> WellPump Sending tx..." + _why);
    //
    // Wait packet to be sent
    LoRa.waitPacketSent();
    //
    Serial.println("doSend() -> WellPump tx sent!" + _why);
    msgCount++;                           // increment message counter 
    wdSend.resetWD();          // 
    toDisplayBySend = true;
    fInterd=true;
  }
}

/*  metodo doReceive()       */
void doReceive(){
// Wait packets from Tank
  if (LoRa.available()) {
    uint8_t len = (uint8_t) tankUnionSize;
    if (LoRa.recv((uint8_t *) &myTankData.tx, &len)) {      
      toDisplayByReceive = true;
    }
    wdReceive.resetWD();
    //Serial.println("doReceive() -> lastReceiveTime=" + String(millis()));
  }
}

/*  metodo doDisplay()       */ 
 void doDisplay(){
  if (toDisplayBySend || toDisplayByReceive){
    if (toDisplayBySend) Serial.println("toDisplayBySend");
    if (toDisplayByReceive) Serial.println("toDisplayByReceive");
    String display = "WellPumpApp payload => localAddress: " +String(localAddress);
    display += "/ byteStatus: "+ String(byteStatus);
    display += "/ msgCount: "+ String(msgCount);
    Serial.println(display);
    String display2 = "TankCtrlApp payload => sender:" + String(myTankData.myTankDataStruct.sender);
    display2 +="/ goByteCmd: " + String(goByteCmd);
    display2 +="/ count: " +String(myTankData.myTankDataStruct.count);
    Serial.println(display2);
    Serial.println("millis()=" + String(millis()));
    Serial.println();
    //
    toDisplayByReceive = false;
    toDisplayBySend = false;  
  }
}
