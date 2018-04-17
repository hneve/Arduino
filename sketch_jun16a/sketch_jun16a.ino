#define w_clk 8
#define fq_ud 9
#define reset 10
#define dataport PORTD

uint32_t Ftw =  50;
uint8_t  Phase = 1; 

void transmitToAD(uint8_t _setup, uint32_t _frekv){
  uint32_t DTS = ((_frekv * pow(2, 24)) / ( 17999e4/256 ));
  
  digitalWrite(fq_ud, LOW);
  PORTD = _setup;
  digitalWrite(w_clk,HIGH);digitalWrite(w_clk,LOW);
  PORTD = DTS >> 24;
  digitalWrite(w_clk,HIGH);digitalWrite(w_clk,LOW);
  PORTD = DTS >> 16;
  digitalWrite(w_clk,HIGH);digitalWrite(w_clk,LOW);
  PORTD = DTS >> 8;
  digitalWrite(w_clk,HIGH);digitalWrite(w_clk,LOW);
  PORTD = DTS;
  digitalWrite(w_clk,HIGH);digitalWrite(w_clk,LOW);
  digitalWrite(fq_ud, HIGH);digitalWrite(fq_ud, LOW);
}

void setup() {
  DDRD = 0xff; // Output
  pinMode(w_clk,OUTPUT);
  pinMode(fq_ud,OUTPUT);
  pinMode(reset,OUTPUT);
  pinMode(13,OUTPUT);
  digitalWrite(fq_ud,HIGH); // Active low
  digitalWrite(w_clk,LOW);  // Low to high edge
  digitalWrite(reset,HIGH);
  digitalWrite(reset,LOW);
  digitalWrite(13,HIGH);
  
  delay(3000);

  transmitToAD(Phase,Ftw);
}

void loop() {
  transmitToAD(Phase,880);
  delay(1000);
  transmitToAD(Phase,440);
  delay(1000);

}
