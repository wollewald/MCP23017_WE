/******************************************************

Example sketch for the MCP23017/MCP23S17/MCP23018/
MCP23S18 library.

This sketch shows how to use the MCP23017 with the TinyWireM library:
https://github.com/adafruit/TinyWireM
This allows you to run the MCP23017_WE library on an ATtiny85, for example.

The wiring for this example can be found under the same name (png file). 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/

/* !!!! IN ORDER TO USE TinyWireM YOU NEED TO UNCOMMENT #define USE_TINY_WIRE_M_     !!!!
 * !!!! IN mcp23017_WE_config.h WHICH YOU FIND IN THE MCP23017_WE/src FOLDER            !!!!                                                       !!!!
 */

#include <TinyWireM.h>
#include <MCP23017_WE.h>
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)
/* A hardware reset is performed during init(). If you want to save a pin you can define a dummy 
 * reset pin >= 99 and connect the reset pin to HIGH. This will trigger a software reset instead 
 * of a hardware reset. 
 */
#define RESET_PIN 4 

typedef MCP23017_WE MCP; // just for less typing!

/* There are two ways to create your MCP23017 object:
 * MCP23017_WE myMCP = MCP23017_WE(MCP_ADDRESS)            -> uses Wire / no reset pin (if not needed)
 * MCP23017_WE myMCP = MCP23017_WE(MCP_ADDRESS, RESET_PIN)  -> uses Wire / RESET_PIN
 */

MCP myMCP = MCP(MCP_ADDRESS, RESET_PIN);

int wT = 1000; // wT = waiting time

void setup(){ 
  TinyWireM.begin();
  myMCP.Init();
  myMCP.setPortMode(0b11111101, MCP::A);  // Port A: all pins are OUTPUT except pin 1
  myMCP.setPortMode(0b11111111, MCP::B);  // Port B: all pins are OUTPUT
  delay(wT);
  myMCP.setAllPins(MCP::A, HIGH); // alle LEDs switched on except A1
  delay(wT);
  myMCP.setPinX(1, MCP::A, OUTPUT, HIGH); // A1 switched on 
  delay(wT); 
  myMCP.setPort(0b11110000, MCP::B); // B4 - B7 switched on
  delay(wT);
  myMCP.setPort(0b01011110, MCP::A); // A0,A5,A7 switched off
  delay(wT);
  myMCP.setPinX(0, MCP::B, OUTPUT, HIGH); // B0 switched on
  delay(wT);
  myMCP.setPinX(4, MCP::B, OUTPUT, LOW); // B4 switched off
  delay(wT);
  myMCP.setAllPins(MCP::A, HIGH); // A0 - A7 all on
  delay(wT);
  myMCP.setPin(3, MCP::A, LOW); // A3 switched off
  delay(wT);
  myMCP.setPortX(0b11110000, 0b01101111, MCP::B); // at port B only B5,B6 are switched on
  delay(wT);
  myMCP.setPinMode(0, MCP::B, OUTPUT); // B0 --> OUTPUT
  for(int i=0; i<5; i++){  // B0 blinking
    myMCP.togglePin(0, MCP::B); 
    delay(200);
    myMCP.togglePin(0, MCP::B);
    delay(200);
  }
  for(int i=0; i<5; i++){ // B7 blinking
    myMCP.togglePin(7, MCP::B);
    delay(200);
    myMCP.togglePin(7, MCP::B);
    delay(200);
  }
}

void loop(){ 
} 
