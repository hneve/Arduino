/*
***** BLDC DRIVER *****
*/

int wait = 3000;
int p1 = 2;
int p2 = 3;
int p3 = 4;
char inChar;

void setup() { 
pinMode(p1, OUTPUT);
pinMode(p2, OUTPUT);
pinMode(p3, OUTPUT);
pinMode(8,OUTPUT);
digitalWrite(8,HIGH);
pinMode(9,OUTPUT);
digitalWrite(9,HIGH);
Serial.begin(115200);
}

// the loop routine runs over and over again forever:
void loop() {

if (Serial.available()){
//inChar = (char)Serial.read(); 
//if (inChar == '-'){
//wait -=1;
//}
//else{
//wait +=1;
//}
wait = Serial.parseInt();
Serial.println(wait);
}
if (wait > 400) wait -= 1;

Serial.println(wait);

digitalWrite(p1, 1); 
digitalWrite(p2, 1);
digitalWrite(p3, 0);
delayMicroseconds(wait); 
digitalWrite(p1, 1); 
digitalWrite(p2, 0);
digitalWrite(p3, 0);
delayMicroseconds(wait);
digitalWrite(p1, 1); 
digitalWrite(p2, 0);
digitalWrite(p3, 1);
delayMicroseconds(wait);
digitalWrite(p1, 0); 
digitalWrite(p2, 0);
digitalWrite(p3, 1);
delayMicroseconds(wait);
digitalWrite(p1, 0); 
digitalWrite(p2, 1);
digitalWrite(p3, 1);
delayMicroseconds(wait);
digitalWrite(p1, 0); 
digitalWrite(p2, 1);
digitalWrite(p3, 0);
delayMicroseconds(wait);
}
