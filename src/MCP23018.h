/*****************************************
This is a library for the MCP23017 and MCP23S17 I/O Port Expander

This specific header file is just for introducing the class name MCP23S17. It inherits 
everything from MCP23017, including the constructors. 

You'll find some examples including the wiring which should enable you to use the library. 
Furthermore I have added a list of commands.

You are free to use it, change it or build on it. In case you like it, it would be cool 
if you give it a star.

If you find bugs, please inform me!

Written by Wolfgang (Wolle) Ewald
https://wolles-elektronikkiste.de/port-expander-mcp23017-2?lang=en

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
