/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for GPIO reading.
Just attach external HIGH or LOW to the pins and see whats happening.  

Same wiring can be used as for the mcp23017_basic_input_output.ino 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/
#include <Wire.h>
#include <MCP23017.h>
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)
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
byte portStatus;
bool pinStatus;

void setup(){ 
  Serial.begin(9600);
  Wire.begin();
  myMCP.Init();  
  myMCP.setPortMode(0b11111111, A);  // Port A: all pins are OUTPUT
  myMCP.setPortMode(0b11111111, B);  // Port B: all pins are OUTPUT
  myMCP.setPort(0b10010011,A);  // 
}

void loop(){ 
  portStatus = myMCP.getPort(A);
  Serial.print("Status GPIO A: ");
  Serial.println(portStatus, BIN);
  pinStatus = myMCP.getPin(7, A);
  Serial.print("Status Port A, Pin 7: ");
  Serial.println(pinStatus, BIN);
  portStatus = myMCP.getPort(B);
  Serial.print("Status GPIO B: ");
  Serial.println(portStatus, BIN);
  pinStatus = myMCP.getPin(0, B);
  Serial.print("Status Port B, Pin 0: ");
  Serial.println(pinStatus, BIN);
  Serial.println("-------------------------------------");
  delay(5000);
} 
