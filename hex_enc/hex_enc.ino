#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define a !checkbit(PINE,4)
#define b !checkbit(PINE,5)
#define zero !checkbit(PING,5)

int count;
int HOLD;
void setup(){
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
Serial.begin(115200);
}

void loop(){
Serial.print(a);
Serial.print(" # ");
Serial.print(b);
Serial.print(" # ");
Serial.println(count);
if (a&b) HOLD = true;
if ((a>b)& HOLD){ 
  count--;
  HOLD = false;
}
if ((a<b)& HOLD){
  count++;
  HOLD = false;
}
if(zero) count=0;
}
