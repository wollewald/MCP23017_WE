#include "MCP23017.h"

MCP23017::MCP23017(int addr, byte rp) // I2C Adresse und Reset Pin (Arduino)  
	: I2C_Address(addr), resetPin(rp){}

void MCP23017::Init(){
	pinMode(resetPin, OUTPUT); 
	digitalWrite(resetPin,LOW);
	delay(10);
	digitalWrite(resetPin, HIGH);
	setGpioOut(B00000000, A);
	setGpioOut(B00000000,B);
	setGpIntEn(B00000000,A);
	setGpIntEn(B00000000,B);
	setIoCon(B00000000, A);
	setIoCon(B00000000, B);
	setIntCon(B00000000, A);
	setIntCon(B00000000, B);
	setDefVal(B00000000, A);
	setDefVal(B00000000, B);
};
	
void MCP23017::setI2C_Address(int addr){
	I2C_Address = addr;
}

void MCP23017::setResetPin(byte rp){
	resetPin = rp;
}

void MCP23017::setIoDir(byte val, PORT port){
	if(port==A){
		ioDirA = val;
		writeMCP23017(IODIRA, ioDirA);
	}
	else if(port==B){
		ioDirB = val;
		writeMCP23017(IODIRB, ioDirB);
	}
}

void MCP23017::setGpioOut(byte val, PORT port){
	if(port==A){
		ioDirA = ~val;
		gpioA = val;
		writeMCP23017(IODIRA, ioDirA);
		writeMCP23017(GPIOA, gpioA);
	}
	else if(port==B){
		ioDirB = ~val;
		gpioB = val;
		writeMCP23017(IODIRB, ioDirB);
		writeMCP23017(GPIOB, gpioB);
	}
}

void MCP23017::setGpioOut(byte valA, byte valB, PORT portA, PORT portB){
	ioDirA = ~valA;
	ioDirB = ~valB;
	gpioA = valA;
	gpioB = valB;
	writeMCP23017(IODIRA, ioDirA, ioDirB);
	writeMCP23017(GPIOA, gpioA, gpioB);
	
}

void MCP23017::setGpioInOut(byte iodirval, byte gpioval, PORT port){
	if(port==A){
		ioDirA = iodirval;
		gpioA = gpioval;
		writeMCP23017(IODIRA, ioDirA);
		writeMCP23017(GPIOA, gpioA);
	}
	else if(port==B){
		ioDirB = iodirval;
		gpioB = gpioval;
		writeMCP23017(IODIRB, ioDirB);
		writeMCP23017(GPIOB, gpioB);
	}
}

void MCP23017::setGpIntEn(byte val, PORT port){
	if(port==A){
		gpIntEnA = val;
		writeMCP23017(GPINTENA, gpIntEnA);
	}
	else if (port==B){
		gpIntEnB = val;
		writeMCP23017(GPINTENB, gpIntEnB);	
	}
}

void MCP23017::setIoCon(byte val, PORT port){
	if(port==A){
		ioConA = val;
		writeMCP23017(IOCONA, ioConA);
	}
	else if (port==B){
		ioConB = val;
		writeMCP23017(IOCONB, ioConB);
	}
}

void MCP23017::setIntCon(byte val, PORT port){
	if(port==A){
		intConA = val;
		writeMCP23017(INTCONA, intConA);
	}
	else if (port==B){
		ioConB = val;
		writeMCP23017(INTCONB, intConB);
	}
}

void MCP23017::setDefVal(byte val, PORT port){
	if(port==A){
		defValA = val;
		writeMCP23017(DEFVALA, defValA);
	}
	else if (port==B){
		defValB = val;
		writeMCP23017(DEFVALB, defValB);	
	}
}

void MCP23017::switchPin(byte pin, PORT port, STATE state){
	if(port==A){
		if(state==ON){
			ioDirA &= ~(1<<pin);
			gpioA |= (1<<pin); 
		}
		else if(state==OFF){
			ioDirA |= (1<<pin);
			gpioA &= ~(1<<pin); 
		}
		writeMCP23017(IODIRA, ioDirA);
	    writeMCP23017(GPIOA, gpioA);
	}
	if(port==B){
		if(state==ON){
			ioDirB &= ~(1<<pin);
			gpioB |= (1<<pin); 
		}
		else if(state==OFF){
			ioDirB |= (1<<pin);
			gpioB &= ~(1<<pin); 
		}
		writeMCP23017(IODIRB, ioDirB);
	    writeMCP23017(GPIOB, gpioB);
	}
}

