## Bootloader Programming

Programming the bootloader is only required if the bootloader becomes corrupted or a new feature is required. The V2.x upload executable is backwards compatible with the V1.x bootloader.

| Bootloader | size (bytes) | Program space (bytes) |
|------------|--------------|-----------------------|
| v1.11      | 1850 | 6330 |
| v2.04      | 1622 | 6522 |
| v2.5       | 1540 | 6586 |
| v2.6       | 1542 | 6586 |

### AVRDUDE with USBasp:

See:  
https://www.fischl.de/usbasp/

    avrdude -c usbasp -p t85 -U micronucleus-2.6-entry_jumper_pb0.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m

### Connections:

| Programmer | EdgeLED v1 |  v2  |
|------------|------------|------|
|  GND       | GND  J3-2  | J2-6 |
|  +5V       | +5V  J3-1  | J2-2 |
| CS / Reset | RST  J2-1  | J2-5 |
|  MOSI      | MOSI J2-3  | J2-4 |
|  MISO      | MISO J3-3  | J2-1 |
|  CLK       | SCK  J2-2  | J2-3 |

EdgeLED V2 uses the common 6-pin ISCP pinout (J2).

### AVRDUDE with Bus Pirate

See:  
http://www.nongnu.org/avrdude/

http://dangerousprototypes.com/docs/Bus_Pirate_AVR_Programming

Example *(adjust port for operating system):*

    avrdude -c buspirate -P com2 -p t85 -U micronucleus-2.6-entry_jumper_pb0.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m

#### Check available space

To make use of the maximum available program space, change the size in ***boards.txt*** for the Digistump board. In Windows this is located in:

C:\Users\<username>\AppData\Local\Arduino15\packages\digistump\hardware\avr\1.6.7

Change:

    digispark-tiny.upload.maximum_size=6012

to (bootloader version 2.04):

    digispark-tiny.upload.maximum_size=6522

or (bootloader versions 2.5 - 2.6):

    digispark-tiny.upload.maximum_size=6586

Available program space is always an integer multiple of the page size, 64 bytes.

## Licence

The Micronucleus bootloader is released under the GPLv2 license which is in turn based on OBJECTIVE DEVELOPMENT GmbH's V-USB driver, also GPLv2.

https://github.com/micronucleus/  
https://www.obdev.at/products/vusb/index.html

The Micronucleus team request that derivitive products are added to their "Devices using Micronucleus" list. This has not been done because EdgeLED has not been offerred for sale.