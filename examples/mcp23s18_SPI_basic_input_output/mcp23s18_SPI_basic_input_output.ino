/******************************************************

Example sketch for the MCP23017/MCP23S17/MCP23018/
MCP23S18 library.

The sketch shows how to use the MCP23S18. The MCP23S18 is working
with SPI. All functions are the identical, except the constructors. 
The MCP23x18 ICs can deal with higher currents, but can only be used
as sink.

Wiring can be found under the same name (png file). 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/

#include <SPI.h>
#include <MCP23S18.h>
#define CS_PIN 7   // Chip Select Pin
#define RESET_PIN 5 
#define MCP_SPI_CTRL_BYTE 0x20 // Do not change

/* There are two ways to create your MCP23017 (MCP23S17) object:
 * MCP23S18 myMCP = MCP23S18(CS_PIN, RESET_PIN, MCP_CTRL_BYTE);
 * MCP23S18 myMCP = MCP23S18(&SPI, CS_PIN, RESET_PIN, MCP_CTRL_BYTE);
 * The second option allows you to create your own SPI objects,
 * e.g. in order to use two SPI interfaces on the ESP32.
 */
 
MCP23S18 myMCP = MCP23S18(CS_PIN, RESET_PIN, MCP_SPI_CTRL_BYTE);

int wT = 500; // wT = waiting time

void setup(){ 
  SPI.begin();
  myMCP.Init(); 
  // myMCP.setSPIClockSpeed(8000000); // Choose SPI clock speed (after Init()!)
  delay(wT);
  myMCP.setAllPins(A,OFF);            // Port A: all pins are LOW
  myMCP.setAllPins(B,OFF);            // Port B: all Pins are LOW
  myMCP.setPortMode(0b11111111, A);   // Port A: all pins are OUTPUT = LEDs are on!
  myMCP.setPortMode(0b11111111, B);   // Port B: all pins are OUTPUT
  delay(wT);
  myMCP.setPortMode(0b00000000, A);   // Port A: all pins are INPUT = LEDs are off
  myMCP.setPortMode(0b00000000, B);   // Port B: all pins are INPUT
  delay(wT);
  byte portModeValue = 0; // = 0b00000000
  for(int i=0; i<8; i++){
    portModeValue += (1<<i); // 0b00000001, 0b00000011, 0b00000111, etc.
    myMCP.setPortMode(portModeValue, A);
    delay(wT);
  }
  portModeValue = 0;
  for(int i=0; i<8; i++){
    portModeValue += (1<<i); // 0b00000001, 0b00000011, 0b00000111, etc.
    myMCP.setPortMode(portModeValue, B);
    delay(wT);
  }
  myMCP.setPortMode(0,A);           // Port A: all pins are INPUT
  myMCP.setPortMode(0,B);           // Port B: all pins are INPUT 
  delay(wT);
  myMCP.setPinMode(3, A, OUTPUT);          // Pin 3 / PORT A is OUTPUT/LOW
  myMCP.setPinMode(1, B, OUTPUT);          // Pin 1 / PORT B is OUTPUT/LOW
  delay(wT);
  myMCP.setPortMode(0,A);           // Port A: all pins are INPUT
  myMCP.setPortMode(0,B);           // Port B: all pins are INPUT
  myMCP.setPinX(1,A,OUTPUT,LOW);    // A1 HIGH 
  delay(wT);
  myMCP.togglePin(1, A);             // A1 LOW
  delay(wT);
  myMCP.togglePin(1, A);             // A1 HIGH
  delay(wT); 
  // the following two lines are similar to setPinX(2,B,OUTPUT,LOW);
  myMCP.setPinMode(2,B,OUTPUT);     // B2 is OUTPUT/LOW
  myMCP.setPin(2,B,LOW);            // B2 is still OUTPUT/LOW
  delay(wT); 
  myMCP.setPortX(0b10001111,0b10000000,B); // B0-B4: OUTPUT/LOW, B7: OUTPUT, HIGH;
}

void loop(){ 
} 
