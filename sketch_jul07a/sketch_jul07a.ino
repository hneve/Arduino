int echoPin = 16;  // digital pin to receive echo pulse
int triggerPin = 15;  // digital pin to send trigger pulse
unsigned long distance = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(echoPin, INPUT);
  pinMode(triggerPin, OUTPUT);
}

void loop()
{
  digitalWrite(triggerPin, HIGH); // set HIGH for 15us to trigger ranging
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);  // set pin LOW
  distance = pulseIn(echoPin, HIGH);  // read in pulse length
  distance = distance / 29; // calculate distance from pulse length
  Serial.println(distance);
  delay(50);  // wait 50ms for next ranging
}
