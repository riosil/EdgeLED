#include <FastLED.h>
FASTLED_USING_NAMESPACE

// FastLED code modified from 'DemoReel100' example
// by Mark Kriegsman, December 2014.
//
// Modified by Steve Carey Dec 2021 for EdgeLED controller.
// Released under GNU GENERAL PUBLIC LICENSE version 3 (GPLv3)
// http://www.gnu.org/licenses/gpl-3.0.html

// Check number of LEDs (NUM_LEDS), bootloader version (1.xx or 2.xx),
// and whether phototransistor is fitted in the defines below,
// Bootloader version is diplayed in the progress window just
// before program upload.

// Required for v2.xx bootloaders to stop "Unknown device" pop-up in
// Windows if the user program does not have USB functionality itself, 
// not required for v1.11 since user program USB is disabled by default.
// For version 1.11 bootloader, comment out the next line.
#define BOOTLOADER_VER_2

// Is SFH3710 phototransistor fitted? Comment out the next line. if no
// phototransistor is fitted.
#define PHOTOTRANSISTOR

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    1          // pin for LED data
#define LED_TYPE    WS2812B    // the latest Neopixels and similar
#define COLOR_ORDER GRB        // the latest Neopixels and similar
#define NUM_LEDS    6          // set number of LEDs in strip here
#define FRAMES_PER_SECOND 100  // set 100 or 120 (2x mains freq), not critical
#define BRIGHTNESS_MIN     16  // adjust to suit taste
#define BRIGHTNESS_MAX    255  // adjust to suit taste

uint8_t brightness =  int((BRIGHTNESS_MIN + BRIGHTNESS_MAX)/2); // initial brightness
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
SimplePatternList gPatterns = { flash, pastels, red, orange, green, blue, white };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0;                  // rotating "base color" used by many of the patterns


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
      fill_solid( leds, NUM_LEDS, CRGB::Black);  // feedback - blink LEDs
      FastLED.show();
      FastLED.delay(600);
      nextPattern();
    }
    gHue++;
    #ifdef PHOTOTRANSISTOR
    // Dim the display if there is negligible ambient light.
    // Uses a SFH3710 phototransistor (emiter-GND, collector-SCK),
    // the pull-up (20-50kΩ) is provided by the microcontroller.
    // The phototransistor is not sensitive enough for continuous
    // brightness adjustness at low ambient light levels - it simply
    // detects whether there is any significant ambient light and
    // makes the display smoothly dim or brighten over approx 25s.
    // This low level code is very space efficient - just 56 bytes!
    ADCSRA |= (1 << ADSC);         // start ADC measurement
    while (ADCSRA & (1 << ADSC) ); // wait till conversion complete
    // If light intensity is above a threshold, brightness ramps up,
    // one step every 100ms. Note the value in ADCH is inversely
    // proportional to light intensity. Threshold range is typically
    // 230-254 since this basic circuit is not very sensitive. The
    // long (25s) time constant averages out detector noise.
    if (ADCH > 244) {
      brightness -= 1;
      if (brightness < BRIGHTNESS_MIN) brightness = BRIGHTNESS_MIN;
    }
    else {
      if (brightness >= BRIGHTNESS_MAX) brightness = BRIGHTNESS_MAX - 1;
      brightness += 1;   // this ensures brightness never rolls over 255
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

void flash() {
  if ((gHue & B00001111) > 7) {
    fill_solid( leds, NUM_LEDS, CRGB::Red);
    for (int i=1; i <= NUM_LEDS; i=i+2) {
      leds[i] = CRGB::Blue;
    }
  }
  else {
    fill_solid( leds, NUM_LEDS, CRGB::Blue);
    for (int i=1; i <= NUM_LEDS; i=i+2) {
      leds[i] = CRGB::Red;
    }
  }
}

void pastels() {
  fill_solid( leds, NUM_LEDS, CHSV(gHue, 160, 255));
}

void red() {
  fill_solid( leds, NUM_LEDS, CRGB::Red);
}

void orange() {
  //fill_solid( leds, NUM_LEDS, CRGB::Orange);   // looks like yellow?
  fill_solid( leds, NUM_LEDS, CHSV(16,255,255)); // try this one instead.
}

void green() {
  fill_solid( leds, NUM_LEDS, CRGB::Green);
}

void blue() {
  fill_solid( leds, NUM_LEDS, CRGB::Blue);
}

void white() {
  fill_solid( leds, NUM_LEDS, CRGB::White);
}
