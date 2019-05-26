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
  myMCP.Init(); // SDA = GPIO0 / SCL = GPIO2 
  myMCP.setIoDir(B11111111,A);
  myMCP.setIoDir(B11111111,B);
  myMCP.switchPort(B11111111, A); 
  myMCP.switchPort(B11111111, B); 
  delay(1000); 
  myMCP.switchAllPins(A, OFF);
  delay(1000);
  myMCP.switchAllPins(B, OFF); 
  delay(1000); 
  myMCP.setInterruptPinPol(HIGH);
  delay(10);
  myMCP.setInterruptOnDefValDevPort(B11111111, B, B00001111); // Pinnummer, Port, Status bei dem der Interrupt ausgelöst werden soll
  delay(2000);
  myMCP.setPortPullUp(B0001111, B);
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
    if((intFlagReg&intCapReg) == 0){
      Serial.println("LOW");
    }
    else{
      Serial.println("HIGH");
    }
    myMCP.switchPort(intFlagReg, A);
    delay(1000);
    event = false;
  }
}

void eventHappened(){
  event = true;
}
