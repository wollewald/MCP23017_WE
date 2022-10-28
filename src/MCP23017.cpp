/*****************************************
This is a library for the MCP23017/MCP23S17/MCP23018/MCP23S18 I/O Port Expander

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2 (English)
https://wolles-elektronikkiste.de/portexpander-mcp23017       (German)

*******************************************/

#include "MCP23017.h"

bool MCP23017::Init(){
    if (useSPI) {
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
    }
    if(resetPin < 99){
        pinMode(resetPin, OUTPUT); 
        digitalWrite(resetPin, HIGH);
        reset();
    }
    else{
        softReset();
    }
    setIoCon(0b00001000, A); // enable SPI hardware address
    setIoCon(0b00001000, B); // enable SPI hardware address
    setIntCon(0b10101010, A);
    if(readMCP23017(INTCONA) != 0b10101010){
        return false;
    }
    setIntCon(0, A);
    ioDirA = 0b00000000;
    ioDirB = 0b00000000;
    gpioA = 0b00000000;
    gpioB = 0b00000000;
    
#ifndef USE_TINY_WIRE_M_ 
    mySPISettings = SPISettings(8000000, MSBFIRST, SPI_MODE0); 
#endif
    return true;
};

void MCP23017::reset(){
    digitalWrite(resetPin,LOW);
    delay(10);
    digitalWrite(resetPin, HIGH);
    delay(10);
}

