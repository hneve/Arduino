/* 
MY ROBOT 1


*/

#define forward  1
#define bakward  0


const int pwm_r_pin = 3;
const int pwm_l_pin = 11;
const int dir_r_pin = 12;
const int dir_l_pin = 13;
// ?????
const int brk_r_pin = 9;
const int brk_l_pin = 8;

int old_r_speed = 0;
int old_l_speed = 0;


void setup()
{
pinMode(3,OUTPUT);
pinMode(11,OUTPUT);
pinMode(12,OUTPUT);
pinMode(13,OUTPUT);
pinMode(9,OUTPUT);
pinMode(8,OUTPUT);
pinMode(2,INPUT_PULLUP);
while(xz digitalRead(2));
  
}






void loop()
{
digitalWrite(dir_r_pin, forward);
digitalWrite(dir_l_pin, forward);

for(int i = 0 ; i != 255 ; i++)
{
    analogWrite(pwm_r_pin, i);
    analogWrite(pwm_l_pin, i);
    delay(50);
}
  
delay(10000);

analogWrite(pwm_r_pin, 0);
analogWrite(pwm_l_pin, 0);

//while(1);


analogWrite(pwm_r_pin, 200);
analogWrite(pwm_l_pin, 200);

delay(2000);

analogWrite(pwm_r_pin, 0);
analogWrite(pwm_l_pin, 0);

delay(2000);

analogWrite(pwm_r_pin, 200);
analogWrite(pwm_l_pin, 200);

delay(2000);

analogWrite(pwm_r_pin, 0);
analogWrite(pwm_l_pin, 0);


delay(2000);


while(1);
}
