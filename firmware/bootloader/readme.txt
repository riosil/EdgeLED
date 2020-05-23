Command line (Windows) to program the bootloader:



AVRDUDE and Bus Pirate V3 (adjust COM port):
  http://www.nongnu.org/avrdude/
  http://dangerousprototypes.com/docs/Bus_Pirate_AVR_Programming

avrdude -c buspirate -P com2 -p t85 -U micronucleus-2.04-entry_jumper_pb0.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m




AVRDUDE and USBasp:
  https://www.fischl.de/usbasp/

avrdude -c usbasp -p t85 -U micronucleus-2.04-entry_jumper_pb0.hex -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m





Connections:

Programmer  |  TinyUSB
-------------------------
  GND       |  GND  J3-2
  +5V       |  +5V  J3-1
  CS        |  RST  J2-1
  MOSI      |  MOSI J2-3
  MISO      |  DATA J3-3
  CLK       |  SCK  J2-2