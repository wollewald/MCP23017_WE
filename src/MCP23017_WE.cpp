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

#include "MCP23017_WE.h"

bool MCP23017_WE::Init(){
    if (useSPI) {
        pinMode(csPin, OUTPUT);
        digitalWrite(csPin, HIGH);
    }
    
    if(resetPin < 99){ 
        pinMode(resetPin, OUTPUT); 
        digitalWrite(resetPin, HIGH);
    }
    
    softReset();
    
    setIoCon(0b00001000, A); // enable SPI hardware address
    setIoCon(0b00001000, B); // enable SPI hardware address
    setIntCon(0b10101010, A);
    if(readMCP23017_WE(INTCONA) != 0b10101010){
        return false;
    }
    setIntCon(0, A);
    
#ifndef USE_TINY_WIRE_M_ 
    mySPISettings = SPISettings(8000000, MSBFIRST, SPI_MODE0); 
#endif
    return true;
};

void MCP23017_WE::reset(){
    digitalWrite(resetPin,LOW);
    delay(10);
    digitalWrite(resetPin, HIGH);
    delay(10);
}

void MCP23017_WE::setPinMode(uint8_t pin, mcp_port port, uint8_t pinState){
    uint8_t gppu = getPortPullUp(port);
    uint8_t ioDir = getIoDir(port);
    
    if(pinState==OUTPUT){
        ioDir |= (1<<pin);
        gppu &= ~(1<<pin);
    }
    else if(pinState==INPUT){
        ioDir &= ~(1<<pin);
        gppu &= ~(1<<pin);
    }
    else if(pinState==INPUT_PULLUP){
        ioDir &= ~(1<<pin);
        gppu |= (1<<pin);
    }
    
    setPortPullUp(gppu, port);
    setIoDir(ioDir, port);
}

void MCP23017_WE::setPortMode(uint8_t portState, mcp_port port){
    setIoDir(portState, port);
    if(port==A){
        writeMCP23017_WE(GPPUA, 0);
    }
    else if(port==B){
        writeMCP23017_WE(GPPUB, 0);
    }
}

uint8_t MCP23017_WE::getPortMode(mcp_port port){
    return getIoDir(port);
}

void MCP23017_WE::setPortMode(uint8_t portState, mcp_port port, uint8_t pu){
    uint8_t gppu = 0;
    uint8_t ioDir = 0;
    
    if(pu != INPUT_PULLUP){
        return;
    }
    if(port==A){
        ioDir = portState;
        gppu = ~portState;
    }
    else if(port==B){
        ioDir = portState;
        gppu = ~portState;
    }
    
    setPortPullUp(gppu, port);
    setIoDir(ioDir, port);
}

void MCP23017_WE::setPin(uint8_t pin, mcp_port port, uint8_t pinLevel){
    uint8_t olat = getOutputLatch(port);

    if(pinLevel==HIGH){
        olat |= (1<<pin); 
    }
    else if(pinLevel==LOW){
        olat &= ~(1<<pin); 
    }
    
    setOutputLatch(olat, port);
}

void MCP23017_WE::togglePin(uint8_t pin, mcp_port port){
    uint8_t olat = getOutputLatch(port);

    if(((olat) & (1<<pin))==0){
        olat |= (1<<pin); 
    }
    else if(((olat) & (1<<pin)) >= 1){
        olat &= ~(1<<pin); 
    }
  
    setOutputLatch(olat, port);
}

void MCP23017_WE::setPinX(uint8_t pin, mcp_port port, uint8_t pinState, uint8_t pinLevel){
    uint8_t gppu = getPortPullUp(port);
    uint8_t ioDir = getIoDir(port);
    uint8_t olat = getOutputLatch(port);

    if(pinState==OUTPUT){
        ioDir |= (1<<pin);
        gppu &= ~(1<<pin);
    }
    else if(pinState==INPUT){
        ioDir &= ~(1<<pin);
        gppu &= ~(1<<pin);
    }
    else if(pinState==INPUT_PULLUP){
        ioDir &= ~(1<<pin);
        gppu |= (1<<pin);
    }
    if(pinLevel==HIGH){
        olat |= (1<<pin); 
    }
    else if(pinLevel==LOW){
        olat &= ~(1<<pin); 
    }
 
    setOutputLatch(olat, port);
    setPortPullUp(gppu, port);
    setIoDir(ioDir, port);
}

