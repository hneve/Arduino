int red = 11;
int grn = 12;
int blu = 13;
int x = 1;
void setup() {                
  pinMode(red, OUTPUT);     
  pinMode(blu, OUTPUT); 
  pinMode(grn, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(grn, LOW);
  digitalWrite(blu, LOW);
}

void LED(int f) {
  if (f == 1) { digitalWrite(red, HIGH); }
  if (f == 2) { digitalWrite(blu, HIGH); }
  if (f == 3) { digitalWrite(grn, HIGH); } 
  if (f == 4) { digitalWrite(grn, HIGH); digitalWrite(red, HIGH);} 
  if (f == 0) {
      digitalWrite(red, LOW);
      digitalWrite(grn, LOW);
      digitalWrite(blu, LOW);
  }
}    

void loop() {

LED(x);
delay(500);
LED(0); 
delay(500);
x++;
if (x == 5) { x = 1;}
}
  
