#define upPin 2
#define dnPin 3
#define redLed 11

int volt;
int tid;

void setup()
{
Serial.begin(9600);
pinMode(upPin, INPUT_PULLUP); //Trykkbryter, øker lysstyrken
pinMode(dnPin, INPUT_PULLUP);
pinMode(redLed, OUTPUT);
}

void loop()
{
if(!digitalRead(upPin)) { //knappen har blitt trykket på og pinnen er derfor 0 (siden du bruker pullup) 
	delay(10);	// må bremse dette litt 
	tid++;
	if(tid > 50) { //sjekke om knappen er holdt inne eller bare trykket kort
		if(volt <= 255) { volt++; } // vil ikke gå høyere enn 255
		tid = 0;
	}
}

if(!digitalRead(dnPin)) { //knappen har blitt trykket på og pinnen er derfor 0 (siden du bruker pullup) 
	delay(10);	// må bremse dette litt 
	tid++;
	if(tid > 50) { //sjekke om knappen er holdt inne eller bare trykket kort
		if(volt >= 1) { volt--; } // vil ikke gå høyere enn 255
		tid = 0;
	}
}

Serial.println(volt);
analogWrite(redLed,volt);
}
