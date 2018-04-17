#include <SPI.h>
const int  RTC_cs=8; //chip select 

void setup() {
  Serial.begin(9600);
  RTC_init();

 
}

void loop() {
  Serial.println(readRTC());
  delay(1000);
    if (Serial.available() > 0) {
      int inByte = Serial.read();
      /*
       digitalWrite(RTC_cs, LOW);
		SPI.transfer(0x80); 
		SPI.transfer(7);        
		digitalWrite(RTC_cs, HIGH);*/
    //day(1-31), month(1-12), year(0-99), hour(0-23), minute(0-59), second(0-59)
  SetTimeDate(17,06,14,00,10,00); 
        
    }
  
}
//=====================================
int RTC_init(){ 
	  pinMode(RTC_cs,OUTPUT); // chip select
	  // start the SPI library:
	  SPI.begin();
	  SPI.setBitOrder(MSBFIRST); 
	  SPI.setDataMode(SPI_MODE3); // both mode 1 & 3 should work 
	  //set control register 
	  digitalWrite(RTC_cs, LOW);  
	  SPI.transfer(0x8E);
	  SPI.transfer(0x60); //60= disable Osciallator and Battery SQ wave @1hz, temp compensation, Alarms disabled
	  digitalWrite(RTC_cs, HIGH);
	  delay(10);
}
//=====================================
int SetTimeDate(int d, int mo, int y, int h, int mi, int s){ 
	int TimeDate [7]={s,mi,h,0,d,mo,y};
	for(int i=0; i<=6;i++){
		if(i==3){  i++;}
			
		int b= TimeDate[i]/10;
		int a= TimeDate[i]-b*10;
		if(i==2){
			if (b==2)
				b=B00000010;
			else if (b==1)
				b=B00000001;
		}	
		TimeDate[i]= a+(b<<4);
		  
		digitalWrite(RTC_cs, LOW);
		SPI.transfer(i+0x80); 
		SPI.transfer(TimeDate[i]);        
		digitalWrite(RTC_cs, HIGH);
  }
}
//=====================================
String ReadTimeDate(){
	String temp;
	int TimeDate [7]; //second,minute,hour,null,day,month,year		
	for(int i=0; i<=6;i++){
		if(i==3)
			i++;
		digitalWrite(RTC_cs, LOW);
		SPI.transfer(i+0x00); 
		unsigned int n = SPI.transfer(0x00);        
		digitalWrite(RTC_cs, HIGH);
		int a=n & B00001111;    
		if(i==2){	
			int b=(n & B00110000)>>4; //24 hour mode
			if(b==B00000010)
				b=20;        
			else if(b==B00000001)
				b=10;
			TimeDate[i]=a+b;
		}
		else if(i==4){
			int b=(n & B00110000)>>4;
			TimeDate[i]=a+b*10;
		}
		else if(i==5){
			int b=(n & B00010000)>>4;
			TimeDate[i]=a+b*10;
		}
		else if(i==6){
			int b=(n & B11110000)>>4;
			TimeDate[i]=a+b*10;
		}
		else{	
			int b=(n & B01110000)>>4;
			TimeDate[i]=a+b*10;	
			}
	}
	temp.concat(TimeDate[4]);
	temp.concat("/") ;
	temp.concat(TimeDate[5]);
	temp.concat("/") ;
	temp.concat(TimeDate[6]);
	temp.concat("     ") ;
	temp.concat(TimeDate[2]);
	temp.concat(":") ;
	temp.concat(TimeDate[1]);
	temp.concat(":") ;
	temp.concat(TimeDate[0]);
  return(temp);
}

String readRTC(){
  int TimeDate [7]; //second,minute,hour,null,day,month,year
  char temp[10];
  
  digitalWrite(RTC_cs, LOW);
  SPI.transfer(0x00); 
  for(int x=0 ; x <= 6 ; x++){ 
    TimeDate[x] = SPI.transfer(0x00);
    TimeDate[x] = (TimeDate[x] & 0x0f) + ((TimeDate[x] >> 4) * 10);     
  }


  digitalWrite(RTC_cs, HIGH);
  
 /* TimeDate[0] = (TimeDate[0] & 0x0f) + ((TimeDate[0] >> 4) * 10); 
  TimeDate[1] = (TimeDate[1] & 0x0f) + ((TimeDate[1] >> 4) * 10); 
  TimeDate[2] = (TimeDate[2] & 0x0f) + ((TimeDate[2] >> 4) * 10); */

  sprintf(temp, "%02u:%02u:%02u %u %02u/%02u/20%02u ",TimeDate[2],TimeDate[1],TimeDate[0],TimeDate[3],TimeDate[4],TimeDate[5],TimeDate[6]);
  
  return(temp);
}
