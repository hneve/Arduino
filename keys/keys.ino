#define b1 13
#define b2 12
#define b3 11
#define b4 10
#define b5 9




void setup(){
  pinMode(b1,INPUT_PULLUP);
  pinMode(b2,INPUT_PULLUP);
  pinMode(b3,INPUT_PULLUP);
  pinMode(b4,INPUT_PULLUP);
  pinMode(b5,INPUT_PULLUP);  
  //pinMode(13,OUTPUT);  
  digitalWrite(13,1);
  Serial.begin(9600);
}

void loop(){
  if(!digitalRead(b1)){ Serial.println("sel");}
  if(!digitalRead(b2)){ Serial.println("dec");}
  if(!digitalRead(b3)){ Serial.println("inc");}
  if(!digitalRead(b4)){ Serial.println("before");}
  if(!digitalRead(b5)){ Serial.println("NEXT");}
  delay(200);
  //digitalWrite(13,(!digitalRead(13)));
}
