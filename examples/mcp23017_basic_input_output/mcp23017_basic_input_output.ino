/******************************************************

Example sketch for the MCP23017 library

The sketch shows the features for basic input / output operations.

Wiring can be found under the same name (png file) 

written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/port-expander-mcp23017-2?lang=en

*******************************************************/

#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW) 
#include <Wire.h>
#include <MCP23017.h> 

/*
 * You can choose if you want to use the reset function or not.
 * If you don't need it, you can save one pin.
 */
MCP23017 myMCP(MCP_ADDRESS,5); // Pin 5 is used as reset pin
//MCP23017 myMCP(MCP_ADDRESS); // alternative option not using the reset pin

int wT = 1000; // wT = waiting time

void setup(){ 
  Wire.begin();
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
