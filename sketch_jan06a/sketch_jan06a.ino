#define stp 8
#define dir 9
#define ROUND 400*32

int counter = ROUND;
void setup() {
  pinMode(stp,OUTPUT);
  pinMode(dir,OUTPUT);
  digitalWrite(7,HIGH);

}

void loop() {
  digitalWrite(dir,LOW);
  if(!digitalRead(7)) counter=ROUND;
  
    while(counter){
      digitalWrite(stp,HIGH);
      //delayMicroseconds(500);
      digitalWrite(stp,LOW);
      delayMicroseconds(200);
      counter--;
    }


}
