/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for interrupts caused by deviation from
the DEFVAL register. 

The wiring for this example can be found under the same name (png file). 

written by Wolfgang (Wolle) Ewald

https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/

#include <Wire.h>
#include <MCP23017_WE.h>
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)
/* A hardware reset is performed during init(). If you want to save a pin you can define a dummy 
 * reset pin >= 99 and connect the reset pin to HIGH. This will trigger a software reset instead 
 * of a hardware reset. 
 */
#define RESET_PIN 5 
int interruptPin = 3;
volatile bool event = false;
uint8_t defValB = 0b00001111; // if port B deviates, an interrupt is triggered

typedef MCP23017_WE MCP; // just for less typing!

/* There are several ways to create your MCP23017 object:
 * MCP23017_WE myMCP = MCP23017_WE(MCP_ADDRESS) -> uses Wire / no reset pin 
 * MCP23017_WE myMCP = MCP23017_WE(MCP_ADDRESS, RESET_PIN) -> uses Wire / RESET_PIN
 * MCP23017_WE myMCP = MCP23017_WE(&Wire, MCP_ADDRESS) -> passing a TwoWire object / no reset pin
 * MCP23017_WE myMCP = MCP23017_WE(&Wire, MCP_ADDRESS, RESET_PIN) -> "all together"
 */
MCP myMCP = MCP(MCP_ADDRESS, RESET_PIN); // short version
// MCP23017_WE myMCP = MCP23017_WE(MCP_ADDRESS, RESET_PIN); // long version

void setup(){ 
  pinMode(interruptPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin), eventHappened, RISING);
  Serial.begin(9600);
  Wire.begin();
  if(!myMCP.Init()){
    Serial.println("Not connected!");
    while(1){} 
  }
  myMCP.setPortMode(0b11111111, MCP::A);
  myMCP.setPort(0b11111111, MCP::A); // just an LED test 
  delay(1000); 
  myMCP.setAllPins(MCP::A, LOW);
  delay(1000);
  myMCP.setInterruptPinPol(HIGH);
  delay(10);
  myMCP.setInterruptOnDefValDevPort(0b11111111, MCP::B, defValB); // interrupt pins, port, DEFVALB
  myMCP.setPortPullUp(0b00001111, MCP::B); // pull-up for B0-B3
  delay(10);
  myMCP.getIntCap(MCP::B); // deletes all interrupts
  event = false;
}  

void loop(){ 
  if(event){
    byte intFlagReg = myMCP.getIntFlag(MCP::B);
    byte eventPin = log(intFlagReg)/log(2);
    byte intCapReg = myMCP.getIntCap(MCP::B);
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
    myMCP.setPort(intFlagReg, MCP::A);
    delay(400);
    /* if you use a pushbutton to trigger the interrupt, uncomment the 
       following line. */
    // while(myMCP.getPort(MCP::B) != defValB) {} // while key is pressed
    intCapReg = myMCP.getIntCap(MCP::B);
    event = false;
  }
}

void eventHappened(){
  event = true;
}
