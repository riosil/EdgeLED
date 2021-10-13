# EdgeLEDcontroller

Tiny controller for addressable LEDs to enable LED strips in tiny spaces - ideal for edge lighting laser cut acrylic signs.

![Example edge lit sign](https://raw.githubusercontent.com/riosil/EdgeLEDcontroller/master/images/keep_calm_sign.jpg)

This prototype was made in Makespace, Cambridge. There is an example two-layer display at the end of the video: https://www.instagram.com/p/CSuGmHmoO2C/

The base is made of three layers of laser cut plywood with a laser engraved channel at the bottom for the LED strip. M3 nuts are embedded between layers before bonding. [Slide binders](https://www.amazon.co.uk/gp/product/B00JPJA7QY) can also be used to clip LED strips onto the edge 5mm thick acrylic, ideal for hanging signs.

The LEDs are type WS2812B (aka Neopixels) or SK6812 LEDs in 3.5mm square packages. The controller is built into a USB plug and programmed with Arduino sketches through the USB. Having the controller built in the plug allows the lighting strip to fit in very slim spaces.

**Security:** by default the controller only takes power from the USB - it only requires power from a USB charger. If plugged into a computer, it will not enumerate as a USB device unless the on-board switch is held down as the plug is inserted. This tiny switch is hard to operate accidentally.

![Edge LED controller](https://raw.githubusercontent.com/riosil/EdgeLEDcontroller/master/images/usb_board.jpg)

## Programming

The controller uses a ATtiny85 programmed with the [Micronucleus](https://github.com/micronucleus/micronucleus.git) bootloader. The controller will only appear as a programmable device if the button (on PB0) is pressed as the controller is plugged into the USB host. Programs can then be loaded via USB using the Arduino IDE with board selection set to: "Digispark (Default - 16.5MHz)."

Install the board into the Arduino IDE by following these [instructions](http://digistump.com/wiki/digispark/tutorials/connecting) for the Digispark:


* File → Preferences → Additional Board Manager URLs, enter:
* http://digistump.com/package_digistump_index.json (separate multiple entries with commas)
* Tools → Board → Boards Manager, Filter on 'Digispark' then Install.
* Select the board: 'Digispark (Default - 16.5MHz)'.

The Digispark driver is not recommended (this uses version 1.02 of Micronucleus). Windows 10 users should follow the [Zadig driver installer guide](https://github.com/micronucleus/micronucleus/tree/master/windows_driver_installer). Linux and OS X users do not require custom drivers.

When creating Arduino sketches note the push button is on pin PB0 (active low) and the LED data is on pin PB1. To upload a sketch into the controller; click the Arduino IDE upload button, plug in the controller holding the push button down when this message appears (the button can be released after a second):

> Running Digispark Uploader...  
> Plug in device now... (will timeout in 60 seconds)

Space for user code is 6330 bytes (bootloader v1.11), 6522 bytes (v2.04) or 6586 bytes (v2.5). The version is displayed as part of the compiler output messages (except v2.04 is displayed as v2.4 because the bootloader does not handle leading zeros for the minor version). Consider adjusting the maximum upload size as per these instructions: [Changing the maximum sketch size of the Arduino IDE](https://gist.github.com/Ircama/22707e938e9c8f169d9fe187797a2a2c#user-content-changing-the-maximum-sketch-size-of-the-arduino-ide) to ensure all program space is available.

Want to program the bootloader but don't want to compile the code? Version 2.xx with PB0 entry jumper (the push button) is in the firmware folder.

**Note** if bootloader v2.xx is installed, assert PB3 low to stop USB enumeration messages. See the example sketch for details. Using bootloader 2.xx gives user programs access to USB but this is disabled in all the examples by asserting PB3 low in each sketch setup section.

The cable is 4-wire flexible telephone extension cord where the length is limited by transients but 3m has worked OK. It was found that using two ground wires does not significantly reduce transients. 3-wire cable is also acceptable.

![Flexible 5mm wide LED strip](https://raw.githubusercontent.com/riosil/EdgeLEDcontroller/master/images/led_flex_strip.jpg)

## Files

The example sketch *EdgeLEDDemo* uses the FadeLED library to smoothly cycle the LEDs through their range of hues. Each press of the on-board switch steps through the colours of the rainbow, back to the rainbow fade. Uses 4720 bytes (72%) of program storage space, 137 bytes of dynamic memory.

This example sketch for Makespace has [two rows of nine LEDs](https://raw.githubusercontent.com/riosil/EdgeLEDcontroller/master/images/sign_makespace.jpg) (LED zero nearest the cable entry). The default sequence rapidly fades up white light, then slowly increases colour saturation before going dark and starting over. Uses 4678 bytes (71%) of program storage space, 167 bytes of dynamic memory.

Controller schematic (pdf) and KiCAD files; connectors J2 and J3 are for programming the bootloader, J3 connects the LED strip. The optional SFH3710 phototransistor connection is between SCK and GND.

This flexible 4mm wide [LED strip](https://www.aliexpress.com/item/32889698964.html) from AliExpress is now the preferred strip. The connections are surprisingly secure when sealed with this adhesive lined [heat shrink tubing](https://www.aliexpress.com/item/32892500051.html) (4.8mm diameter) and a small piece of reinforcing plastic shim.

## Updates

* 6 June 2019 - received a batch of SFH3710 phototransistors from China. Simple mod enables auto dimming in the dark. See comments in the example sketch.

* 9 June 2019 - issues found with recent releases of the FastLED library; version 3.2.7 fails to compile for the ATtiny85, version 3.2.8 uses 48% more program memory for the example sketch. Last known working version of FastLED is 3.2.6. Fixed with release 3.2.9.

* 9 Feb 2020 - FastLED is broken again with version 3.3.3, use version 3.3.2 until a fix is released.

* 13 Oct 2021 - FastLED 3.4 works OK. Added Micronucleus bootloader v2.5. Minor tweak to the example code. 

---
*© Keep Calm styles, though used in the UK since 1939, may be copyright in the EEA/EU by Keep Calm and Carry On Ltd.*