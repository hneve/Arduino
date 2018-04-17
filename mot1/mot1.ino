#define btnL 12
#define btnH 11
#define MOT_1 A1
#define MOT_2  A0
#define imp_inp 2
volatile long impulses = 0;
volatile bool dir=false;

void setup() {
pinMode(btnL,INPUT_PULLUP);
pinMode(btnH,INPUT_PULLUP);
pinMode(imp_inp,INPUT_PULLUP);
pinMode(MOT_1,OUTPUT);
pinMode(MOT_2,OUTPUT);

attachInterrupt(0,imp_count,RISING);
digitalWrite(MOT_1,LOW);
digitalWrite(MOT_2,LOW);

Serial.begin(9600);
}

void loop() {

  if(!digitalRead(btnL)) {
    go_in(7);
  }

  if(digitalRead(btnH)) {
    go_out(92);
  }

Serial.println(impulses);

}

void go_out(long n){
  while( n > impulses) {
    dir=true;
    digitalWrite(MOT_2,LOW);
    digitalWrite(MOT_1,HIGH);
  }
  digitalWrite(MOT_1,LOW);
  digitalWrite(MOT_2,LOW);
}

void go_in(long n){
  while( n < impulses) {
    dir=false;
    digitalWrite(MOT_1,LOW);
    digitalWrite(MOT_2,HIGH);
  }
  digitalWrite(MOT_1,LOW);
  digitalWrite(MOT_2,LOW);
}
void imp_count() {
  if(dir){
    impulses++;
  }
  else {
    impulses--;
  }
}

