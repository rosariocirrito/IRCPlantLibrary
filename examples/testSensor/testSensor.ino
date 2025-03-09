#include "IRCPlantLibrary.h"

// Sensor(String name,int bitsADC,int pinAI,float y0,float y1,String um,bool debug)
// Sensor(String name,int bitsADC,int pinAI,float y0,float y1,String um,bool debug, 
//        float setPoint, float plusMinus)
Sensor sens1 = Sensor("P1", 16, A7, 0.0, 6.0, "bar", true, 3.0, 0.5);

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
  // 65535 is the max value with 16 bits resolution set by analogReadResolution(16)
  // 4095 is the max value with 12 bits resolution set by analogReadResolution(12)
}

void loop() {    
  if (t1s.q()){
    //sens1.getValue();
    Serial.println(sens1.toString());
    // 
    float sensSin = sin(count*3.14/180.0);
    sens1.setReading(sensSin*65536);
    count++;     
    if (count > 90) count = 0;
  }
}
