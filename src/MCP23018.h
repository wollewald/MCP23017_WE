/*****************************************
This is a library for the MCP23017/MCP23S17/MCP23018/MCP23S18 I/O Port Expander.

This specific header file is just for introducing the class name MCP23018. It inherits 
everything from MCP23017, including the constructors. 

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

LICENSE: MIT (https://github.com/wollewald/MCP23017_WE/blob/master/LICENSE)

In case you like it, it would be cool if you give it a star.

If you find bugs, don't hesitate to raise an issue!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2 (English)
https://wolles-elektronikkiste.de/portexpander-mcp23017       (German)

*******************************************/

#ifndef MCP23018_H
#define MCP23018_H
#include <MCP23017.h>

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class MCP23018 : public MCP23017 {
    public:
        using MCP23017::MCP23017;               
};

#endif
