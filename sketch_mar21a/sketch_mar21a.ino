/* PULSER */
/* Making lib to send timed pulses */

int newTime;
int repTime = 1000;


void setup() {
  Serial.begin(115200);
  newTime = millis() + repTime;

}

void loop() {

if (millis() >= newTime){
  digitalWrite(2,HIGH);
  digitalWrite(2,LOW);
  newTime = millis() + repTime;
}

}
