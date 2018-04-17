//=====================================
int RTC_init(){ //Test...
	//TEST.
	  pinMode(cs,OUTPUT); // chip select
	  // start the SPI library:
	  SPI.begin();
	  SPI.setBitOrder(MSBFIRST); 
	  SPI.setDataMode(SPI_MODE1); // both mode 1 & 3 should work 
	  //set control register 
	  digitalWrite(cs, LOW);  
	  SPI.transfer(0x8E);
	  SPI.transfer(0x60); //60= disable Osciallator and Battery SQ wave @1hz, temp compensation, Alarms disabled
	  digitalWrite(cs, HIGH);
	  delay(10);
}
//=====================================
int SetTimeDate(int d, int mo, int y, int h, int mi, int s){ 
	int TimeDate [7]={s,mi,h,0,d,mo,y};
	for(int i=0; i<=6;i++){
		if(i==3)
			i++;
		int b= TimeDate[i]/10;
		int a= TimeDate[i]-b*10;
		if(i==2){
			if (b==2)
				b=B00000010;
			else if (b==1)
				b=B00000001;
		}	
		TimeDate[i]= a+(b<<4);
		  
		digitalWrite(cs, LOW);
		SPI.transfer(i+0x80); 
		SPI.transfer(TimeDate[i]);        
		digitalWrite(cs, HIGH);
  }
}
//=====================================
String ReadTime(){
	String temp;
	int TimeDate [7]; //second,minute,hour,null,day,month,year		
	for(int i=0; i<=6;i++){
		if(i==3)
			i++;
		digitalWrite(cs, LOW);
		SPI.transfer(i+0x00); 
		unsigned int n = SPI.transfer(0x00);        
		digitalWrite(cs, HIGH);
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
	//temp.concat(TimeDate[4]);
	//temp.concat("/") ;
	//temp.concat(TimeDate[5]);
	//temp.concat("/") ;
	//temp.concat(TimeDate[6]);
	temp.concat("    ") ;
        if(TimeDate[2] < 10) temp.concat("0") ;
	temp.concat(TimeDate[2]);
	temp.concat(":") ;
        if(TimeDate[1] < 10) temp.concat("0") ;
	temp.concat(TimeDate[1]);
	temp.concat(":") ;
        if(TimeDate[0] < 10) temp.concat("0") ;
	temp.concat(TimeDate[0]);
  return(temp);
}

String ReadDate(){
	String temp;
	int TimeDate [7]; //second,minute,hour,null,day,month,year
	for(int i=0; i<=6;i++){
		if(i==3)
		i++;
		digitalWrite(cs, LOW);
		SPI.transfer(i+0x00);
		unsigned int n = SPI.transfer(0x00);
		digitalWrite(cs, HIGH);
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
	if(TimeDate[4] < 10) temp.concat("0") ;
	temp.concat(TimeDate[4]);
	temp.concat(".") ;
	if(TimeDate[5] < 10) temp.concat("0") ;
	temp.concat(TimeDate[5]);
	temp.concat(".20") ;
	if(TimeDate[6] < 10) temp.concat("0") ;
	temp.concat(TimeDate[6]);
//	temp.concat("    ") ;
// 	if(TimeDate[2] < 10) temp.concat("0") ;
// 	temp.concat(TimeDate[2]);
// 	temp.concat(":") ;
// 	if(TimeDate[1] < 10) temp.concat("0") ;
// 	temp.concat(TimeDate[1]);
// 	temp.concat(":") ;
// 	if(TimeDate[0] < 10) temp.concat("0") ;
// 	temp.concat(TimeDate[0]);
	return(temp);
}

int get_hour(){
	digitalWrite(cs, LOW);
	SPI.transfer(0x02);
	unsigned int hour = SPI.transfer(0x00);
	digitalWrite(cs, HIGH);
	if(hour & 0b00010000) hour = 10 + (hour & 0b00001111);
	if(hour & 0b00100000) hour = 20 + (hour & 0b00001111);
	
	return hour;
}

int get_minute(){
	digitalWrite(cs, LOW);
	SPI.transfer(0x01);
	unsigned int minute = SPI.transfer(0x00);
	digitalWrite(cs, HIGH);
	int a = (minute >> 4) * 10 ;
	int b = minute & 0b00001111;
	
	return a+b;
}
int get_seconds(){
	digitalWrite(cs, LOW);
	SPI.transfer(0x00);
	unsigned int second = SPI.transfer(0x00);
	digitalWrite(cs, HIGH);
	int a = (second >> 4) * 10 ;
	int b = second & 0b00001111;
	
	return a+b;
}