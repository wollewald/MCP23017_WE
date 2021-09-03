/*****************************************
This is a library for the MCP23017 I/O Port Expander

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/port-expander-mcp23017-2?lang=en

*******************************************/

#include "MCP23017.h"

MCP23017::MCP23017(int addr){
    _wire = &Wire;
    I2C_Address = addr;   
}

MCP23017::MCP23017(int addr, uint8_t rp){
    _wire = &Wire;
    I2C_Address = addr;
    resetPin = rp;
    pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, HIGH);
}

MCP23017::MCP23017(TwoWire *w, int addr){
    _wire = w;
    I2C_Address = addr; 
}

MCP23017::MCP23017(TwoWire *w, int addr, uint8_t rp){
    _wire = w;
    I2C_Address = addr;
    resetPin = rp;
    pinMode(resetPin, OUTPUT); 
    digitalWrite(resetPin, HIGH);
}

void MCP23017::Init(){
    ioConA = B00000000;
    ioConB = B00000000;
    ioDirA = B00000000;
    ioDirB = B00000000;
    gppuA = B00000000;
    gppuB = B00000000;
    setPortX(B00000000, B00000000, A);
    setPortX(B00000000, B00000000, B);
    setGpIntEn(B00000000,A);
    setGpIntEn(B00000000,B);
    setIoCon(ioConA, A);
    setIoCon(ioConB, B);
    setIntCon(B00000000, A);
    setIntCon(B00000000, B);
    setDefVal(B00000000, A);
    setDefVal(B00000000, B);
};

void MCP23017::reset(){
    digitalWrite(resetPin,LOW);
    delay(10);
    digitalWrite(resetPin, HIGH);
    delay(10);
}

void MCP23017::setPinMode(uint8_t pin, MCP_PORT port, uint8_t state){
    if(port==A){
        if(state==ON){
            ioDirA &= ~(1<<pin);
            gppuA &= ~(1<<pin);
        }
        else if(state==OFF){
            ioDirA |= (1<<pin);
            gppuA &= ~(1<<pin);
        }
        else if(state==INPUT_PULLUP){
            ioDirA |= (1<<pin);
            gppuA |= (1<<pin);
        }
        writeMCP23017(GPPUA, gppuA);
        writeMCP23017(IODIRA, ioDirA);  
    }
    else if(port==B){
        if(state==ON){
            ioDirB &= ~(1<<pin);
            gppuB &= ~(1<<pin);
        }
        else if(state==OFF){
            ioDirB |= (1<<pin);
            gppuB &= ~(1<<pin);
        }
        else if(state==INPUT_PULLUP){
            ioDirB |= (1<<pin);
            gppuB |= (1<<pin);
        }
        writeMCP23017(GPPUB, gppuB);
        writeMCP23017(IODIRB, ioDirB);  
    }       
}

void MCP23017::setPortMode(uint8_t val, MCP_PORT port){
    if(port==A){
        ioDirA = ~val;
        gppuA = 0;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPPUA, gppuA);
    }
    else if(port==B){
        ioDirB = ~val;
        gppuB = 0;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPPUB, gppuB);
    }
}

void MCP23017::setPortMode(uint8_t val, MCP_PORT port, uint8_t pu){
    if(pu != INPUT_PULLUP){
        return;
    }
    if(port==A){
        ioDirA = ~val;
        gppuA = ~val;
        writeMCP23017(GPPUA, gppuA);
        writeMCP23017(IODIRA, ioDirA);  
    }
    else if(port==B){
        ioDirB = ~val;
        gppuB = ~val;
        writeMCP23017(GPPUB, gppuB);
        writeMCP23017(IODIRB, ioDirB);
    }
}