void MCP23017_WE::setAllPins(mcp_port port, uint8_t pinLevel){
    uint8_t olat = 0;
    if(pinLevel==HIGH){
        olat = 0b11111111;
    }
    else if (pinLevel==LOW){
        olat = 0b00000000;
    }
    setOutputLatch(olat, port);
}

void MCP23017_WE::setPort(uint8_t olat, mcp_port port){
    setOutputLatch(olat, port);
}

uint8_t MCP23017_WE::getPort(mcp_port port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017_WE(MCP_GPIOA);
    }
    else if(port==B){
        value = readMCP23017_WE(MCP_GPIOB);
    }
    return value;
}

uint8_t MCP23017_WE::getOutputLatch(mcp_port port){
    uint8_t olat = 0;
    if(port==A){
        olat = readMCP23017_WE(OLATA);
    }
    else if(port==B){
        olat = readMCP23017_WE(OLATB);
    }
    return olat;
}   

void MCP23017_WE::setPort(uint8_t portLevelA, uint8_t portLevelB){
    writeMCP23017_WE(OLATA, portLevelA, portLevelB);
}

void MCP23017_WE::setPortX(uint8_t portState, uint8_t portLevel, mcp_port port){
    setOutputLatch(portLevel, port);
    setIoDir(portState, port);
}

void MCP23017_WE::setInterruptPinPol(uint8_t level){
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
    writeMCP23017_WE(IOCONA, ioConA);
    writeMCP23017_WE(IOCONB, ioConB);
}   

void MCP23017_WE::setIntOdr(uint8_t openDrain){
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
    writeMCP23017_WE(IOCONA, ioConA);
    writeMCP23017_WE(IOCONB, ioConB);
}   

void MCP23017_WE::setInterruptOnChangePin(uint8_t pin, mcp_port port){
    uint8_t gpIntEn = getGpIntEn(port);
    uint8_t ioDir = getIoDir(port);
    
    ioDir &= ~(1<<pin);
    gpIntEn |= (1<<pin);
    
    setIoDir(ioDir, port);
    setGpIntEn(gpIntEn, port);
}

void MCP23017_WE::setInterruptOnDefValDevPin(uint8_t pin, mcp_port port, uint8_t pinIntLevel){
    uint8_t gpIntEn = getGpIntEn(port);
    uint8_t intCon = getIntCon(port);
    uint8_t defVal = getDefVal(port);
    uint8_t ioDir = getIoDir(port);
    
    ioDir &= ~(1<<pin); 
    gpIntEn |= (1<<pin);
    intCon |= (1<<pin);
    if(pinIntLevel==HIGH){
        defVal |= (1<<pin);
    }
    else if(pinIntLevel==LOW){
        defVal &= ~(1<<pin);
    }
    
    setIoDir(ioDir, port);
    setGpIntEn(gpIntEn, port);
    setIntCon(intCon, port);
    setDefVal(defVal, port);
}

void MCP23017_WE::setInterruptOnChangePort(uint8_t intOnChangePins, mcp_port port){
    uint8_t ioDir = getIoDir(port);
    uint8_t gpIntEn = intOnChangePins;

    ioDir &= ~intOnChangePins;
        
    setIoDir(ioDir, port);
    setGpIntEn(gpIntEn, port);
}

void MCP23017_WE::setInterruptOnDefValDevPort(uint8_t intPins, mcp_port port, uint8_t defVal){
    uint8_t gpIntEn = getGpIntEn(port);
    uint8_t intCon = getIntCon(port);
    uint8_t ioDir = getIoDir(port);
    
    intCon |= intPins;
    gpIntEn |= intPins;
    ioDir &= ~intPins;
    
    setIoDir(ioDir, port);
    setGpIntEn(gpIntEn, port);
    setIntCon(intCon, port);
    setDefVal(defVal, port);
}

void MCP23017_WE::deleteAllInterruptsOnPort(mcp_port port){
    setGpIntEn(0, port);
}

