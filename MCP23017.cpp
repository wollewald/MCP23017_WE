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

#include "MCP23017.h"

MCP23017::MCP23017(int addr, byte rp) // I2C Adresse und Reset Pin (Arduino)  
	: I2C_Address(addr), resetPin(rp){}

void MCP23017::Init(){
	pinMode(resetPin, OUTPUT); 
	reset();
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

void MCP23017::setPinMode(byte pin, PORT port, byte state){
		if(port==A){
			if(state==ON){
				ioDirA &= ~(1<<pin);
			}
			else if(state==OFF){
				ioDirA |= (1<<pin);
			}
			writeMCP23017(IODIRA, ioDirA);
		}
		else if(port==B){
			if(state==ON){
				ioDirB &= ~(1<<pin);
			}
			else if(state==OFF){
				ioDirB |= (1<<pin);
			}
			writeMCP23017(IODIRB, ioDirB);
		}		
}

void MCP23017::setPortMode(byte val, PORT port){
	if(port==A){
		ioDirA = ~val;
		writeMCP23017(IODIRA, ioDirA);
	}
	else if(port==B){
		ioDirB = ~val;
		writeMCP23017(IODIRB, ioDirB);
	}
}

void MCP23017::setPin(byte pin, PORT port, byte state){
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

void MCP23017::togglePin(byte pin, PORT port){
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

void MCP23017::setPinX(byte pin, PORT port, byte ioDir, byte state){
	if(port==A){
		if(ioDir==OUTPUT){
			ioDirA &= ~(1<<pin);
		}
		else if(ioDir==INPUT){
			ioDirA |= (1<<pin);
		}
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
		if(ioDir==OUTPUT){
			ioDirB &= ~(1<<pin);
		}
		else if(ioDir==INPUT){
			ioDirB |= (1<<pin);
		}
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

void MCP23017::setAllPins(PORT port, byte state){
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

void MCP23017::setPort(byte val, PORT port){
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

void MCP23017::setPort(byte valA, byte valB, PORT portA, PORT portB){
	gpioA = valA;
	gpioB = valB;
	writeMCP23017(IODIRA, ioDirA, ioDirB);
	writeMCP23017(GPIOA, gpioA, gpioB);
}

void MCP23017::setPortX(byte iodirval, byte gpioval, PORT port){
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

void MCP23017::setInterruptPinPol(byte state){
	if(state==HIGH){
		ioConA |= (1<<INTPOL);
		ioConB |= (1<<INTPOL);
	}
	if(state==LOW){
		ioConA |= (0<<INTPOL);
		ioConB |= (0<<INTPOL);
	}
	writeMCP23017(IOCONA, ioConA);
	writeMCP23017(IOCONB, ioConB);
}	

void MCP23017::setIntOdr(byte state){
	if(state==ON){
		ioConA |= (1<<INTODR);
		ioConB |= (1<<INTODR);
	}
	if(state==OFF){
		ioConA |= (0<<INTODR);
		ioConB |= (0<<INTODR);
	}
	writeMCP23017(IOCONA, ioConA);
	writeMCP23017(IOCONB, ioConB);
}	

void MCP23017::setInterruptOnChangePin(byte pin, PORT port){
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

void MCP23017::setInterruptOnDefValDevPin(byte pin, PORT port, byte intState){
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

void MCP23017::setInterruptOnChangePort(byte val, PORT port){
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

void MCP23017::setInterruptOnDefValDevPort(byte val, PORT port, byte state){
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

void MCP23017::setPinPullUp(byte pin, PORT port, byte state){
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
		
void MCP23017::setPortPullUp(byte val, PORT port){
	if(port==A){
		gppuA = val;
		writeMCP23017(GPPUA, gppuA);
	}
	else if(port==B){
		gppuB = val;
		writeMCP23017(GPPUB, gppuB);
	}
}

void MCP23017::setIntMirror(byte state){
	if(state==ON){
		ioConA |= (1<<MIRROR);
		ioConB |= (1<<MIRROR);
	}
	if(state==OFF){
		ioConA |= (0<<MIRROR);
		ioConB |= (0<<MIRROR);
	}
	writeMCP23017(IOCONA, ioConA);
	writeMCP23017(IOCONB, ioConB);
}	

byte MCP23017::getIntFlag(PORT port){
	if(port==A) return readMCP23017(INTFA);
	else if (port==B) return readMCP23017(INTFB);
}

bool MCP23017::getPin(byte pin, PORT port){
	byte result = 0;
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

byte MCP23017::getPort(PORT port){
	if(port==A) return readMCP23017(GPIOA);
	else if (port==B) return readMCP23017(GPIOB);
}

byte MCP23017::getIntCap(PORT port){
	if(port==A) return readMCP23017(INTCAPA);
	else if (port==B) return readMCP23017(INTCAPB);
}

void MCP23017::setI2C_Address(int addr){
	I2C_Address = addr;
}

void MCP23017::setResetPin(byte rp){
	resetPin = rp;
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

void MCP23017::writeMCP23017(byte reg, byte val){
	Wire.beginTransmission(I2C_Address);
	Wire.write(reg);
	Wire.write(val);
	Wire.endTransmission();
	delay(1);
}

void MCP23017::writeMCP23017(byte reg, byte valA, byte valB){
	Wire.beginTransmission(I2C_Address);
	Wire.write(reg);
	Wire.write(valA);
	Wire.write(valB);
	Wire.endTransmission();
	delay(1);
}

byte MCP23017::readMCP23017(byte reg){
	byte regVal;
	Wire.beginTransmission(I2C_Address);
	Wire.write(reg);
	Wire.endTransmission();
	Wire.requestFrom(I2C_Address, 1);
	regVal = Wire.read();
	delay(1);
	return regVal;
}

