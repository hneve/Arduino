#define MATRIX_NOOP	    0
#define MATRIX_DIGIT0       1
#define MATRIX_DIGIT1       2
#define MATRIX_DIGIT2       3
#define MATRIX_DIGIT3       4
#define MATRIX_DIGIT4       5
#define MATRIX_DIGIT5       6
#define MATRIX_DIGIT6       7
#define MATRIX_DIGIT7       8
#define MATRIX_DECODEMODE   9
#define MATRIX_INTENSITY   10
#define MATRIX_SCANLIMIT   11
#define MATRIX_SHUTDOWN    12
#define MATRIX_DISPLAYTEST 15
#define select_spi_device PORTB &= ~(1<<PORTB2)
#define deselect_spi_device PORTB |= (1<<PORTB2)


void spi_init(uint8_t clock, uint8_t mode )
{
	
	
	SPCR |= clock;		//set clock speed
	SPCR |= mode;		// set mode
	SPCR |= _BV(MSTR);	// Set as master 
	SPCR |= _BV(SPE);	// ENABLE SPI
	
	DDRB |= (1<<PB3)|(1<<PB5);		// set as output MOSI - SCK
}

uint8_t spi_send(uint8_t tdata)
{
	SPDR = tdata;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void led_sendbyte(uint8_t m_adr,uint8_t m_data)
{
	select_spi_device;
	spi_send(m_adr);
	spi_send(m_data);
	deselect_spi_device;
}

void setup()
{
  // put your setup code here, to run once:
  DDRB = DDRB |= (1<<PORTB2);
  spi_init(0,0);
  uint8_t MATRIX_SETUP_ARRAY[] = {MATRIX_DISPLAYTEST, MATRIX_SCANLIMIT, MATRIX_DECODEMODE, MATRIX_SHUTDOWN, MATRIX_INTENSITY};
  uint8_t MATRIX_SETUP_VALUE[] = {0x00,0x07,0xff,0x01,0x01};
			
  for(uint8_t _mod_line = 1 ; _mod_line < 6 ; _mod_line++){  // Prepare to send first line to NUM_MODULES modules
    select_spi_device;
    for(uint8_t _module = 1 ; _module ; _module--){
    spi_send(MATRIX_SETUP_ARRAY[_mod_line-1]);
    spi_send(MATRIX_SETUP_VALUE[_mod_line-1]);
    }
  deselect_spi_device;
  }
  for(int x = 1 ; x < 9 ; x++){
    led_sendbyte(x,0xf);
  }
  
}
int loops;
void loop()
{
  // put your main code here, to run repeatedly:
  for(int x = 1 ; x < 9 ; x++){
    led_sendbyte(x,loops);
  }
    
  loops++;
  if(loops > 9) loops = 0;
  //while(1);
  delay(300);
}
