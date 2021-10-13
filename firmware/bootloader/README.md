## Bootloader

Programming the bootloader is only required if the bootloader becomes corrupted

### AVRDUDE and Bus Pirate

See:  
http://www.nongnu.org/avrdude/ 
http://dangerousprototypes.com/docs/Bus_Pirate_AVR_Programming

Example *(adjust port for operating system):*

    avrdude -c buspirate -P com2 -p t85 -U micronucleus-2.5-entry_jumper_pb0.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m

### AVRDUDE and USBasp:

See:  
https://www.fischl.de/usbasp/

    avrdude -c usbasp -p t85 -U micronucleus-2.5-entry_jumper_pb0.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m

### Connections:

| Programmer |   EdgeLED   |
|------------|-------------|
|  GND       | GND  (J3-2) |
|  +5V       | +5V  (J3-1) |
| CS / Reset | RST  (J2-1) |
|  MOSI      | MOSI (J2-3) |
|  MISO      | MISO (J3-3) |
|  CLK       | SCK  (J2-2) |

## Arduino IDE

To make use of the maximum available program space, change the size in ***boards.txt*** for the Digistump board. In Windows this is located in:

C:\Users\<username>\AppData\Local\Arduino15\packages\digistump\hardware\avr\1.6.7

Change:

    digispark-tiny.upload.maximum_size=6012

to (bootloader version 2.04):

    digispark-tiny.upload.maximum_size=6522

or (bootloader version 2.5):

    digispark-tiny.upload.maximum_size=6586

## Licence

The Micronucleus bootloader is released under the GPLv2 license.

https://github.com/micronucleus/

In addition the Micronucleus team request that derivitive products are added to their "Devices using Micronucleus" list. This has not been done because I have not yet offered an EdgeLED for sale.