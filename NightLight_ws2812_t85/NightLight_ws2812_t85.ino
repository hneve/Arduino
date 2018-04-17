#define Blue  0x0000FF
#define Brown 0xA52A2A
#define Green  0x008000
#define Orange  0xFFA500
#define Purple  0x800080
#define Red  0xFF0000
#define Violet  0xEE82EE
#define Yellow  0xFFFF00
#define Indigo  0x4B0082
#define Black 0x000000
#define White 0xffffff

#include <Adafruit_NeoPixel.h>
//#include <avr/power.h>

#define PIN 3
#define PIXELS 1
#define LIGHTLEVEL 255

uint8_t grayzone;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXELS, PIN, NEO_GRB + NEO_KHZ800);




void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop()
{
 // if(analogRead(3) < LIGHTLEVEL) rainbow(50);
  //else colorWipe(Black, 50);

  newRainbow(25);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((j) & 255));
    }
    
    //if(analogRead(3) > LIGHTLEVEL) return;
  
    strip.show();
    delay(wait);
        
  }
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

void newRainbow(int wait){
uint16_t cRed, cBlue, cGreen;
//  1
  cRed = 255;
  for (int i = 0 ; i != 255 ; i++){
    cGreen = i;
      for(byte z = 0 ; z <= 15 ; z++){
        strip.setPixelColor(z,strip.Color(cRed,cGreen,cBlue));
      }
    strip.show();
    delay(wait);
  }
//  2
   for (int i = 0 ; i != 255 ; i++){
    cRed = 255-i;
    for(byte z = 0 ; z <= 15 ; z++){
        strip.setPixelColor(z,strip.Color(cRed,cGreen,cBlue));
      }
    strip.show();
    delay(wait);
  }
// 3
  for (int i = 0 ; i != 255 ; i++){
    cBlue = i;
    for(byte z = 0 ; z <= 15 ; z++){
        strip.setPixelColor(z,strip.Color(cRed,cGreen,cBlue));
      }
    strip.show();
    delay(wait);
  }
// 4
   for (int i = 0 ; i != 255 ; i++){
    cGreen = 255-i;
    for(byte z = 0 ; z <= 15 ; z++){
        strip.setPixelColor(z,strip.Color(cRed,cGreen,cBlue));
      }
    strip.show();
    delay(wait);
  }
// 5
  for (int i = 0 ; i != 255 ; i++){
    cRed = i;
    for(byte z = 0 ; z <= 15 ; z++){
        strip.setPixelColor(z,strip.Color(cRed,cGreen,cBlue));
      }
    strip.show();
    delay(wait);
  }
// 6
   for (int i = 0 ; i != 255 ; i++){
    cBlue = 255-i;
    for(byte z = 0 ; z <= 15 ; z++){
        strip.setPixelColor(z,strip.Color(cRed,cGreen,cBlue));
      }
    strip.show();
    delay(wait);
  }

}