void MCP23017::setPin(uint8_t pin, MCP_PORT port, uint8_t state){
    if(port==A){
        if(state==ON){
            gpioA |= (1<<pin); 
        }
        else if(state==OFF){
            gpioA &= ~(1<<pin); 
        }
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(state==ON){
            gpioB |= (1<<pin); 
        }
        else if(state==OFF){
            gpioB &= ~(1<<pin); 
        }
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::togglePin(uint8_t pin, MCP_PORT port){
    if(port==A){
        if(((gpioA) & (1<<pin))==0){
            gpioA |= (1<<pin); 
        }
        else if(((gpioA) & (1<<pin)) >= 1){
            gpioA &= ~(1<<pin); 
        }
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(((gpioB) & (1<<pin))==0){
            gpioB |= (1<<pin); 
        }
        else if(((gpioB) & (1<<pin)) >= 1){
            gpioB &= ~(1<<pin); 
        }
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setPinX(uint8_t pin, MCP_PORT port, uint8_t ioDir, uint8_t state){
    if(port==A){
        if(ioDir==OUTPUT){
            ioDirA &= ~(1<<pin);
            gppuA &= ~(1<<pin);
        }
        else if(ioDir==INPUT){
            ioDirA |= (1<<pin);
            gppuA &= ~(1<<pin);
        }
        else if(ioDir==INPUT_PULLUP){
            ioDirA |= (1<<pin);
            gppuA |= (1<<pin);
        }
        if(state==ON){
            gpioA |= (1<<pin); 
        }
        else if(state==OFF){
            gpioA &= ~(1<<pin); 
        }
        writeMCP23017(GPPUA, gppuA);
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(ioDir==OUTPUT){
            ioDirB &= ~(1<<pin);
            gppuB &= ~(1<<pin);
        }
        else if(ioDir==INPUT){
            ioDirB |= (1<<pin);
            gppuB &= ~(1<<pin);
        }
        else if(ioDir==INPUT_PULLUP){
            ioDirB |= (1<<pin);
            gppuB |= (1<<pin);
        }
        if(state==ON){
            gpioB |= (1<<pin); 
        }
        else if(state==OFF){
            gpioB &= ~(1<<pin); 
        }
        writeMCP23017(GPPUB, gppuB);
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setAllPins(MCP_PORT port, uint8_t state){
    if(port==A){
        if(state==ON){
            gpioA = B11111111;
        }
        else if (state==OFF){
            gpioA = B00000000;
        }
        writeMCP23017(GPIOA, gpioA);
    }
    if(port==B){
        if(state==ON){
            gpioB = B11111111;
        }
        else if (state==OFF){
            gpioB = B00000000;
        }
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setPort(uint8_t val, MCP_PORT port){
    if(port==A){
        gpioA = val;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    else if(port==B){
        gpioB = val;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setPort(uint8_t valA, uint8_t valB){
    gpioA = valA;
    gpioB = valB;
    writeMCP23017(IODIRA, ioDirA, ioDirB);
    writeMCP23017(GPIOA, gpioA, gpioB);
}

void MCP23017::setPortX(uint8_t iodirval, uint8_t gpioval, MCP_PORT port){
    if(port==A){
        ioDirA = ~iodirval;
        gpioA = gpioval;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
    }
    else if(port==B){
        ioDirB = ~iodirval;
        gpioB = gpioval;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
    }
}

void MCP23017::setInterruptPinPol(uint8_t state){
    if(state==HIGH){
        ioConA |= (1<<INTPOL);
        ioConB |= (1<<INTPOL);
    }
    if(state==LOW){
        ioConA &= ~(1<<INTPOL);
        ioConB &= ~(1<<INTPOL);
    }
    writeMCP23017(IOCONA, ioConA);
    writeMCP23017(IOCONB, ioConB);
}   

void MCP23017::setIntOdr(uint8_t state){
    if(state==ON){
        ioConA |= (1<<INTODR);
        ioConB |= (1<<INTODR);
    }
    if(state==OFF){
        ioConA &= ~(1<<INTODR);
        ioConB &= ~(1<<INTODR);
    }
    writeMCP23017(IOCONA, ioConA);
    writeMCP23017(IOCONB, ioConB);
}   

void MCP23017::setInterruptOnChangePin(uint8_t pin, MCP_PORT port){
    if(port==A){
        ioDirA |= (1<<pin); 
        gpIntEnA |= (1<<pin);
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
        writeMCP23017(GPINTENA, gpIntEnA);
    }
    else if (port==B){
        ioDirB |= (1<<pin); 
        gpIntEnB |= (1<<pin);
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
        writeMCP23017(GPINTENB, gpIntEnB);
    }
}

void MCP23017::setInterruptOnDefValDevPin(uint8_t pin, MCP_PORT port, uint8_t intState){
    if(port==A){
        ioDirA |= (1<<pin); 
        gpIntEnA |= (1<<pin);
        intConA |= (1<<pin);
        if(intState==ON) defValA |= (1<<pin);
        else if(intState==OFF) defValA &= ~(1<<pin);
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPIOA, gpioA);
        writeMCP23017(GPINTENA, gpIntEnA);
        writeMCP23017(INTCONA, intConA);
        writeMCP23017(DEFVALA, defValA);
    }
    else if (port==B){
        ioDirB |= (1<<pin); 
        gpIntEnB |= (1<<pin);
        intConB |= (1<<pin);
        if(intState==ON) defValB |= (1<<pin);
        else if(intState==OFF) defValB &= ~(1<<pin);
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPIOB, gpioB);
        writeMCP23017(GPINTENB, gpIntEnB);
        writeMCP23017(INTCONB, intConB);
        writeMCP23017(DEFVALB, defValB);
    }
}

void MCP23017::setInterruptOnChangePort(uint8_t val, MCP_PORT port){
    if(port==A){
        ioDirA |= val;
        gpIntEnA = val;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPINTENA, gpIntEnA);
    }
    else if (port==B){
        ioDirB |= val;
        gpIntEnB = val;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPINTENB, gpIntEnB);
    }
}

void MCP23017::setInterruptOnDefValDevPort(uint8_t val, MCP_PORT port, uint8_t state){
    if(port==A){
        ioDirA |= val; 
        gpIntEnA |= val;
        intConA |= val;
        defValA = state;
        writeMCP23017(IODIRA, ioDirA);
        writeMCP23017(GPINTENA, gpIntEnA);
        writeMCP23017(INTCONA, intConA);
        writeMCP23017(DEFVALA, defValA);
    }
    else if (port==B){
        ioDirB |= val; 
        gpIntEnB |= val;
        intConB |= val;
        defValB = state;
        writeMCP23017(IODIRB, ioDirB);
        writeMCP23017(GPINTENB, gpIntEnB);
        writeMCP23017(INTCONB, intConB);
        writeMCP23017(DEFVALB, defValB);
    }
}

void MCP23017::deleteAllInterruptsOnPort(MCP_PORT port){
    if(port==A){
        gpIntEnA = B00000000;
        writeMCP23017(GPINTENA, gpIntEnA);
    }
    else if (port==B){
        gpIntEnB = B00000000;
        writeMCP23017(GPINTENB, gpIntEnB);
    }
}

void MCP23017::setPinPullUp(uint8_t pin, MCP_PORT port, uint8_t state){
    if(port==A){
        if(state==ON){
            gppuA |= (1<<pin);
        }
        else if(state==OFF){
            gppuA &= ~(1<<pin);
        }
        writeMCP23017(GPPUA, gppuA);
    }
    else if(port==B){
        if(state==ON){
            gppuB |= (1<<pin);
        }
        else if(state==OFF){
            gppuB &= ~(1<<pin);
        }
        writeMCP23017(GPPUB, gppuB);
    }
}   
        
void MCP23017::setPortPullUp(uint8_t val, MCP_PORT port){
    if(port==A){
        gppuA = val;
        writeMCP23017(GPPUA, gppuA);
    }
    else if(port==B){
        gppuB = val;
        writeMCP23017(GPPUB, gppuB);
    }
}

void MCP23017::setIntMirror(uint8_t state){
    if(state==ON){
        ioConA |= (1<<MIRROR);
        ioConB |= (1<<MIRROR);
    }
    if(state==OFF){
        ioConA &= ~(1<<MIRROR);
        ioConB &= ~(1<<MIRROR);
    }
    writeMCP23017(IOCONA, ioConA);
    writeMCP23017(IOCONB, ioConB);
}   

uint8_t MCP23017::getIntFlag(MCP_PORT port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017(INTFA);
    }
    else if (port==B){ 
        value = readMCP23017(INTFB);
    }
    return value;
}

bool MCP23017::getPin(uint8_t pin, MCP_PORT port){
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

uint8_t MCP23017::getPort(MCP_PORT port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017(GPIOA);
    }
    else if(port==B){
        value = readMCP23017(GPIOB);
    }
    return value;
}

uint8_t MCP23017::getIntCap(MCP_PORT port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017(INTCAPA);
    }
    else if (port==B){
        value = readMCP23017(INTCAPB);
    }
    return value;
}

void MCP23017::setI2C_Address(int addr){
    I2C_Address = addr;
}

void MCP23017::setResetPin(uint8_t rp){
    resetPin = rp;
}

void MCP23017::setIoCon(uint8_t val, MCP_PORT port){
    if(port==A){
        ioConA = val;
        writeMCP23017(IOCONA, ioConA);
    }
    else if (port==B){
        ioConB = val;
        writeMCP23017(IOCONB, ioConB);
    }
}

void MCP23017::setGpIntEn(uint8_t val, MCP_PORT port){
    if(port==A){
        gpIntEnA = val;
        writeMCP23017(GPINTENA, gpIntEnA);
    }
    else if (port==B){
        gpIntEnB = val;
        writeMCP23017(GPINTENB, gpIntEnB);  
    }
}

void MCP23017::setIntCon(uint8_t val, MCP_PORT port){
    if(port==A){
        intConA = val;
        writeMCP23017(INTCONA, intConA);
    }
    else if (port==B){
        ioConB = val;
        writeMCP23017(INTCONB, intConB);
    }
}

void MCP23017::setDefVal(uint8_t val, MCP_PORT port){
    if(port==A){
        defValA = val;
        writeMCP23017(DEFVALA, defValA);
    }
    else if (port==B){
        defValB = val;
        writeMCP23017(DEFVALB, defValB);    
    }
}

void MCP23017::writeMCP23017(uint8_t reg, uint8_t val){
    _wire->beginTransmission(I2C_Address);
    _wire->write(reg);
    _wire->write(val);
    _wire->endTransmission();
    //delay(1);
}

void MCP23017::writeMCP23017(uint8_t reg, uint8_t valA, uint8_t valB){
    _wire->beginTransmission(I2C_Address);
    _wire->write(reg);
    _wire->write(valA);
    _wire->write(valB);
    _wire->endTransmission();
}

uint8_t MCP23017::readMCP23017(uint8_t reg){
    uint8_t regVal;
    _wire->beginTransmission(I2C_Address);
    _wire->write(reg);
    _wire->endTransmission(false);
    _wire->requestFrom(I2C_Address, 1);
    regVal = _wire->read();
    return regVal;
}

