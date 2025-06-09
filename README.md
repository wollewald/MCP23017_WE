# MCP23017_WE
This is a library for the MCP23017, MCP23S17, MCP23018 and MCP23S18 I/O Port Expander.

You'll find some examples including the wiring (examples folder) which 
should enable you to use the library. Furthermore I have added a list of commands.

I have implemented an option to use it with an ATtiny and the TineWireM library. 

In case you like this library it would be cool if you give it a star.

If you find bugs, please inform me!

<b>Important notice</b>:
In 2022 Microchip has changed the data sheet of the MCP23017. <b>GPA7 and GPB7 have lost their input function</b>:</BR>
![new_design_mcp23017](https://user-images.githubusercontent.com/41305162/232289151-890811c7-b6f1-40a1-af07-35e38afbcfbe.png) </BR>
According to the data sheet you should only use these pins as output. It appears that Microchip has responded to issues encountered in practice. I, personally, have not exerienced any issue with the input function of GPA7 or GPB7. 

You find this information in latest datasheet:

https://www.microchip.com/en-us/product/mcp23017

And here you find a statement from Microchip regarding the use of the GPA7/B7 as input pins:

https://microchip.my.site.com/s/article/GPA7---GPB7-Cannot-Be-Used-as-Inputs-In-MCP23017

There is no change to the MCP23S17, MCP23018 and MCP23S18.


You can find more information about my library here:

http://wolles-elektronikkiste.de/portexpander-mcp23017 (German)

https://wolles-elektronikkiste.de/en/port-expander-mcp23017-2 (English)
