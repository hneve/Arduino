void setup() {
 DDRB = 0xff;
}
int L;
void loop() {
  L= analogRead(0);
  if(L > 700){
    PORTB = 0xff;
    delay(100);
  }
  else{
    PORTB = 0;
  }
}
