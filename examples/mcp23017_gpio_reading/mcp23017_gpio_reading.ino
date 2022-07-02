/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for GPIO reading.
I have added a Fritzing scheme for this sketch. Press the push buttons
and see what's happening.   

Same wiring can be used as for the mcp23017_basic_input_output.ino 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2

*******************************************************/
#include <Wire.h>
#include <MCP23017.h>
#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW)

/* A hardware reset is performed during init(). If you want to save a pin you can define a dummy 
 * pin >= 99 and connect the reset pin to HIGH. This will trigger a software reset instead of a 
 * hardware reset. 
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
byte portStatus;
bool pinStatus;

void setup(){ 
  Serial.begin(9600);
  Wire.begin();
  if(!myMCP.Init()){
    Serial.println("Not connected!");
    while(1){} 
  }  
  myMCP.setPortMode(0b00000000, A);  // Port A: all pins are INPUT
  myMCP.setPortPullUp(0b11110000, A);  // Port A: Pin 4 - 7 are pulled up
}

void loop(){ 
  
  portStatus = myMCP.getPort(A); // query the complete port status 
  Serial.print("Status GPIO A: ");
  Serial.println(portStatus, BIN);
  
  pinStatus = myMCP.getPin(5, A); // query one pin status
  Serial.print("Status Port A, Pin 5: ");
  Serial.println(pinStatus, BIN);
 
  Serial.println("-------------------------------------");
  delay(1000);
} 
