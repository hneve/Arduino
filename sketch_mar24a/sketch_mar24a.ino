#include <avr/interrupt.h>

#define    MAX_SPEED 75
#define   MIN_SPEED 
uint16_t targetSpeed = 300;
uint8_t accSpeed = 1;


void setup() {
DDRB |= (1 << 1); // Set as output
DDRB |= (1 << 0); // Set as output

TCCR1B |= (1 << WGM12); // Configure timer 1 for CTC mode
TCCR1A |= (1 << COM1A0); // Enable timer 1 Compare Output channel A in toggle mode
TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt
sei(); //  Enable global interrupts
OCR1A   = targetSpeed; // Set CTC compare value 
TCCR1B |= ((1 << CS10) | (1 << CS11)); // Start timer at Fcpu/64

}


void loop() {
  // put your main code here, to run repeatedly:

}

ISR(TIMER1_COMPA_vect){
  uint16_t nowSpeed = OCR1A;
  if(nowSpeed > targetSpeed) OCR1A -= accSpeed;
  if(nowSpeed < targetSpeed) OCR1A += accSpeed;
  
  //if(nowSpeed == 75) targetSpeed = 500;
  //if(nowSpeed == 500) targetSpeed = 75;
  
  //PORTB ^= (1 << 0); // Toggle the LED
  //if (OCR1A > 200) OCR1A -= 10;
}