void MCP23017_WE::setIpolPin(uint8_t pin, mcp_port port, uint8_t ipolState){
    uint8_t ipolPortState = getIpolPort(port);
    
    if(ipolState){
        ipolPortState |= (1<<pin);
    }
    else {
        ipolPortState &= ~(1<<pin);
    }
    
    setIpolPort(ipolPortState, port);
}   

void MCP23017_WE::setIpolPort(uint8_t ipol, mcp_port port){
    if(port==A){
        writeMCP23017_WE(IPOLA, ipol);
    }
    else if (port==B){
        writeMCP23017_WE(IPOLB, ipol);
    }
}

uint8_t MCP23017_WE::getIpolPort(mcp_port port){
    uint8_t ipolPort = 0;
    if(port==A){
        ipolPort = readMCP23017_WE(IPOLA);
    }
    else if(port==B){
        ipolPort = readMCP23017_WE(IPOLB);
    }
    return ipolPort;
}

void MCP23017_WE::setPinPullUp(uint8_t pin, mcp_port port, uint8_t pinLevel){
    uint8_t gppu = getPortPullUp(port);
    
    if(pinLevel==HIGH){
        gppu |= (1<<pin);
    }
    else if(pinLevel==LOW){
        gppu &= ~(1<<pin);
    }
        
    setPortPullUp(gppu, port);
}
        
void MCP23017_WE::setPortPullUp(uint8_t pulledUpPins, mcp_port port){
    if(port==A){
        writeMCP23017_WE(GPPUA, pulledUpPins);
    }
    else if(port==B){
        writeMCP23017_WE(GPPUB, pulledUpPins);
    }
}

uint8_t MCP23017_WE::getPortPullUp(mcp_port port){
    uint8_t gppu = 0;
    if(port==A){
        gppu = readMCP23017_WE(GPPUA);
    }
    else{ 
        gppu = readMCP23017_WE(GPPUB);
    }
    return gppu;
}      

void MCP23017_WE::setIntMirror(uint8_t mirrored){
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
    writeMCP23017_WE(IOCONA, ioConA);
    writeMCP23017_WE(IOCONB, ioConB);
}   

uint8_t MCP23017_WE::getIntFlag(mcp_port port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017_WE(INTFA);
    }
    else if (port==B){ 
        value = readMCP23017_WE(INTFB);
    }
    return value;
}

bool MCP23017_WE::getPin(uint8_t pin, mcp_port port){
    uint8_t result = 0;
    if(port==A){
        result = readMCP23017_WE(MCP_GPIOA);
    }
    else if(port==B){
        result = readMCP23017_WE(MCP_GPIOB);
    }
    result &= (1<<pin);
    if(result) return true;
    else return false;  
}

uint8_t MCP23017_WE::getIntCap(mcp_port port){
    uint8_t value = 0;
    if(port==A){
        value = readMCP23017_WE(INTCAPA);
    }
    else if (port==B){
        value = readMCP23017_WE(INTCAPB);
    }
    return value;
}

#ifndef USE_TINY_WIRE_M_
void MCP23017_WE::setSPIClockSpeed(unsigned long clock){
    mySPISettings = SPISettings(clock, MSBFIRST, SPI_MODE0);
}
#endif

void MCP23017_WE::softReset(){
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

#ifdef DEBUG_MCP23017_WE   // see MCP23017_WE_config.h
void MCP23017_WE::printAllRegisters(){
    uint8_t reg = 0;
    uint8_t regVal = 0;
    char buf[20] = {};
    
    Serial.println(F("Register status:"));
    
    regVal = readMCP23017_WE(reg); regVal = ~regVal; reg++;
    sprintf(buf, "IODIRA  : 0x%02X | 0b", regVal); 
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); regVal = ~regVal; reg++;
    sprintf(buf, "IODIRB  : 0x%02X | 0b", regVal); 
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "IPOLA   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "IPOLB   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "GPINTENA: 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "GPINTENB: 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "DEFVALA : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "DEFVALB : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "INTCONA : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "INTCONB : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "IOCONA  : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "IOCONB  : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "GPPUA   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "GPPUB   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "INTFA   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "INTFB   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "INTCAPA : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "INTCAPB : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "MCP_GPIOA   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "MCP_GPIOB   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "OLATA   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
    
    regVal = readMCP23017_WE(reg); reg++;
    sprintf(buf, "OLATB   : 0x%02X | 0b", regVal);
    Serial.print(buf); printBin(regVal);
}