void MCP23017::togglePin(byte pin, PORT port){
	if(port==A){
		if(((gpioA) & (1<<pin))==0){
			ioDirA &= ~(1<<pin);
			gpioA |= (1<<pin); 
		}
		else if(((gpioA) & (1<<pin)) >= 1){
			ioDirA |= (1<<pin);
			gpioA &= ~(1<<pin); 
		}
		setIoDir(ioDirA, A);
	    setGpioOut(gpioA, A);
	}
	if(port==B){
		if(((gpioB) & (1<<pin))==0){
			ioDirB &= ~(1<<pin);
			gpioB |= (1<<pin); 
		}
		else if(((gpioB) & (1<<pin)) >= 1){
			ioDirB |= (1<<pin);
			gpioB &= ~(1<<pin); 
		}
		writeMCP23017(IODIRB, ioDirB);
	    writeMCP23017(GPIOB, gpioB);
	}
}

void MCP23017::setInterruptOnChangePin(byte pin, PORT port){
	if(port==A){
		ioDirA |= (1<<pin); 
		gpioA &= ~(1<<pin);
		gpIntEnA |= (1<<pin);
		writeMCP23017(IODIRA, ioDirA);
	    writeMCP23017(GPIOA, gpioA);
	    writeMCP23017(GPINTENA, gpIntEnA);
	}
	else if (port==B){
		ioDirB |= (1<<pin); 
		gpioB &= ~(1<<pin);
		gpIntEnB |= (1<<pin);
		writeMCP23017(IODIRB, ioDirB);
	    writeMCP23017(GPIOB, gpioB);
	    writeMCP23017(GPINTENB, gpIntEnB);
	}
}

void MCP23017::setInterruptOnDefValDev(byte pin, PORT port, STATE intState){
	if(port==A){
		ioDirA |= (1<<pin); 
		gpioA &= ~(1<<pin);
		gpIntEnA |= (1<<pin);
		intConA |= (1<<pin);
		if(intState==ON) defValA &= ~(1<<pin);
		else if(intState=OFF) defValA |= (1<<pin); 
		writeMCP23017(IODIRA, ioDirA);
	    writeMCP23017(GPIOA, gpioA);
	    writeMCP23017(GPINTENA, gpIntEnA);
		writeMCP23017(INTCONA, intConA);
		writeMCP23017(DEFVALA, defValA);
	}
	else if (port==B){
		ioDirB |= (1<<pin); 
		gpioB &= ~(1<<pin);
		gpIntEnB |= (1<<pin);
		intConB |= (1<<pin);
		if(intState==ON) defValB &= ~(1<<pin);
		else if(intState=OFF) defValB |= (1<<pin); 
		writeMCP23017(IODIRB, ioDirB);
	    writeMCP23017(GPIOB, gpioB);
	    writeMCP23017(GPINTENB, gpIntEnB);
		writeMCP23017(INTCONB, intConB);
		writeMCP23017(DEFVALB, defValB);
	}
}

void MCP23017::setInterruptOnChangePort(byte val, PORT port){
	if(port==A){
		ioDirA = val;
		gpioA = B00000000;
		gpIntEnA = val;
		writeMCP23017(IODIRA, ioDirA);
	    writeMCP23017(GPIOA, gpioA);
	    writeMCP23017(GPINTENA, gpIntEnA);
	}
	else if (port==B){
		ioDirB = val;
		gpioB = B00000000;
		gpIntEnB = val;
		writeMCP23017(IODIRB, ioDirB);
	    writeMCP23017(GPIOB, gpioB);
	    writeMCP23017(GPINTENB, gpIntEnB);
	}
}

void MCP23017::deleteAllInterruptsOnPort(PORT port){
	if(port==A){
		gpIntEnA = B00000000;
		writeMCP23017(GPINTENA, gpIntEnA);
	}
	else if (port==B){
		gpIntEnB = B00000000;
		writeMCP23017(GPINTENB, gpIntEnB);
	}
}

byte MCP23017::getIntFlag(PORT port){
	if(port==A) return readMCP23017(INTFA);
	else if (port==B) return readMCP23017(INTFB);
}

byte MCP23017::getGpio(PORT port){
	if(port==A) return readMCP23017(GPIOA);
	else if (port==B) return readMCP23017(GPIOB);
}

byte MCP23017::getIntCap(PORT port){
	if(port==A) return readMCP23017(INTCAPA);
	else if (port==B) return readMCP23017(INTCAPB);
}


void MCP23017::writeMCP23017(byte reg, byte val){
	Wire.beginTransmission(I2C_Address);
	Wire.write(reg);
	Wire.write(val);
	Wire.endTransmission();
	delay(5);
}

void MCP23017::writeMCP23017(byte reg, byte valA, byte valB){
	Wire.beginTransmission(I2C_Address);
	Wire.write(reg);
	Wire.write(valA);
	Wire.write(valB);
	Wire.endTransmission();
	delay(5);
}

byte MCP23017::readMCP23017(byte reg){
	byte regVal;
	Wire.beginTransmission(I2C_Address);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(I2C_Address, 1);
	regVal = Wire.read();
	delay(5);
	return regVal;
}

