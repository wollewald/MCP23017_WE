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

#include <Wire.h>
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


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

enum PORT {A, B};
enum STATE {OFF, ON};

class MCP23017{
	public:
		MCP23017(int, byte); 
		void Init();
		void reset(); 
		void setPinMode(byte, PORT, byte); 
		void setPortMode(byte, PORT); 
		void setPin(byte, PORT, byte); 
		void togglePin(byte, PORT); 
		void setPinX(byte, PORT, byte, byte); 
		void setAllPins(PORT, byte); 
		void setPort(byte, PORT);    
		void setPort(byte, byte, PORT, PORT); 
		void setPortX(byte, byte, PORT); 
		void setInterruptPinPol(byte); 
		void setIntOdr(byte);  
		void setInterruptOnChangePin(byte, PORT); 
		void setInterruptOnDefValDevPin(byte, PORT, byte);  
		void setInterruptOnChangePort(byte, PORT); 
		void setInterruptOnDefValDevPort(byte, PORT, byte);
		void deleteAllInterruptsOnPort(PORT); 
		void setPinPullUp(byte, PORT, byte); 
		void setPortPullUp(byte, PORT); 
		void setIntMirror(byte);
		byte getIntFlag(PORT);
		bool getPin(byte, PORT);
		byte getPort(PORT);  
		byte getIntCap(PORT); 
		
	private:
		void setI2C_Address(int);  
		void setResetPin(byte);	   
		void setIoCon(byte, PORT);  
		void setGpIntEn(byte, PORT);  
		void setIntCon(byte, PORT);   
		void setDefVal(byte, PORT);	  
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

