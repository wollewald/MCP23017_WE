/*****************************************
This is a library for the MCP23017 I/O Port Expander

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de

*******************************************/


#ifndef MCP23017_H
#define MCP23017_H

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include <Wire.h>


#define IODIRA 0x00   
#define IODIRB 0x01   
#define IOCONA 0x0A   
#define IOCONB 0x0B  
#define INTCAPA 0x10  
#define INTCAPB 0x11  
#define INTCONA 0x08  
#define INTCONB 0x09  
#define INTFA 0x0E    
#define INTFB 0x0F
#define GPINTENA 0x04 
#define GPINTENB 0x05
#define DEFVALA 0x06  
#define DEFVALB 0x07
#define IPOLA 0x02	 
#define GPIOA 0x12    
#define GPIOB 0x13
#define INTPOL 1	
#define INTODR 2
#define MIRROR 6	
#define GPPUA 0x0C	
#define GPPUB 0x0D

enum MCP_PORT {A, B};
enum STATE {OFF, ON};

class MCP23017{
	public:
		MCP23017(int, byte); 
		void Init();
		void reset(); 
		void setPinMode(byte, MCP_PORT, byte); 
		void setPortMode(byte, MCP_PORT); 
		void setPin(byte, MCP_PORT, byte); 
		void togglePin(byte, MCP_PORT); 
		void setPinX(byte, MCP_PORT, byte, byte); 
		void setAllPins(MCP_PORT, byte); 
		void setPort(byte, MCP_PORT);    
		void setPort(byte, byte, MCP_PORT, MCP_PORT); 
		void setPortX(byte, byte, MCP_PORT); 
		void setInterruptPinPol(byte); 
		void setIntOdr(byte);  
		void setInterruptOnChangePin(byte, MCP_PORT); 
		void setInterruptOnDefValDevPin(byte, MCP_PORT, byte);  
		void setInterruptOnChangePort(byte, MCP_PORT); 
		void setInterruptOnDefValDevPort(byte, MCP_PORT, byte);
		void deleteAllInterruptsOnPort(MCP_PORT); 
		void setPinPullUp(byte, MCP_PORT, byte); 
		void setPortPullUp(byte, MCP_PORT); 
		void setIntMirror(byte);
		byte getIntFlag(MCP_PORT);
		bool getPin(byte, MCP_PORT);
		byte getPort(MCP_PORT);  
		byte getIntCap(MCP_PORT); 
		
	private:
		void setI2C_Address(int);  
		void setResetPin(byte);	   
		void setIoCon(byte, MCP_PORT);  
		void setGpIntEn(byte, MCP_PORT);  
		void setIntCon(byte, MCP_PORT);   
		void setDefVal(byte, MCP_PORT);	  
		void writeMCP23017(byte, byte);
		void writeMCP23017(byte, byte, byte);
		byte readMCP23017(byte);
		int I2C_Address;
		byte resetPin;
		byte ioDirA, ioDirB;
		byte gpioA, gpioB;
		byte gpIntEnA, gpIntEnB;
		byte ioConA, ioConB;
		byte intConA, intConB;
		byte defValA, defValB;
		byte gppuA, gppuB;
};

#endif

