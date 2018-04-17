/*  
 *   Demo Sketch for the AD9851 Board and Arduino UNO
 *   11.09.2015, V1.0, Alexander C. Frank
 */
 
unsigned long DDS_CLOCK = 29995000 ;
unsigned long FREQ = 20000000 ;
unsigned long FTW ;
unsigned long REF_MUL = 0x0000 ;
 
// THIS IS FOR VERSION 2
int DATA = 9;
int CLOCK = 7; 
int UPDATE = 8; 
 
// ///////////////////////////////////////////////////////////// 
 
void UseMitDaemBitstaengeli(unsigned long data) 
{
unsigned long pointer = 0b00000000000000000000000000000001 ;
for (int i=0; i<32; i++)
  {
    if ((data & pointer)>0) { digitalWrite(DATA, HIGH); }
      else { digitalWrite(DATA, LOW); }
    delay(1);
    digitalWrite(CLOCK, HIGH);
    delay(1);
    digitalWrite(CLOCK, LOW);
    pointer = pointer << 1 ;
    delay(1);
  }
 
pointer = 0b00000000000000000000000000000001 ;
 
for (int i=0; i<8; i++)
  {
    if ((REF_MUL & pointer)>0) { digitalWrite(DATA, HIGH); }
      else { digitalWrite(DATA, LOW); }
    delay(1);
    digitalWrite(CLOCK, HIGH);
    delay(1);
    digitalWrite(CLOCK, LOW);
    pointer = pointer << 1 ;
    delay(1);
  }
delay(1);
digitalWrite(UPDATE, HIGH);
delay(1);
digitalWrite(UPDATE, LOW);
}
 
// ///////////////////////////////////////////////////////////// 
 
void setup() 
{
  // DEFINE OUTPUTs
  pinMode (DATA, OUTPUT);
  pinMode (CLOCK, OUTPUT);
  pinMode (UPDATE, OUTPUT);
  // THE BITSTREAM TO BE SEND
  FTW = (FREQ * pow(2, 24)) / ( DDS_CLOCK/256 ) ;
  // PROGRAM IT !
  UseMitDaemBitstaengeli(FTW);
  // THAT's IT.
}
 
void loop() 
{
  // DO NOTHING :-)
}
