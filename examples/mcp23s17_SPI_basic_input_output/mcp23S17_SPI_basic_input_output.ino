/******************************************************

Example sketch for the MCP23017/MCP23S17 library

The sketch shows how to use the MCP23S17. The MCP23S17 is working
with SPI instead of I2C. The class is still called MCP23017. All 
functions are the identical, except the constructors. 

Although the MCP23S17 is SPI based you still have to select an 
address!

Wiring can be found under the same name (png file) 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/

#include <SPI.h>
#include <MCP23017.h>
#define CS_PIN 5   // Chip Select Pin
#define RESET_PIN 7 
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)

/* There are ways to create your MCP23017 (MCP23S17) object:
 * MCP23017 myMCP = MCP23017(CS_PIN, RESET_PIN, MCP_ADDRESS);
 * MCP23017 myMCP = MCP23017(&SPI, CS_PIN, RESET_PIN, MCP_ADDRESS);
 * The second option enables you to create your own SPI objects,
 * e.g. in order to use two SPI interfaces on the ESP32.
 */
 
MCP23017 myMCP = MCP23017(&SPI, CS_PIN, RESET_PIN, MCP_ADDRESS);

int wT = 1000; // wT = waiting time

void setup(){ 
  SPI.begin();
  Serial.begin(9600); // wieder entfernen!
  myMCP.Init();  
  myMCP.setPortMode(B11111101, A);  // Port A: all pins are OUTPUT except pin 1
  myMCP.setPortMode(B11111111, B);  // Port B: all pins are OUTPUT
  delay(wT);
  myMCP.setAllPins(A, ON); // alle LEDs switched on except A1
  delay(wT);
  myMCP.setPinX(1, A, OUTPUT, HIGH); // A1 switched on 
  delay(wT); 
  myMCP.setPort(B11110000, B); // B4 - B7 switched on
  delay(wT);
  myMCP.setPort(B01011110, A); // A0,A5,A7 switched off
  delay(wT);
  myMCP.setPinX(0,B,OUTPUT,HIGH); // B0 switched on
  delay(wT);
  myMCP.setPinX(4,B,OUTPUT,LOW); // B4 switched off
  delay(wT);
  myMCP.setAllPins(A, HIGH); // A0 - A7 all on
  delay(wT);
  myMCP.setPin(3, A, LOW); // A3 switched off
  delay(wT);
  myMCP.setPortX(B11110000, B01101111,B); // at port B only B5,B6 are switched on
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
