#define MCP_ADDRESS 0x20 // (A2/A1/A0 = LOW) 
#include <Wire.h>
#include <MCP23017.h> 
MCP23017 myMCP(MCP_ADDRESS,5); 

void setup(){ 
  Wire.begin();
  myMCP.Init(); // SDA = GPIO0 / SCL = GPIO2 
  myMCP.setIoDir(B11111111, A);
  myMCP.setIoDir(B00001111, B);
  delay(1000);
  myMCP.switchPort(B11111111, B);
  delay(2000);
  myMCP.switchPort(B01011110, A); 
  delay(1000);
  myMCP.setPin(0,B,OUTPUT,HIGH); //ON oder HIGH
  myMCP.setPin(4,B,OUTPUT,LOW); //OFF oder LOW
  delay(1000);
  myMCP.switchAllPins(A, HIGH);
  delay(1000);
  myMCP.switchPin(3, A, LOW);
  delay(1000);
  myMCP.setPort(B11111111, B01101111,B);
  delay(1000);
  for(int i=0; i<5; i++){
    myMCP.togglePin(0,B);
    delay(200);
    myMCP.togglePin(0,B);
    delay(200);
  }
  for(int i=0; i<5; i++){
    myMCP.togglePin(7,B);
    delay(200);
    myMCP.togglePin(7,B);
    delay(200);
  }
} 

void loop(){ 
 
}
