#include <avr/interrupt.h>
#include <avr/io.h>
#include <arduino.h>

int counter;
void init(){
  pinMode(2,OUTPUT);
  pinMode(14,OUTPUT);
  
  TCCR1B |= (1 << WGM12);                 // Configure timer 1 for CTC mode
  
  TIMSK1 |= (1 << OCIE1A);                 // Enable CTC interrupt

  sei();                                  //  Enable global interrupts

  OCR1A   = 250;                        // Set CTC compare value to 1Hz at 1MHz AVR clock, with a prescaler of 64

//  TCCR1B |= ((1 << CS10) | (1 << CS11));  // Start timer at Fcpu/1024
  
}

void startTimer(){
  TCCR1B |= ((1 << CS10) | (1 << CS11));  // Start timer at Fcpu/1024
}

void stopTimer(){
  TCCR1B &= ~((1 << CS10) | (1 << CS11));
}

void setTimerDelay(int del){
cli();
OCR1A = del;
sei();
}


  ISR(TIMER1_COMPA_vect)
{
   PORTB ^= (1 << 0); 
   if (PORTB & 1) counter++;
}
