#include <Adafruit_NeoPixel.h>

#define PIN 0
#define PIXELS 1
#define LIGHTLEVEL 200

const int wait=50;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup(){
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else if(WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    } 
    else {
      WheelPos -= 170;
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
}

void loop(){
    uint16_t i, j;
    for(j=0; j<256; j++) {
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((j) & 255));
      }
    strip.show();
    delay(wait);
    }
}


