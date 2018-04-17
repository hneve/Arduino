#include <avr/io.h>
#include <avr/interrupt.h>
//#define F_CPU 250000UL
//#define F_CPU 8000000UL
#include <util/delay.h>

#define LCD_WR PD4
#define LCD_RD PD3
#define LCD_CS PD2
#define LCD_DATA PD5


volatile uint16_t count;

void write_lcd_cmd(unsigned char cmd);
void write_lcd_disp(unsigned char addr, unsigned char data);



void write_lcd_cmd(unsigned char cmd)
{

  //1
  PORTD |= (1 << LCD_WR); // wr high
  _delay_us(100);
  PORTD = PORTD | ((1 << LCD_DATA)); // data high MSB 3bit com
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_WR); // wr low, active low
  _delay_us(100);

  //2
  PORTD = PORTD | (1 << LCD_WR); // wr high
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_DATA); // data low
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_WR); // wr low active low
  _delay_us(100);

  //3
  PORTD = PORTD | (1 << LCD_WR); // wr high
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_DATA); // data low
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_WR); // wr low active low
  _delay_us(100);

  PORTD = PORTD | (1 << LCD_WR); // wr high

  for (int i = 8; i >= 0; i--)
  {
    PORTD = PORTD | (1 << LCD_WR); // wr high
    if (!(cmd & (1 << i)))
      PORTD = PORTD & ~(1 << LCD_DATA); // data low, if clear
    else
      PORTD = PORTD | (1 << LCD_DATA); // data high , if set
    PORTD = PORTD & ~(1 << LCD_WR); // wr low, active low
    _delay_us(100);
  }
}


void write_lcd_disp(unsigned char addr, unsigned char data)
{

  PORTD |= (1 << LCD_WR); // wr high
  _delay_us(100);
  //1
  PORTD = PORTD | (1 << LCD_DATA); // data high MSB 3bit com .... 1
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_WR); // wr low, active low
  _delay_us(100);
  //2
  PORTD = PORTD | (1 << LCD_WR); // wr high
  _delay_us(100);
  PORTD = PORTD | ((1 << LCD_DATA)); // data high ..............1
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_WR); // wr low active low
  _delay_us(100);
  //3
  PORTD = PORTD | (1 << LCD_WR); // wr high
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_DATA); // data low . ..............0
  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_WR); // wr low active low
  _delay_us(100);

  PORTD = PORTD | (1 << LCD_WR); // wr high

  for (int i = 6; i >= 0; i--) // write address 6 bit address,
  {

    PORTD = PORTD | (1 << LCD_WR); // wr high
    if (!(addr & (1 << i)))
      PORTD = PORTD & ~(1 << LCD_DATA); // data low, if clear
    else
      PORTD = PORTD | (1 << LCD_DATA); // data high , if set
    PORTD = PORTD & ~(1 << LCD_WR); // wr low, active low
    _delay_us(100);

  }
  PORTD = PORTD | (1 << LCD_WR); // wr high
  _delay_us(100);


  for (int i = 1; i <= 4; i++) // write data 4 bit data
  {
    PORTD = PORTD | (1 << LCD_WR); // wr high
    if (!(data & (1 << i)))
      PORTD = PORTD & ~(1 << LCD_DATA); // data low, if clear
    else
      PORTD = PORTD | (1 << LCD_DATA); // data high , if set

    PORTD = PORTD & ~(1 << LCD_WR); // wr low, active low
    _delay_us(100);

  }
  PORTD = PORTD | (1 << LCD_WR); // wr high
  _delay_us(100);


}

void setup(){
  
  DDRB = 0x01; // ext clk enable

  DDRD = 0xF7;



  /* MAX232_R1OUT-0,MAx232_T1IN-1,HV-PP1,CS-2,RD-3, WR-4,DATA-5, EPROM_CS-6,ADC_SD0-7 */

  //  1 1 1 0 1 1 1 1


  CLKPR = (1 << CLKPCE);
  //_delay_us(1000);
  CLKPR = 0x06;//


  PORTD = PORTD & ~(1 << LCD_CS); // cs low
  _delay_us(100);
  write_lcd_cmd(0x03) ; // lcd on command....................00000011
  _delay_us(100);
  PORTD = PORTD | ((1 << LCD_CS)); // cs high


  PORTD = PORTD & ~(1 << LCD_CS); // cs low
  _delay_us(100);
  write_lcd_cmd(0x29); // cmd 1/3bias , 1/4 duty;..........00101001
  PORTD = PORTD | ((1 << LCD_CS)); // cs hi

  _delay_us(100);
  PORTD = PORTD & ~(1 << LCD_CS); // cs low
  write_lcd_disp(0x2, 0xF);
  write_lcd_disp(0x3, 0xF);
  PORTD = PORTD | ((1 << LCD_CS)); // cs hi


  }

  void loop(){
    
    }
