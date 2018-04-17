#define setbit(ADDRESS,BIT) (ADDRESS |= (1<<BIT))
#define clearbit(ADDRESS,BIT) (ADDRESS &= ~(1<<BIT))
#define checkbit(ADDRESS,BIT) (ADDRESS & (1<<BIT))
#define setbitmask(x,y) (x |= (y))
#define clearbitmask(x,y) (x &= (~y))
#define flipbitmask(x,y) (x ^= (y))
#define checkbitmask(x,y) (x & (y))
#define Button checkbit(PINF,2)


#define led_on setbit(PORTB,7)
#define led_off clearbit(PORTB,7)


void setup()
{
setbitmask(DDRB, 0b10000000);
setbit(PORTF, 2);
//pinMode(13,OUTPUT);
  /* add setup code here */

}

void loop()
{

led_off;
if(Button == LOW){
	
	led_on;
	}

}
