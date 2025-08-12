/*
   Copyright (c) 2025 Rosario Cirrito All rights reserved
 */

#include <LoRa.h>
#include "IRCPlantLibrary.h"

# define APPLNAME "VascaMotori_xxx"

/*
  * LoRa parameters
*/
//
byte localAddress = 0x01;     // address of this device
byte destination = 0xFF;      // destination to send to 0xFF = broadcast

byte destVasca = 0x01;        // destination to send to Livello Vasca
byte destPozzoBagari = 0x02;  // destination to send to Pozzo Bagari
byte destPozzo3Fiumi = 0x03;  // destination to send to Pozzo TreFiumi
byte destHouse = 0x04;        // destination to send to House Gateway
//
byte msgCount = 0;            // count of outgoing messages
ulong intervalSend = 120000L;    // interval between sends for HB
ulong intervalReceive = 180000L; // interval between receive for watchdog
//
boolean motorON = false;
byte goByteCmd = 0;
byte goByteCmdLast = 0;

/* global variables for Monitor */
boolean toDisplayBySend = false;
boolean toDisplayByReceive = false;

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

WatchDog wdReceive = WatchDog("WDReceive", intervalReceive);
WatchDog wdSend = WatchDog("WDSend", intervalSend);
unsigned long setT= 2000L;
unsigned long setTInterd= 15000L;
// TimerSE(String name, bool trig, long timeSet);
bool trigDI= false;
bool fInterd= false;
TimerSE tse = TimerSE("T_SE",trigDI,setT);
TimerSE tInterd = TimerSE("T_SE",fInterd,setTInterd);

//T_Task t5s = T_Task(5000);

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200UL);
  //
  pinMode(I0_4, INPUT);   // LevelSwitch signal
  //
  pinMode(Q0_0, OUTPUT);  // Well pump on feedback
  pinMode(Q0_1, OUTPUT);  // WatchDog receive signal
  pinMode(R0_0, OUTPUT);  // Well pump start command
  //
  wdReceive.setEnabled(true);
  wdSend.setEnabled(true);
  //
  // Begin LoRa hardware
  if (!LoRa.begin()) {
    Serial.println("LoRa begin error: is LoRa module connected?");
    while (true);
  }

  // Default LoRa values after begin for EU ESP32-based PLCs:
  // Frequency: 868.0MHz
  // Modulation LoRa 4/5, BW 125, SF12, Preamble Length 8
  // TX power: +8dBm
  // For America ESP32-based PLCs frequency is 915.0MHz
  // For Asia ESP32-based PLCs frequency is 433.05MHz

  Serial.println("LoRa init succeeded");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // 1. receive
  doReceive();
  
  // 1. read tank level DI and update goByteCmd
  trigDI = digitalRead(I0_4);
  tse.setTrig(trigDI);
  if (tse.q()) goByteCmd = 0x01; 
  else goByteCmd = 0x00;
  //
  tInterd.setTrig(fInterd);
  if (tInterd.q()) fInterd=false;
  // r0 updating
    if (goByteCmd == 0) digitalWrite(R0_0, LOW);
    else if (goByteCmd == 1) digitalWrite(R0_0, HIGH);

  // 2. Send by COS 
  if (goByteCmd != goByteCmdLast){
    doSend("by_COS");
    goByteCmdLast = goByteCmd;
  }
  
  // 3. HB WatchDog on Send
  if (wdSend.q()) doSend("by_HB");

  // 7. WatchDog on receive
  digitalWrite(Q0_1, wdReceive.q());

  // 4. Q0_0 On Feedback
  if (motorON) digitalWrite(Q0_0, HIGH);
  else digitalWrite(Q0_0, LOW);

  doDisplay();

  //String debug = "tInterd.ct:"+String(tInterd.ct()+"fInterd:"+String(fInterd));
  //debug += " trigDI:"+String(trigDI)+" tse.ct:"+String(tse.ct())+" tse.q:"+String(tse.q());
  //if(t5s.q()) Serial.println(debug);
}


/*  doSend method  */
void doSend(String _why) {
  if(!fInterd){
    myTankData.myTankDataStruct.sender=localAddress;
    myTankData.myTankDataStruct.command=goByteCmd;
    myTankData.myTankDataStruct.count=msgCount;
    // Send 
    LoRa.send(myTankData.tx, tankUnionSize);
    //
    Serial.println("doSendMessage() -> Tank Sending tx..." + _why);
    //
    // Wait packet to be sent
    LoRa.waitPacketSent();
    //
    Serial.println("doSendMessage() -> Tank tx sent!"+ _why);
    msgCount++;                           // increment message counter
    wdSend.resetWD();              // 
    toDisplayBySend = true;
    fInterd=true;
  }
}

/*  metodo doReceive       */
void doReceive()
{
  if (LoRa.available())             // can't use readString() in callback
  {
    uint8_t len = (uint8_t) pumpUnionSize;
    if (LoRa.recv((uint8_t *) &myPumpData.tx, &len)) {
      toDisplayByReceive = true;
      Serial.println("rssi: "+String(LoRa.lastRssi()));
    }
    wdReceive.resetWD();
  }
}

/*  metodo doDisplay()       */ 
 void doDisplay(){
  if (toDisplayBySend || toDisplayByReceive){
    if (toDisplayBySend) Serial.println("toDisplayBySend");
    if (toDisplayByReceive) Serial.println("toDisplayByReceive");
    String display2 = "TankCtrlApp payload => local address:" +String(localAddress);
    display2 +="/ goByteCmd: " + String(goByteCmd);
    display2 +="/ count: " +String(myTankData.myTankDataStruct.count);
    Serial.println(display2);
    String display = "WellPumpApp payload => sender: " + String(myPumpData.myPumpDataStruct.sender);
    display += "/ byteStatus: "+ String(myPumpData.myPumpDataStruct.status);
    display += "/ msgCount: "+ String(myPumpData.myPumpDataStruct.count);
    Serial.println(display);
    
    Serial.println("millis()=" + String(millis()));
    Serial.println();
    //
    toDisplayByReceive = false;
    toDisplayBySend = false;  
  }
}