/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for interrupts caused by deviation from
the DEFVAL register. 

The wiring for this example can be found under the same name (png file). 

written by Wolfgang (Wolle) Ewald

https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/

#include <Wire.h>
#include <MCP23017.h>
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)
/* A hardware reset is performed during init(). If you want to save a pin you can define a dummy 
 * reset pin >= 99 and connect the reset pin to HIGH. This will trigger a software reset instead 
 * of a hardware reset. 
 */
#define RESET_PIN 5 
int interruptPin = 3;
volatile bool event = false;

/* There are several ways to create your MCP23017 object:
 * MCP23017 myMCP = MCP23017(MCP_ADDRESS) -> uses Wire / no reset pin 
 * MCP23017 myMCP = MCP23017(MCP_ADDRESS, RESET_PIN) -> uses Wire / RESET_PIN
 * MCP23017 myMCP = MCP23017(&Wire, MCP_ADDRESS) -> passing a TwoWire object / no reset pin
 * MCP23017 myMCP = MCP23017(&Wire, MCP_ADDRESS, RESET_PIN) -> "all together"
 */
MCP23017 myMCP = MCP23017(MCP_ADDRESS, RESET_PIN);

void setup(){ 
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), eventHappened, RISING);
  Serial.begin(9600);
  Wire.begin();
  if(!myMCP.Init()){
    Serial.println("Not connected!");
    while(1){} 
  }
  myMCP.setPortMode(0b11111111,A);
  myMCP.setPort(0b11111111, A); // just an LED test 
  delay(1000); 
  myMCP.setAllPins(A, LOW);
  delay(1000);
  myMCP.setInterruptPinPol(HIGH);
  delay(10);
  myMCP.setInterruptOnDefValDevPort(0b11111111, B, 0b00001111); // interrupt pins, port, DEFVALB
  myMCP.setPortPullUp(0b00001111, B); // pull-up for B0-B3
  delay(10);
  myMCP.getIntCap(B); // deletes all interrupts
  event=false;
}  

void loop(){ 
  //myMCP.getIntCap(B); // bouncing can lead to issues - uncommenting this line can help 
  if(event){
    byte intFlagReg = myMCP.getIntFlag(B);
    byte eventPin = log(intFlagReg)/log(2);
    byte intCapReg = myMCP.getIntCap(B);
    Serial.println("Interrupt!");
    Serial.print("Interrupt Flag Register: ");
    Serial.println(intFlagReg, BIN); 
    Serial.print("Interrupt Capture Register: ");
    Serial.println(intCapReg, BIN); 
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
    delay(400);
    intCapReg = myMCP.getIntCap(B);
    event = false;
  }
}

void eventHappened(){
  event = true;
}
