/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for interrupts caused by pin polarity change. 

Wiring can be found under the same name (png file) 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/
#include <Wire.h>
#include <MCP23017.h>
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)
#define RESET_PIN 5  
int interruptPin = 3;
volatile bool event; 
byte intCapReg; 

/* There are several ways to create your MCP23017 object:
 * MCP23017 myMCP = MCP23017(MCP_ADDRESS)            -> uses Wire / no reset pin (if not needed)
 * MCP23017 myMCP = MCP23017(MCP_ADDRESS, RESET_PIN)  -> uses Wire / RESET_PIN
 * MCP23017 myMCP = MCP23017(&wire2, MCP_ADDRESS)    -> uses the TwoWire object wire2 / no reset pin
 * MCP23017 myMCP = MCP23017(&wire2, MCP_ADDRESS, RESET_PIN) -> all together
 * Successfully tested with two I2C busses on an ESP32
 */
MCP23017 myMCP = MCP23017(MCP_ADDRESS, RESET_PIN);

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
  myMCP.setInterruptPinPol(HIGH); // set INTA and INTB active-high
  delay(10);
  myMCP.setInterruptOnChangePort(B11111111, B); //set all B pins as interrrupt Pins
  event=false;
}  

void loop(){ 
  intCapReg = myMCP.getIntCap(B); // ensures that existing interrupts are cleared
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
    if((intFlagReg&intCapReg) == 0){  //LOW-HIGH or HIGH-LOW interrupt?
      Serial.println("LOW");
    }
    else{
      Serial.println("HIGH");
    }
    myMCP.setPort(intFlagReg, A);
    //delay(1000);
    event = false; 
  }
}

void eventHappened(){
  event = true;
}
