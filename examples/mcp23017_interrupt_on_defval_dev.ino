/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for interrupts caused by deviation from
the DEFVAL register. 

Wiring can be found under the same name (png file) 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de

*******************************************************/

#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW) 
#include <Wire.h>
#include <MCP23017.h> 
int interruptPin = 3;
volatile bool event = false;
byte intCapReg; 

MCP23017 myMCP(MCP_ADDRESS,5); // 5 = ResetPin

void setup(){ 
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), eventHappened, RISING);
  Serial.begin(9600);
  Wire.begin();
  myMCP.Init();
  myMCP.setPortMode(B11111111,A);
  myMCP.setPort(B11111111, A); // just an LED test 
  delay(1000); 
  myMCP.setAllPins(A, OFF);
  delay(1000);
  myMCP.setInterruptPinPol(HIGH);
  delay(10);
  myMCP.setInterruptOnDefValDevPort(B11111111, B, B00001111); // interrupt pins, port, DEFVALB
  myMCP.setPortPullUp(B00001111, B); // pull-up for B0-B3
  event=false;
}  

void loop(){ 
  intCapReg = myMCP.getIntCap(B);
  if(event){
    delay(200);
    byte intFlagReg, eventPin; 
    intFlagReg = myMCP.getIntFlag(B);
    eventPin = log(intFlagReg)/log(2);
    intCapReg = myMCP.getIntCap(B);
    Serial.println("Interrupt!");
    Serial.print("Interrupt Flag Register: ");
    Serial.println(intFlagReg,BIN); 
    Serial.print("Interrupt Capture Register: ");
    Serial.println(intCapReg,BIN); 
    Serial.print("Pin No.");
    Serial.print(eventPin);
    Serial.print(" went ");
    if((intFlagReg&intCapReg) == 0){ // HIGH/LOW or LOW/HIGH change?
      Serial.println("LOW");
    }
    else{
      Serial.println("HIGH");
    }
    myMCP.setPort(intFlagReg, A);
    delay(1000);
    event = false;
  }
}

void eventHappened(){
  event = true;
}
