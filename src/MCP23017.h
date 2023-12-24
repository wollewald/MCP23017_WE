/*****************************************
This is a library for the MCP23017/MCP23S17/MCP23018/MCP23S18 I/O Port Expander.

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

LICENSE: MIT (https://github.com/wollewald/MCP23017_WE/blob/master/LICENSE)

In case you like it, it would be cool if you give it a star.

If you find bugs, don't hesitate to raise an issue!

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


typedef enum MCP_PORT {A, B} mcp_port;
typedef enum MCP_ENABLE {OFF, ON} mcp_enable;

class MCP23017{
    public:
        /* Registers */
        static constexpr uint8_t IODIRA  {0x00};  
        static constexpr uint8_t IODIRB  {0x01}; 
        static constexpr uint8_t IOCONA  {0x0A}; 
        static constexpr uint8_t IOCONB  {0x0B};
        static constexpr uint8_t INTCAPA {0x10};
        static constexpr uint8_t INTCAPB {0x11};
        static constexpr uint8_t INTCONA {0x08};
        static constexpr uint8_t INTCONB {0x09};
        static constexpr uint8_t INTFA   {0x0E};  
        static constexpr uint8_t INTFB   {0x0F};
        static constexpr uint8_t GPINTENA{0x04};
        static constexpr uint8_t GPINTENB{0x05};
        static constexpr uint8_t DEFVALA {0x06};
        static constexpr uint8_t DEFVALB {0x07};
        static constexpr uint8_t IPOLA   {0x02}; 
        static constexpr uint8_t GPIOA   {0x12};  
        static constexpr uint8_t GPIOB   {0x13};
        static constexpr uint8_t INTPOL  {0x01};  
        static constexpr uint8_t INTODR  {0x02};
        static constexpr uint8_t MIRROR  {0x06};  
        static constexpr uint8_t GPPUA   {0x0C};
        static constexpr uint8_t GPPUB   {0x0D};
        static constexpr uint8_t SPI_READ{0x01};

        /* constructors */
#ifndef USE_TINY_WIRE_M_
        MCP23017(uint8_t addr, uint8_t rp = 99) : _wire{&Wire}, I2C_Address{addr}, resetPin{rp}, useSPI{false} {}
        MCP23017(TwoWire *w, uint8_t addr, uint8_t rp = 99) : _wire{w}, I2C_Address{addr}, resetPin{rp}, useSPI{false} {}
        MCP23017(uint8_t cs, uint8_t rp, uint8_t addr) : _spi{&SPI}, SPI_Address{addr}, resetPin{rp}, csPin{cs}, useSPI{true} {}
        MCP23017(SPIClass *s, uint8_t cs, uint8_t rp, uint8_t addr) : _spi{s}, SPI_Address{addr}, resetPin{rp}, csPin{cs}, useSPI{true} {}
#else
        MCP23017(uint8_t addr, uint8_t rp) : I2C_Address{addr}, resetPin{rp}, useSPI{false} {}
#endif
        
        /* Public functions */
        bool Init();
        void reset(); 
        void setPinMode(uint8_t, mcp_port, uint8_t); 
        void setPortMode(uint8_t, mcp_port);
        void setPortMode(uint8_t val, mcp_port, uint8_t pu);
        void setPin(uint8_t, mcp_port, uint8_t); 
        void togglePin(uint8_t, mcp_port); 
        void setPinX(uint8_t, mcp_port, uint8_t, uint8_t); 
        void setAllPins(mcp_port, uint8_t); 
        void setPort(uint8_t, mcp_port);    
        void setPort(uint8_t, uint8_t); 
        void setPortX(uint8_t, uint8_t, mcp_port); 
        void setInterruptPinPol(uint8_t); 
        void setIntOdr(uint8_t);  
        void setInterruptOnChangePin(uint8_t, mcp_port); 
        void setInterruptOnDefValDevPin(uint8_t, mcp_port, uint8_t);  
        void setInterruptOnChangePort(uint8_t, mcp_port); 
        void setInterruptOnDefValDevPort(uint8_t, mcp_port, uint8_t);
        void deleteAllInterruptsOnPort(mcp_port); 
        void setPinPullUp(uint8_t, mcp_port, uint8_t); 
        void setPortPullUp(uint8_t, mcp_port);
        uint8_t getPortPullUp(mcp_port);
        void setIntMirror(uint8_t);
        uint8_t getIntFlag(mcp_port);
        bool getPin(uint8_t, mcp_port);
        uint8_t getPort(mcp_port);  
        uint8_t getIntCap(mcp_port);
        void setSPIClockSpeed(unsigned long clock); 
        void softReset();
        uint8_t i2cConnectionError();
#ifdef DEBUG_MCP23017  // see MCP23017_config.h
        void printAllRegisters();
        void printBin(uint8_t val);
#endif
        
    protected:
        void setResetPin(uint8_t);     
        void setIoCon(uint8_t, mcp_port);
        uint8_t getIoCon(mcp_port);
        void setGpIntEn(uint8_t, mcp_port);
        uint8_t getGpIntEn(mcp_port);
        void setIntCon(uint8_t, mcp_port);
        uint8_t getIntCon(mcp_port);
        void setDefVal(uint8_t, mcp_port);
        uint8_t getDefVal(mcp_port);
        void writeMCP23017(uint8_t, uint8_t);
        void writeMCP23017(uint8_t, uint8_t, uint8_t);
        uint8_t readMCP23017(uint8_t);
#ifndef USE_TINY_WIRE_M_   
        TwoWire *_wire;
        SPIClass *_spi;
        SPISettings mySPISettings;
#endif
        uint8_t I2C_Address;
        uint8_t SPI_Address;
        uint8_t resetPin;
        uint8_t csPin;
        bool useSPI;
        uint8_t ioDirA, ioDirB;
        uint8_t gpioA, gpioB;
};

#endif

