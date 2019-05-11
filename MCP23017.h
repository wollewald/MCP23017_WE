#ifndef MCP23017_H
#define MCP23017_H

#include<Wire.h>   //requires wire library 
#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif


#define IODIRA 0x00   //IODIR Register PORT A --> 1 = Input, 0 = Output
#define IODIRB 0x01   //IODIR Register PORT B 
#define IOCONA 0x0A   //Konfigurationsregister PORT A --> see datasheet
#define IOCONB 0x0B  
#define INTCAPA 0x10  //Interrupt Capture Register A --> GPIO Port Value at Interrupt
#define INTCAPB 0x11  //Interrupt Capture Register B 
#define INTCONA 0x08  //Interrupt Configuation --> 0: Interrupt on Change, 1: Interrupt if DEFVAL != Pin state
#define INTCONB 0x09  
#define INTFA 0x0E    //Interrupt Flag Register A --> captures which Pin caused Interrupt
#define INTFB 0x0F
#define GPINTENA 0x04 //Interrupt Enable Register A --> 0: Enabled, 1: Disabled
#define GPINTENB 0x05
#define DEFVALA 0x06  //Opposite GPIO Pin state causes Interrupt if corresponding INTCON Pin = 1;
#define DEFVALB 0x07
#define IPOLA 0x02	  //Input Polarity of GPIO Pin --> 1: GPIO Register bit is opposite 
#define GPIOA 0x12    //GPIO Register: 0 = LOW, 1 = HIGH
#define GPIOB 0x13

enum PORT {A, B};
enum STATE {OFF, ON};

class MCP23017{
	public:
		MCP23017(int, byte); // address, resetPin
		void Init();
		void Init(int,int);
		void setI2C_Address(int);  //Address = 1 0 0 A2 A1 A0
		void setResetPin(byte);	   //Reset Pin des Arduino
		void setIoDir(byte, PORT);  //sets complete IODIR 
		void setGpioOut(byte, PORT);    //sets complete GPIO A or B, "OUT and HIGH"
		void setGpioOut(byte, byte, PORT, PORT); // sets GPIOA and GPIOB
		void setGpioInOut(byte, byte, PORT); // sets complete IODIR A or B and GPIO A or B 
		void setGpIntEn(byte, PORT);  //sets complete GPINTENA
		void setIoCon(byte, PORT);   //sets complete IOCON
		void setIntCon(byte, PORT);   // sets complete INTCON
		void setDefVal(byte, PORT);	  // sets complete DEFVAL
		void switchPin(byte, PORT, STATE); //switches single Pin
		void togglePin(byte, PORT); // toggles single Pin
		void setInterruptOnChangePin(byte, PORT); // sets Interrrupt On Change for single Pin
		void setInterruptOnDefValDev(byte, PORT, STATE);  //"Interrupt on Defval", single Pin, State = the state which will cause an Interrupt
		void setInterruptOnChangePort(byte, PORT); // sets Intterupt on Change for complete Port
		void deleteAllInterruptsOnPort(PORT); // deletes all Interupts 
		byte getIntFlag(PORT); // self explaining
		byte getGpio(PORT);  //self explaining
		byte getIntCap(PORT); // self explaining
		
		
	private:
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
};

#endif

