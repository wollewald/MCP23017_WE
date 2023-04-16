# MCP23017_WE
This is a library for the MCP23017, MCP23S17, MCP23018 and MCP23S18 I/O Port Expander.

You'll find some examples including the wiring (examples folder) which 
should enable you to use the library. Furthermore I have added a list of commands.

I have implemented an option to use it with an ATtiny and the TineWireM library. 

In case you like this library it would be cool if you give it a star.

If you find bugs, please inform me!

<b>Important notice</b>:
In 2022 Microchip has unfortunately updated the design of the MCP23017. <b>GPA7 and GPB7 have lost their input function</b>:</BR>
![new_design_mcp23017](https://user-images.githubusercontent.com/41305162/232289151-890811c7-b6f1-40a1-af07-35e38afbcfbe.png) </BR>
You can only use these pins as output. There is no change to the MCP23S17, MCP23018 and MCP23S18. 
You find this information in latest datasheet:
https://www.microchip.com/en-us/product/mcp23017


Written by Wolfgang (Wolle) Ewald

http://wolles-elektronikkiste.de/portexpander-mcp23017 (German)

https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2 (English)
