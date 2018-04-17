//DEFINE bit manipulation:
#define setbit(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define clearbit(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define flipbit(ADDRESS,BIT) (ADDRESS ^= (1<<BIT))
#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define setbitmask(x,y) (x |= (y))
#define clearbitmask(x,y) (x &= (~y))
#define flipbitmask(x,y) (x ^= (y))
#define checkbitmask(x,y) (x & (y))

//#define  !digitalRead(2) !checkbit(PORTB,2)

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 3



#define Blue  0x0000FF
#define Brown 0xA52A2A
#define Green  0x008000
#define Orange  0xFFA500
#define Purple  0x800080
#define Red  0xFF0000
#define Violet  0xEE82EE
#define Yellow  0xFFFF00
#define Indigo  0x4B0082

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(16, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


volatile int i = 0;
int demomode;
void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
//#if defined (__AVR_ATtiny85__)
//  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
//#endif
  // End of trinket special code


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  pinMode(2,INPUT);
  digitalWrite(2,HIGH);
  delay(50);
  if(!digitalRead(2)){ demomode = true; }
}

void loop() {
  
  if(1){ rainbow(200); }
 else{   
  
  if( !digitalRead(2)){ i++; delay(500);}
if(i > 6 ) i = 0;
  
  
  switch(i){
    case 0: colorWipe(Red, 50);    // Black/off
            if(demomode) i++;
            break;     
    case 1: colorWipe(Orange, 50);  // Red
            if(demomode) i++;
            break;
    case 2: colorWipe(Yellow, 50);  // Green
            if(demomode) i++;
            break;
    case 3: colorWipe(Green, 50);  // Blue
            if(demomode) i++;
            break;
    case 4: colorWipe(Blue, 50);  // Blue
            if(demomode) i++;
            break;
    case 5: colorWipe(Indigo, 50);  // Blue
            if(demomode) i++;
            break;
    case 6: colorWipe(Violet, 50);  // Blue
            if(demomode) i++;
            break;
    case 7: rainbow(50);
            if(demomode) i++;
            break;
    case 8: rainbowCycle(50);
            if(demomode) i++;
            break;
    case 9: theaterChaseRainbow(50);
            if(demomode) i=0;
            break;
  }
 }
  // Some example procedures showing how to display to the pixels:
//  colorWipe(strip.Color(255, 0, 0), 50); // Red
//  colorWipe(strip.Color(0, 255, 0), 50); // Green
  //colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127,   0,   0), 50); // Red
  //theaterChase(strip.Color(  0,   0, 127), 50); // Blue

// rainbow(20);
 //rainbowCycle(20);
  //theaterChaseRainbow(50);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
      //if( !digitalRead(2)){  goto bailout;}
  }
bailout: ;
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
      //if( !digitalRead(2)){   goto bailout; }
    }
    
    
    strip.show();
    delay(wait);
    
    
  }
bailout: ;
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      if( !digitalRead(2)){   goto bailout; }
    }
    strip.show();
    delay(wait);
    
  }
  bailout: ;
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
      if( !digitalRead(2)){  goto bailout; }
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
  bailout: ;
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
          if( !digitalRead(2)){  goto bailout; }
        }
        strip.show();
       
        delay(wait);
        
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
          if( !digitalRead(2)){  goto bailout; }
        }
    }
  }
  bailout: ;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

