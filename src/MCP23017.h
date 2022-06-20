/*****************************************
This is a library for the MCP23017/MCP23S17/MCP23018/MCP23S18 I/O Port Expander.

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2 (English)
https://wolles-elektronikkiste.de/portexpander-mcp23017       (German)


*******************************************/


#ifndef MCP23017_H
#define MCP23017_H

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include "MCP23017_config.h"

#ifdef USE_TINY_WIRE_M_
 #include <TinyWireM.h>
#endif
#ifndef USE_TINY_WIRE_M_
 #include <Wire.h>
 #include <SPI.h>
#endif


#define IODIRA      0x00   
#define IODIRB      0x01 
#define IOCONA      0x0A   
#define IOCONB      0x0B  
#define INTCAPA     0x10  
#define INTCAPB     0x11  
#define INTCONA     0x08  
#define INTCONB     0x09  
#define INTFA       0x0E    
#define INTFB       0x0F
#define GPINTENA    0x04 
#define GPINTENB    0x05
#define DEFVALA     0x06  
#define DEFVALB     0x07
#define IPOLA       0x02   
#define GPIOA       0x12    
#define GPIOB       0x13
#define INTPOL      1    
#define INTODR      2
#define MIRROR      6    
#define GPPUA       0x0C  
#define GPPUB       0x0D
#define SPI_READ    0x01

enum MCP_PORT {A, B};
enum STATE {OFF, ON};

class MCP23017{
    public:
        MCP23017(int addr, int rp);
        MCP23017(int addr);
#ifndef USE_TINY_WIRE_M_   
        MCP23017(TwoWire *w, int addr);
        MCP23017(TwoWire *w, int addr, int rp);
        MCP23017(int cs, int rp, int addr);
        MCP23017(SPIClass *s, int cs, int rp, int addr);
#endif
        bool Init();
        void reset(); 
        void setPinMode(uint8_t, MCP_PORT, uint8_t); 
        void setPortMode(uint8_t, MCP_PORT);
        void setPortMode(uint8_t val, MCP_PORT port, uint8_t pu);
        void setPin(uint8_t, MCP_PORT, uint8_t); 
        void togglePin(uint8_t, MCP_PORT); 
        void setPinX(uint8_t, MCP_PORT, uint8_t, uint8_t); 
        void setAllPins(MCP_PORT, uint8_t); 
        void setPort(uint8_t, MCP_PORT);    
        void setPort(uint8_t, uint8_t); 
        void setPortX(uint8_t, uint8_t, MCP_PORT); 
        void setInterruptPinPol(uint8_t); 
        void setIntOdr(uint8_t);  
        void setInterruptOnChangePin(uint8_t, MCP_PORT); 
        void setInterruptOnDefValDevPin(uint8_t, MCP_PORT, uint8_t);  
        void setInterruptOnChangePort(uint8_t, MCP_PORT); 
        void setInterruptOnDefValDevPort(uint8_t, MCP_PORT, uint8_t);
        void deleteAllInterruptsOnPort(MCP_PORT); 
        void setPinPullUp(uint8_t, MCP_PORT, uint8_t); 
        void setPortPullUp(uint8_t, MCP_PORT); 
        void setIntMirror(uint8_t);
        uint8_t getIntFlag(MCP_PORT);
        bool getPin(uint8_t, MCP_PORT);
        uint8_t getPort(MCP_PORT);  
        uint8_t getIntCap(MCP_PORT);
        void setSPIClockSpeed(unsigned long clock); 
        void softReset();
        
    private:
        void setI2C_Address(int);  
        void setResetPin(uint8_t);     
        void setIoCon(uint8_t, MCP_PORT);  
        void setGpIntEn(uint8_t, MCP_PORT);  
        void setIntCon(uint8_t, MCP_PORT);   
        void setDefVal(uint8_t, MCP_PORT);    
        void writeMCP23017(uint8_t, uint8_t);
        void writeMCP23017(uint8_t, uint8_t, uint8_t);
        uint8_t readMCP23017(uint8_t);
#ifndef USE_TINY_WIRE_M_   
        TwoWire *_wire;
        SPIClass *_spi;
        SPISettings mySPISettings;
#endif
    int I2C_Address;
        int SPI_Address;
        int resetPin;
        int csPin;
        bool useSPI;
        uint8_t ioDirA, ioDirB;
        uint8_t gpioA, gpioB;
        uint8_t gpIntEnA, gpIntEnB;
        uint8_t ioConA, ioConB;
        uint8_t intConA, intConB;
        uint8_t defValA, defValB;
        uint8_t gppuA, gppuB;
};

#endif

