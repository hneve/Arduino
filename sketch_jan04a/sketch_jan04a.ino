#define SIGNAL_STOP   2
#define SIGNAL_START  3
#define SIGNAL_BREKK  4
#define OUT_START     10
#define OUT_GLODING   11
#define OUT_TENNING   12
#define OUT_LYS       13


void setup() {
pinMode(SIGNAL_STOP, INPUT_PULLUP);   //Stopp
pinMode(SIGNAL_START, INPUT_PULLUP);  //Start
pinMode(SIGNAL_BREKK, INPUT_PULLUP);  //Håndbrekk
pinMode(OUT_START, OUTPUT);           //Start
pinMode(OUT_GLODING, OUTPUT);         //Gløding
pinMode(OUT_TENNING, OUTPUT);         //Tenning
pinMode(OUT_LYS, OUTPUT);             //Lys

Serial.begin(9600);
}

void loop() {
        
int sensorVal = digitalRead(SIGNAL_START);  //read the pushbutton value into a variable
Serial.println(sensorVal);          //print out the value of the pushbutton
int sensorVal1 = digitalRead(SIGNAL_BREKK);
Serial.println(sensorVal1);         //print out the value of the pushbutton
int sensorVal2 = digitalRead(2);
Serial.println(sensorVal);          //print out the value of the pushbutton


digitalWrite(OUT_START, HIGH);
digitalWrite(OUT_GLODING, HIGH);
digitalWrite(OUT_TENNING, HIGH);
digitalWrite(OUT_LYS, HIGH);

if (sensorVal2 == LOW) {
digitalWrite(OUT_START, HIGH);
digitalWrite(OUT_GLODING, HIGH);
digitalWrite(OUT_TENNING, HIGH);
digitalWrite(OUT_LYS, HIGH);
} else {


}

if (sensorVal1 == LOW && sensorVal == LOW) {
digitalWrite(OUT_GLODING, LOW);
delay(5000);
digitalWrite(OUT_TENNING, LOW);
delay(2000);
digitalWrite(OUT_START, LOW);
delay(1000);
digitalWrite(OUT_START, HIGH);
delay(1000);
digitalWrite(OUT_GLODING, HIGH);
delay(2000);
} else {


}
}


