/******************************************************

Example sketch for the MCP23017/MCP23S17/MCP23018/
MCP23S18 library.

The sketch shows the features for basic input / output operations
for the MCP23017.

Wiring can be found under the same name (png file) 

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

/* There are several ways to create your MCP23017 object:
 * MCP23017 myMCP = MCP23017(MCP_ADDRESS)            -> uses Wire / no reset pin (if not needed)
 * MCP23017 myMCP = MCP23017(MCP_ADDRESS, RESET_PIN)  -> uses Wire / RESET_PIN
 * MCP23017 myMCP = MCP23017(&wire2, MCP_ADDRESS)    -> uses the TwoWire object wire2 / no reset pin
 * MCP23017 myMCP = MCP23017(&wire2, MCP_ADDRESS, RESET_PIN) -> all together
 * Successfully tested with two I2C busses on an ESP32
 */
MCP23017 myMCP = MCP23017(MCP_ADDRESS, RESET_PIN);

int wT = 1000; // wT = waiting time

void setup(){ 
  Serial.begin(9600);
  Wire.begin();
  if(!myMCP.Init()){
    Serial.println("Not connected!");
    while(1){} 
  }
  myMCP.setPortMode(0b11111101, A);  // Port A: all pins are OUTPUT except pin 1
  myMCP.setPortMode(0b11111111, B);  // Port B: all pins are OUTPUT
  delay(wT);
  myMCP.setAllPins(A, ON); // alle LEDs switched on except A1
  delay(wT);
  myMCP.setPinX(1, A, OUTPUT, HIGH); // A1 switched on 
  delay(wT); 
  myMCP.setPort(0b11110000, B); // B4 - B7 switched on
  delay(wT);
  myMCP.setPort(0b01011110, A); // A0,A5,A7 switched off
  delay(wT);
  myMCP.setPinX(0,B,OUTPUT,HIGH); // B0 switched on
  delay(wT);
  myMCP.setPinX(4,B,OUTPUT,LOW); // B4 switched off
  delay(wT);
  myMCP.setAllPins(A, HIGH); // A0 - A7 all on
  delay(wT);
  myMCP.setPin(3, A, LOW); // A3 switched off
  delay(wT);
  myMCP.setPortX(0b11110000, 0b01101111,B); // at port B only B5,B6 are switched on
  delay(wT);
  myMCP.setPinMode(0,B,OUTPUT); // B0 --> OUTPUT
  for(int i=0; i<5; i++){  // B0 blinking
    myMCP.togglePin(0,B); 
    delay(200);
    myMCP.togglePin(0,B);
    delay(200);
  }
  for(int i=0; i<5; i++){ // B7 blinking
    myMCP.togglePin(7,B);
    delay(200);
    myMCP.togglePin(7,B);
    delay(200);
  }
}

void loop(){ 
} 
