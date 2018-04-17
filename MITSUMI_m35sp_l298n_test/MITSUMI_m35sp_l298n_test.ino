
#define CW 2
#define CCW 3

#define ENA 6
#define ENB 7

#define black 8  // In1
#define brown 9  // In2
#define orange 10  // In3
#define yellow 11  // In4

int stepplace = 0;
// bits defined to Brown - Black - Orange - Yellow
int stepcode[4] = {
  0b0110,
  0b0101,
  0b1001,
  0b1010};



void setup()
{
  DDRB = 0x3f; // Digital pins 8-13 output
  PORTB = 0x00; // all outputs Dp8-13 set to off

  pinMode(CW, INPUT_PULLUP);
  pinMode(CCW, INPUT_PULLUP);
}

void loop()
{
  if (!digitalRead(CW))  RUN(0,1,1);
  if (!digitalRead(CCW)) {
  RUN(0,(96*7),1);
  delay(500);
  RUN(1,(96*7),1);
  delay(500);
  }
}


void RUN(int dir, int steps, int stepdelay) {
// Enable H-bridges A & B
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  stepdelay++;  // Stepdelay should not be less than one

  while(steps){
    steps--;
    if(dir) stepplace++;
    else  stepplace--;
    if(stepplace == 4) stepplace = 0;
    if(stepplace == -1) stepplace= 3;

    PORTB = stepcode[stepplace];

    delay(stepdelay);
  }

  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}