void MCP23017_WE::printBin(uint8_t val){
    uint8_t valCopy = val;
    uint8_t proceedingZeros = 0;
    
    while(!(valCopy & 0x80) && proceedingZeros < 7){
        valCopy = valCopy << 1;
        proceedingZeros++;
    }
    
    for(int i=0; i<proceedingZeros; i++){
        Serial.print("0");
    }
    Serial.println(val, BIN);   
}
#endif // DEBUG_MCP23017_WE 


uint8_t MCP23017_WE::i2cConnectionError(){
    uint8_t error = 0;
    if(!useSPI){
#ifndef USE_TINY_WIRE_M_
        _wire->beginTransmission(I2C_Address);
        error = _wire->endTransmission();
#else
        TinyWireM.beginTransmission(I2C_Address);
        error = TinyWireM.endTransmission();    
#endif
    }
    return error;
}

/* Private Functions */

void MCP23017_WE::setResetPin(uint8_t rp){
    resetPin = rp;
}

void MCP23017_WE::setIoCon(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017_WE(IOCONA, val);
    }
    else if (port==B){
        writeMCP23017_WE(IOCONB, val);
    }
}

uint8_t MCP23017_WE::getIoCon(mcp_port port){
    if(port==A){
        return readMCP23017_WE(IOCONA);
    }
    else{
        return readMCP23017_WE(IOCONB);
    }
}


void MCP23017_WE::setGpIntEn(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017_WE(GPINTENA, val);
    }
    else if (port==B){
        writeMCP23017_WE(GPINTENB, val);  
    }
}


uint8_t MCP23017_WE::getGpIntEn(mcp_port port){
    if(port==A){
        return readMCP23017_WE(GPINTENA);
    }
    else{
        return readMCP23017_WE(GPINTENB);
    }
}


void MCP23017_WE::setIntCon(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017_WE(INTCONA, val);
    }
    else if (port==B){
        writeMCP23017_WE(INTCONB, val);
    }
}

uint8_t MCP23017_WE::getIntCon(mcp_port port){
    if(port==A){
        return readMCP23017_WE(INTCONA);
    }
    else{
        return readMCP23017_WE(INTCONB);
    }
}

void MCP23017_WE::setDefVal(uint8_t val, mcp_port port){
    if(port==A){
        writeMCP23017_WE(DEFVALA, val);
    }
    else if (port==B){
        writeMCP23017_WE(DEFVALB, val);    
    }
}

uint8_t MCP23017_WE::getDefVal(mcp_port port){
    if(port==A){
        return readMCP23017_WE(DEFVALA);
    }
    else{
        return readMCP23017_WE(DEFVALB);  
    }
}

void MCP23017_WE::setOutputLatch(uint8_t olat, mcp_port port){
    if(port==A){
        writeMCP23017_WE(OLATA, olat);
    }
    else if(port==B){
        writeMCP23017_WE(OLATB, olat);
    }
}

/* I changed the logic: 0 = INPUT, 1 = OUTPUT */
void MCP23017_WE::setIoDir(uint8_t ioDir, mcp_port port){
    if(port==A){
        writeMCP23017_WE(IODIRA, ~ioDir);
    }
    else if(port==B){
        writeMCP23017_WE(IODIRB, ~ioDir);
    }
}

/* I changed the logic: 0 = INPUT, 1 = OUTPUT */    
uint8_t MCP23017_WE::getIoDir(mcp_port port){
    uint8_t ioDir = 0;
    if(port==A){
        ioDir = readMCP23017_WE(IODIRA);
    }
    else if(port==B){
        ioDir = readMCP23017_WE(IODIRB);
    }
    return ~ioDir;
}   

void MCP23017_WE::writeMCP23017_WE(uint8_t reg, uint8_t val){
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

void MCP23017_WE::writeMCP23017_WE(uint8_t reg, uint8_t valA, uint8_t valB){
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

uint8_t MCP23017_WE::readMCP23017_WE(uint8_t reg){
    uint8_t regVal = 0;
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
    }
#endif
    return regVal;
}