void MCP23017::setPinMode(uint8_t pin, mcp_port port, uint8_t pinState){
    uint8_t gppu = getPortPullUp(port);
    if(port==A){
        if(pinState==OUTPUT){
            ioDirA &= ~(1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT){
            ioDirA |= (1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT_PULLUP){
            ioDirA |= (1<<pin);
            gppu |= (1<<pin);
        }
        writeMCP23017(GPPUA, gppu);
        writeMCP23017(IODIRA, ioDirA);  
    }
    else if(port==B){
        if(pinState==OUTPUT){
            ioDirB &= ~(1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT){
            ioDirB |= (1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT_PULLUP){
            ioDirB |= (1<<pin);
            gppu |= (1<<pin);
        }
        writeMCP23017(GPPUB, gppu);
        writeMCP23017(IODIRB, ioDirB);  
    }       
}

void MCP23017::setPortMode(uint8_t portState, mcp_port port){
    if(port==A){
        ioDirA = ~portState;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPPUA, 0);
    }
    else if(port==B){
        ioDirB = ~portState;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPPUB, 0);
    }
}

void MCP23017::setPortMode(uint8_t portState, mcp_port port, uint8_t pu){
    uint8_t gppu = getPortPullUp(port);
    if(pu != INPUT_PULLUP){
        return;
    }
    if(port==A){
        ioDirA = ~portState;
        gppu = ~portState;
        writeMCP23017(GPPUA, gppu);
        writeMCP23017(IODIRA, ioDirA);  
    }
    else if(port==B){
        ioDirB = ~portState;
        gppu = ~portState;
        writeMCP23017(GPPUB, gppu);
        writeMCP23017(IODIRB, ioDirB);
    }
}

void MCP23017::setPin(uint8_t pin, mcp_port port, uint8_t pinLevel){
    if(port==A){
        if(pinLevel==HIGH){
            gpioA |= (1<<pin); 
        }
        else if(pinLevel==LOW){
            gpioA &= ~(1<<pin); 
        }
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(pinLevel==HIGH){
            gpioB |= (1<<pin); 
        }
        else if(pinLevel==LOW){
            gpioB &= ~(1<<pin); 
        }
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::togglePin(uint8_t pin, mcp_port port){
    if(port==A){
        if(((gpioA) & (1<<pin))==0){
            gpioA |= (1<<pin); 
        }
        else if(((gpioA) & (1<<pin)) >= 1){
            gpioA &= ~(1<<pin); 
        }
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(((gpioB) & (1<<pin))==0){
            gpioB |= (1<<pin); 
        }
        else if(((gpioB) & (1<<pin)) >= 1){
            gpioB &= ~(1<<pin); 
        }
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setPinX(uint8_t pin, mcp_port port, uint8_t pinState, uint8_t pinLevel){
    uint8_t gppu = getPortPullUp(port);
    if(port==A){
        if(pinState==OUTPUT){
            ioDirA &= ~(1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT){
            ioDirA |= (1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT_PULLUP){
            ioDirA |= (1<<pin);
            gppu |= (1<<pin);
        }
        if(pinLevel==HIGH){
            gpioA |= (1<<pin); 
        }
        else if(pinLevel==LOW){
            gpioA &= ~(1<<pin); 
        }
        writeMCP23017(GPPUA, gppu);
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(pinState==OUTPUT){
            ioDirB &= ~(1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT){
            ioDirB |= (1<<pin);
            gppu &= ~(1<<pin);
        }
        else if(pinState==INPUT_PULLUP){
            ioDirB |= (1<<pin);
            gppu |= (1<<pin);
        }
        if(pinLevel==HIGH){
            gpioB |= (1<<pin); 
        }
        else if(pinLevel==LOW){
            gpioB &= ~(1<<pin); 
        }
        writeMCP23017(GPPUB, gppu);
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setAllPins(mcp_port port, uint8_t pinLevel){
    if(port==A){
        if(pinLevel==HIGH){
            gpioA = 0b11111111;
        }
        else if (pinLevel==LOW){
            gpioA = 0b00000000;
        }
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(pinLevel==HIGH){
            gpioB = 0b11111111;
        }
        else if (pinLevel==LOW){
            gpioB = 0b00000000;
        }
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setPort(uint8_t portLevel, mcp_port port){
    if(port==A){
        gpioA = portLevel;
        writeMCP23017(GPIOA, gpioA);
    }
    else if(port==B){
        gpioB = portLevel;
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setPort(uint8_t portLevelA, uint8_t portLevelB){
    gpioA = portLevelA;
    gpioB = portLevelB;
    writeMCP23017(GPIOA, gpioA, gpioB);
}

void MCP23017::setPortX(uint8_t portState, uint8_t portLevel, mcp_port port){
    if(port==A){
        ioDirA = ~portState;
        gpioA = portLevel;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    else if(port==B){
        ioDirB = ~portState;
        gpioB = portLevel;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setInterruptPinPol(uint8_t level){
    uint8_t ioConA = getIoCon(A);
    uint8_t ioConB = getIoCon(B);
    if(level==HIGH){
        ioConA |= (1<<INTPOL);
        ioConB |= (1<<INTPOL);
    }
    if(level==LOW){
        ioConA &= ~(1<<INTPOL);
        ioConB &= ~(1<<INTPOL);
    }
    writeMCP23017(IOCONA, ioConA);
    writeMCP23017(IOCONB, ioConB);
}   

void MCP23017::setIntOdr(uint8_t openDrain){
    uint8_t ioConA = getIoCon(A);
    uint8_t ioConB = getIoCon(B);
    if(openDrain){
        ioConA |= (1<<INTODR);
        ioConB |= (1<<INTODR);
    }
    else{
        ioConA &= ~(1<<INTODR);
        ioConB &= ~(1<<INTODR);
    }
    writeMCP23017(IOCONA, ioConA);
    writeMCP23017(IOCONB, ioConB);
}   

void MCP23017::setInterruptOnChangePin(uint8_t pin, mcp_port port){
    uint8_t gpIntEn = getGpIntEn(port);
    if(port==A){
        ioDirA |= (1<<pin); 
        gpIntEn |= (1<<pin);
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
        writeMCP23017(GPINTENA, gpIntEn);
    }
    else if (port==B){
        ioDirB |= (1<<pin); 
        gpIntEn |= (1<<pin);
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
        writeMCP23017(GPINTENB, gpIntEn);
    }
}

void MCP23017::setInterruptOnDefValDevPin(uint8_t pin, mcp_port port, uint8_t pinIntLevel){
    uint8_t gpIntEn = getGpIntEn(port);
    uint8_t intCon = getIntCon(port);
    uint8_t defVal = getDefVal(port);
    if(port==A){
        ioDirA |= (1<<pin); 
        gpIntEn |= (1<<pin);
        intCon |= (1<<pin);
        if(pinIntLevel==HIGH) defVal |= (1<<pin);
        else if(pinIntLevel==LOW) defVal &= ~(1<<pin);
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
        writeMCP23017(GPINTENA, gpIntEn);
        writeMCP23017(INTCONA, intCon);
        writeMCP23017(DEFVALA, defVal);
    }
    else if (port==B){
        ioDirB |= (1<<pin); 
        gpIntEn |= (1<<pin);
        intCon |= (1<<pin);
        if(pinIntLevel==HIGH) defVal |= (1<<pin);
        else if(pinIntLevel==LOW) defVal &= ~(1<<pin);
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
        writeMCP23017(GPINTENB, gpIntEn);
        writeMCP23017(INTCONB, intCon);
        writeMCP23017(DEFVALB, defVal);
    }
}

void MCP23017::setInterruptOnChangePort(uint8_t intOnChangePins, mcp_port port){
    if(port==A){
        ioDirA |= intOnChangePins;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPINTENA, intOnChangePins);
    }
    else if (port==B){
        ioDirB |= intOnChangePins;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPINTENB, intOnChangePins);
    }
}

void MCP23017::setInterruptOnDefValDevPort(uint8_t intPins, mcp_port port, uint8_t defVal){
    uint8_t gpIntEn = getGpIntEn(port);
    uint8_t intCon = getIntCon(port);
    intCon |= intPins;
    gpIntEn |= intPins;
    if(port==A){
        ioDirA |= intPins; 
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPINTENA, gpIntEn);
        writeMCP23017(INTCONA, intCon);
        writeMCP23017(DEFVALA, defVal);
    }
    else if (port==B){
        ioDirB |= intPins; 
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPINTENB, gpIntEn);
        writeMCP23017(INTCONB, intCon);
        writeMCP23017(DEFVALB, defVal);
    }
}

void MCP23017::deleteAllInterruptsOnPort(mcp_port port){
    if(port==A){
        writeMCP23017(GPINTENA, 0);
    }
    else if (port==B){
        writeMCP23017(GPINTENB, 0);
    }
}

void MCP23017::setPinPullUp(uint8_t pin, mcp_port port, uint8_t pinLevel){
    uint8_t gppu = getPortPullUp(port);
    if(port==A){
        if(pinLevel==HIGH){
            gppu |= (1<<pin);
        }
        else if(pinLevel==LOW){
            gppu &= ~(1<<pin);
        }
        writeMCP23017(GPPUA, gppu);
    }
    else if(port==B){
        if(pinLevel==HIGH){
            gppu |= (1<<pin);
        }
        else if(pinLevel==LOW){
            gppu &= ~(1<<pin);
        }
        writeMCP23017(GPPUB, gppu);
    }
}
        
void MCP23017::setPortPullUp(uint8_t pulledUpPins, mcp_port port){
    if(port==A){
        writeMCP23017(GPPUA, pulledUpPins);
    }
    else if(port==B){
        writeMCP23017(GPPUB, pulledUpPins);
    }
}

uint8_t MCP23017::getPortPullUp(mcp_port port){
    if(port==A){
        return readMCP23017(GPPUA);
    }
    else{ 
        return readMCP23017(GPPUB);
    }
}      

void MCP23017::setIntMirror(uint8_t mirrored){
    uint8_t ioConA = getIoCon(A);
    uint8_t ioConB = getIoCon(B);
    if(mirrored){
        ioConA |= (1<<MIRROR);
        ioConB |= (1<<MIRROR);
    }
    else{
        ioConA &= ~(1<<MIRROR);
        ioConB &= ~(1<<MIRROR);
    }
    writeMCP23017(IOCONA, ioConA);
    writeMCP23017(IOCONB, ioConB);
}   

uint8_t MCP23017::getIntFlag(mcp_port port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017(INTFA);
    }
    else if (port==B){ 
        value = readMCP23017(INTFB);
    }
    return value;
}

bool MCP23017::getPin(uint8_t pin, mcp_port port){
    uint8_t result = 0;
    if(port==A){
        result = readMCP23017(GPIOA);
    }
    else if(port==B){
        result = readMCP23017(GPIOB);
    }
    result &= (1<<pin);
    if(result) return true;
    else return false;  
}

uint8_t MCP23017::getPort(mcp_port port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017(GPIOA);
    }
    else if(port==B){
        value = readMCP23017(GPIOB);
    }
    return value;
}

uint8_t MCP23017::getIntCap(mcp_port port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017(INTCAPA);
    }
    else if (port==B){
        value = readMCP23017(INTCAPB);
    }
    return value;
}

#ifndef USE_TINY_WIRE_M_
void MCP23017::setSPIClockSpeed(unsigned long clock){
    mySPISettings = SPISettings(clock, MSBFIRST, SPI_MODE0);
}
#endif

void MCP23017::softReset(){
    setPortMode(0, A);
    setPortMode(0, B);
    uint8_t reg = 0x02;
    if(!useSPI){
#ifndef USE_TINY_WIRE_M_
        _wire->beginTransmission(I2C_Address);
        _wire->write(reg);
        for(int8_t i=0; i<18; i++){
            _wire->write(0x00);
        }
        _wire->endTransmission();
#else
        TinyWireM.beginTransmission(I2C_Address);
        TinyWireM.send(reg);
        for(int8_t i=0; i<18; i++){
            TinyWireM.send(0x00);
        }
        TinyWireM.endTransmission();    
#endif
    }
#ifndef USE_TINY_WIRE_M_    
    else{
        _spi->beginTransaction(mySPISettings);
        digitalWrite(csPin, LOW);
        uint16_t transBytes = ((SPI_Address<<1) << 8 | reg);
        _spi->transfer16(transBytes); 
        for(int8_t i=0; i<18; i++){
            _spi->transfer(0x00);
        }
        digitalWrite(csPin, HIGH);
        _spi->endTransaction();
    }
#endif
}

/* Private Functions */

void MCP23017::setResetPin(uint8_t rp){
    resetPin = rp;
}

void MCP23017::setIoCon(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017(IOCONA, val);
    }
    else if (port==B){
        writeMCP23017(IOCONB, val);
    }
}

uint8_t MCP23017::getIoCon(mcp_port port){
    if(port==A){
        return readMCP23017(IOCONA);
    }
    else{
        return readMCP23017(IOCONB);
    }
}


void MCP23017::setGpIntEn(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017(GPINTENA, val);
    }
    else if (port==B){
        writeMCP23017(GPINTENB, val);  
    }
}


uint8_t MCP23017::getGpIntEn(mcp_port port){
    if(port==A){
        return readMCP23017(GPINTENA);
    }
    else{
        return readMCP23017(GPINTENB);
    }
}


void MCP23017::setIntCon(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017(INTCONA, val);
    }
    else if (port==B){
        writeMCP23017(INTCONB, val);
    }
}

uint8_t MCP23017::getIntCon(mcp_port port){
    if(port==A){
        return readMCP23017(INTCONA);
    }
    else{
        return readMCP23017(INTCONB);
    }
}

void MCP23017::setDefVal(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017(DEFVALA, val);
    }
    else if (port==B){
        writeMCP23017(DEFVALB, val);    
    }
}

uint8_t MCP23017::getDefVal(mcp_port port){
    if(port==A){
        return readMCP23017(DEFVALA);
    }
    else{
        return readMCP23017(DEFVALB);  
    }
}

void MCP23017::writeMCP23017(uint8_t reg, uint8_t val){
    if(!useSPI){
#ifndef USE_TINY_WIRE_M_
        _wire->beginTransmission(I2C_Address);
        _wire->write(reg);
        _wire->write(val);
        _wire->endTransmission();
#else
        TinyWireM.beginTransmission(I2C_Address);
        TinyWireM.send(reg);
        TinyWireM.send(val);
        TinyWireM.endTransmission();    
#endif
    }
#ifndef USE_TINY_WIRE_M_    
    else{
        _spi->beginTransaction(mySPISettings);
        digitalWrite(csPin, LOW);
        uint16_t transBytes = ((SPI_Address<<1) << 8 | reg);
        _spi->transfer16(transBytes); 
        _spi->transfer(val);
        digitalWrite(csPin, HIGH);
        _spi->endTransaction();
    }
#endif
}

void MCP23017::writeMCP23017(uint8_t reg, uint8_t valA, uint8_t valB){
    if(!useSPI){
#ifndef USE_TINY_WIRE_M_
        _wire->beginTransmission(I2C_Address);
        _wire->write(reg);
        _wire->write(valA);
        _wire->write(valB);
        _wire->endTransmission();
#else
        TinyWireM.beginTransmission(I2C_Address);
        TinyWireM.send(reg);
        TinyWireM.send(valA);
        TinyWireM.send(valB);
        TinyWireM.endTransmission();    
#endif  
    }
#ifndef USE_TINY_WIRE_M_
    else{
        _spi->beginTransaction(mySPISettings);
        digitalWrite(csPin, LOW);
        uint16_t transBytes = ((SPI_Address<<1) << 8 | reg);
        _spi->transfer16(transBytes); 
        _spi->transfer(valA);
        _spi->transfer(valB);
        digitalWrite(csPin, HIGH);
        _spi->endTransaction();
    }
#endif
}

uint8_t MCP23017::readMCP23017(uint8_t reg){
    uint8_t regVal;
    if(!useSPI){
#ifndef USE_TINY_WIRE_M_
        _wire->beginTransmission(I2C_Address);
        _wire->write(reg);
        _wire->endTransmission(false);
        _wire->requestFrom(I2C_Address, static_cast<uint8_t>(1));
        regVal = _wire->read();
#else
        TinyWireM.beginTransmission(I2C_Address);
        TinyWireM.send(reg);
        TinyWireM.endTransmission();
        TinyWireM.requestFrom(I2C_Address, static_cast<uint8_t>(1));
        regVal = TinyWireM.receive();
#endif
        return regVal;
    }
#ifndef USE_TINY_WIRE_M_
    else{
        _spi->beginTransaction(mySPISettings);
        digitalWrite(csPin, LOW);
        uint16_t transBytes = (((SPI_Address<<1) | SPI_READ) << 8 | reg);
        _spi->transfer16(transBytes); 
        regVal = _spi->transfer(0x00);
        digitalWrite(csPin, HIGH);
        _spi->endTransaction();
        return regVal;
    }
#endif
}

