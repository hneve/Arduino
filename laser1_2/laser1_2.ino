
/*###########################################################################
# Flash trigger SW 0.1
###########################################################################*/
// System variables:
int DelayTime = 0;  // Forsinkelse fra trig til flash
int toleranse = 5;  // Ekstra toleranse på selvkalibrering


/*###########################################################################
# Konstanter - pinnedeklarasjoner
###########################################################################*/
// Color LED's definition
const int red = 11;
const int grn = 12;
const int blu = 13;
// PIN DECLARATION
const int SensorInput = 14; 
const int SensorActivator = 9;
const int Button_1 = 2;
const int Button_2 = 3;
// GLOBAL VARIABLES
int low_level;
int high_level;
int level = 0;

/*###########################################################################
# SETUP
###########################################################################*/
void setup() {                
  pinMode(red, OUTPUT);     
  pinMode(blu, OUTPUT); 
  pinMode(grn, OUTPUT);
  pinMode(SensorActivator, OUTPUT);
  pinMode(Button_1, INPUT_PULLUP);
  pinMode(Button_2, INPUT_PULLUP);
  //Serial.begin(9600);
  digitalWrite(SensorActivator, LOW);
}
/*###########################################################################
# TRIPPEL LED ROUTINE
###########################################################################*/
void LED(boolean r = 0,boolean g = 0,boolean b = 0) {
  digitalWrite(red, LOW);
  digitalWrite(grn, LOW);
  digitalWrite(blu, LOW);
  
  if (r) { digitalWrite(red, HIGH); }
  if (g) { digitalWrite(grn, HIGH); } 
  if (b) { digitalWrite(blu, HIGH); }
}    

/*###########################################################################
# SENSORINPUT SELF CALIBRATION
###########################################################################*/
void calibrate(){                           
  level = analogRead(SensorInput);
  low_level = level;
  high_level = level;
  unsigned long stamp = millis() + 5000;
  LED(0,1,0);
  while (millis() < stamp) {
      level = analogRead(SensorInput);
      if (level < low_level) { low_level = level - toleranse;}
      if (level > high_level) { high_level = level + toleranse;}
  }
LED(0,0,0);
}

/*###########################################################################
# TRIGGER ROUTINE
###########################################################################*/
void trigger() {  
  digitalWrite(SensorActivator, HIGH);
  if (DelayTime != 0) {
          LED(1,0,0);
          delay(DelayTime);
  }
  LED(0,0,1);
  delay(100);
  digitalWrite(SensorActivator, LOW);
  LED(0,0,0);
  
  delay(2000);
}

/*###########################################################################
# RUN ROUTINE
###########################################################################*/
void run() {
LED(0,0,0);
while ((level > low_level) && (level < high_level)) {
    level = analogRead(SensorInput);
    }
trigger();
}

/*###########################################################################
# INFINITE SYSTEM LOOP
###########################################################################*/
void loop() {
level = analogRead(SensorInput);
if (level < low_level) {LED(0,1,0);} 
if (level > high_level) {LED(1,0,0);}
  else {LED(0,0,0);}       
if (!digitalRead(Button_1)){ calibrate(); }
if (!digitalRead(Button_2)){ run(); }
}

