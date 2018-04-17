#define DIR 2
#define STP 3
#define ENA 4
#define DELAY 1
#define DMS 30
int adelay;
long turns;
void setup() {
  Serial.begin(115200);
DDRD = 0xff;
digitalWrite(13,HIGH);
bitSet(PORTD,2); // set direction
}

void loop() {
adelay = analogRead(0);
bitClear(PORTD,4); // enable module
/*
for (int x=0 ; x < 35 ; x++){
  bitSet(PORTD,3);
  delayMicroseconds(DMS);
  //delay(DELAY);
  bitClear(PORTD,3);
  delayMicroseconds(DMS);
  //delay(DELAY);
  turns++;
  }
 */


  bitSet(PORTD,3);
  delayMicroseconds(adelay);
  //delay(DELAY);
  bitClear(PORTD,3);
  //delayMicroseconds(adelay);

  while(!digitalRead(13)){
    bitSet(PORTD,4);
    };

}
