#include <FastLED.h>

#define LED_PIN     3
#define COLOR_ORDER GRB
#define CHIPSET     WS2811
#define NUM_LEDS    30

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 60
#define COOLING  55
#define SPARKING 120
#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

// How many seconds to show each temperature before switching
#define DISPLAYTIME 20
// How many seconds to show black between switches
#define BLACKTIME   3

bool gReverseDirection = false;

CRGBPalette16 gPal;

CRGB leds[NUM_LEDS];

void setup_fastled() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  gPal = HeatColors_p;
  FastLED.setBrightness(BRIGHTNESS);
}

void led1() {
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(255, 0, 0);
  }
  FastLED.show();
}

void led2() {
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(0, 0, 255);
  }
  FastLED.show();
}

void ledlock() {
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(0, 255, 0);
  }
  FastLED.show();
}

void unknown() {
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(178, 102, 255);
  }
  FastLED.show();
}

void web_mode(){
  for (int i = 0; i < NUM_LEDS; i++ )
  {
    leds[i] = CRGB(255, 128, 0);
  }
  FastLED.show();
}

void Fire2012WithPalette()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 1; k >= 2; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    // Scale the heat value from 0-255 down to 0-240
    // for best results with color palettes.
    byte colorindex = scale8( heat[j], 240);
    CRGB color = ColorFromPalette( gPal, colorindex);
    int pixelnumber;
    if ( gReverseDirection ) {
      pixelnumber = (NUM_LEDS - 1) - j;
    } else {
      pixelnumber = j;
    }
    leds[pixelnumber] = color;
  }
}

void colortemp() {
  static uint8_t starthue = 0;
  fill_rainbow( leds + 5, NUM_LEDS - 5, --starthue, 20);

  // Choose which 'color temperature' profile to enable.
  uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
  if ( secs < DISPLAYTIME) {
    FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
    leds[0] = TEMPERATURE_1; // show indicator pixel
  } else {
    FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
    leds[0] = TEMPERATURE_2; // show indicator pixel
  }

  // Black out the LEDs for a few secnds between color changes
  // to let the eyes and brains adjust
  if ( (secs % DISPLAYTIME) < BLACKTIME) {
    memset8( leds, 0, NUM_LEDS * sizeof(CRGB));
  }

  FastLED.show();
  FastLED.delay(8);
}
