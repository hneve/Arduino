void setup() {
pinMode(4,OUTPUT);

}

int Lval;
int intervall;

void loop() {
Lval = analogRead(3);
if (Lval > 190){
  intervall++;
}
else{
  intervall = 0;
}
delay(8000);

if (intervall > 10){
  digitalWrite(4,HIGH);
}
else{
  digitalWrite(4,LOW);
}

}
