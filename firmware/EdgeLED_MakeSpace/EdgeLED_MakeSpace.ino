#include <FastLED.h>
FASTLED_USING_NAMESPACE

// FastLED code modified from 'DemoReel100' example.
// -Mark Kriegsman, December 2014
// TWO ROWS OF 9 LEDs, USED FOR MAKESPACE SIGN
// Modified by Steve Carey June 2019 for Edge LED lighting demo.
// Each button press cycles the display from a pastel rainbow fade then
// then five fixed colours; red, orange, green, blue, white.
// Check number of LEDs (NUM_LEDS), bootloader version (1.xx or 2.xx),
// and whether phototransistor is fitted in the defines below,
// bootloader version is diplayed just before program upload.

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

// This is required for v2.xx bootloaders to stop "Unknown device" pop-up
// in Windows if the user program does not have USB functionality itself, 
// not required for v1.11 since user program USB is disabled by default.
// For version 1.11 bootloader, comment out the next line.
#define BOOTLOADER_VER_2

// Is SFH3710 phototransistor fitted?
#define PHOTOTRANSISTOR

#define DATA_PIN    1          // pin for LED data
#define LED_TYPE    WS2812B    // the latest Neopixels and similar
#define COLOR_ORDER GRB        // the latest Neopixels and similar
#define NUM_LEDS    18         // set number of LEDs in strip here
#define FRAMES_PER_SECOND 100  // set 100 or 120 (2x mains freq), not critical
#define BRIGHTNESS_MIN    20   // adjust to suit taste
#define BRIGHTNESS_MAX    254  // adjust to suit taste

uint8_t brightness = int((BRIGHTNESS_MIN + BRIGHTNESS_MAX)/2); // initial brightness
CRGB leds[NUM_LEDS];

void setup() {
  // Set pin modes and ADC setup
  // see: https://www.marcelpost.com/wiki/index.php/ATtiny85_ADC
  // using direct register access instead of pinMode() saves 98 bytes per PinMode().
  #ifdef BOOTLOADER_VER_2  // Disables USB enumeration. Only required for bootloader v2.0+
  DDRB |= (1 << PB3);    // PB3 is the USB D- PIN, set this to OUTPUT LOW to disable
  PORTB &= ~(1 << PB3);  // USB enumeration by a host - not per USB spec but it works!
  #endif
  DDRB &= ~(1 << PB0);     //  set DDR for pin PB0 to INPUT - used for the button
  PORTB |= (1 << PB0);     //  enable pullup for pin PB0
  DDRB &= ~(1 << PB2);     //  set DDR for pin PB2 to INPUT - used for light sensor
  PORTB |= (1 << PB2);     //  enable pullup for pin PB2
  ADMUX =   (1 << ADLAR) |     // left shift result
            (0 << REFS1) |     // Sets ref. voltage to VCC, bit 1
            (0 << REFS0) |     // Sets ref. voltage to VCC, bit 0
            (0 << MUX3)  |     // use ADC1 for input (PB2), MUX bit 3
            (0 << MUX2)  |     // use ADC1 for input (PB2), MUX bit 2
            (0 << MUX1)  |     // use ADC1 for input (PB2), MUX bit 1
            (1 << MUX0);       // use ADC1 for input (PB2), MUX bit 0
  ADCSRA =  (1 << ADEN)  |     // Enable ADC 
            (1 << ADPS2) |     // set prescaler to 128, bit 2 
            (1 << ADPS1) |     // set prescaler to 128, bit 1 
            (1 << ADPS0);      // set prescaler to 128, bit 0
            
  delay(500); // delay initial current peak
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(brightness);
}


// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { ramp, normal, flash };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gSat = 0;                  // variable colour saturation (the S in HSV space)
uint8_t gVal = 0;                  // variable brightness (the V in HSV space)
uint8_t newSat = 0;                 // scratchpad saturation value


void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS(100) {
    if (!digitalRead(PB0)) {
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      FastLED.delay(600);
      nextPattern();
    }
    gSat++;
    #ifdef PHOTOTRANSISTOR
    // Dim the display if there is negligible ambient light.
    // Uses a SFH3710 phototransistor (emiter-GND, collector-SCK),
    // the pull-up (20-50kΩ) is provided by the microcontroller.
    // The phototransistor is not sensitive enough for continuous
    // brightness adjustness at low ambient light levels - it simply
    // detects whether there is any significant ambient light and
    // makes the display smoothly dim or brighten over approx 10-20s.
    // This low level code is very space efficient - just 56 bytes!
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    // If light level is above a threshold, brightness ramps up, one
    // step every 100ms.
    if (ADCH > 254) {
      brightness -= 1;
      if (brightness <= BRIGHTNESS_MIN) brightness = BRIGHTNESS_MIN;
    }
    else {
      // If light level is below a threshold, brightness ramps down, one
      // step every 100ms.
      if (brightness >= BRIGHTNESS_MAX) brightness = (BRIGHTNESS_MAX-1);
      brightness += 1;
    }
    FastLED.setBrightness(brightness);
    #endif
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void normal() {
  frontGreen();
  backOrange();
}

void flash() {
  if ((gSat & B00001111) > 7) {
    frontOrange();
    backGreen();
  }
  else {
    frontGreen();
    backOrange();
  }
}

void ramp() {
  // Represent the growth of Makespace in light!
  // Note gSat increments by 1 every 100ms,
  // ramp up HSV Value to max in increments of 8
  // ramp up Saturation in increments of 4
  // until wrap around above gSat = 255
  if (gSat < 32) gVal = gSat * 8;
  else gVal = 255;
  if (gSat < 64) newSat = gSat * 4;
  else newSat = 255;
  // Set rear row, green
  for (int i = 0; i < 9; i++) {
    leds[i] = CHSV(12, newSat, gVal);
  }
  // Set front row, orange
  for (int i = 9; i < 18; i++) {
    leds[i] = CHSV(96 ,newSat, gVal);
  }
}

void frontGreen() {
  for (int i = 9; i < 18; i++) {
    leds[i] = CRGB::Green;
  }
}

void frontOrange() {
  for (int i = 9; i < 18; i++) {
    leds[i] = CHSV(12, 255, 255);
  }
}

void backGreen() {
  for (int i = 0; i < 9; i++) {
    leds[i] = CRGB::Green;
  }
}

void backOrange() {
  for (int i = 0; i < 9; i++) {
    leds[i] = CHSV(12, 255, 255);
  }
}
