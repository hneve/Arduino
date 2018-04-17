//////////////////////////////////////////////////////////////////////////
//	New lib for reading analog valued button register
//	longpress ability with variable repeat.
//
//	by: Hans . Neve (at) gmail.com
//	based on function from dfrobot wiki.
//	Beerware
//	Default setup for DFrobot lcd shield.
//
//	my lcd blinks at aprox. 1.3 times a sec, 1/1.3=0.769 sec
//	making rate at 769 will repeat at same freq as lcd_blink
//////////////////////////////////////////////////////////////////////////

#ifndef key_read_h
	#define key_read_h

	#include <Arduino.h>
	#define btn_right			0	
	#define btn_up				1
	#define btn_down			2
	#define btn_left			3
	#define btn_select			4
	#define btn_none			5


	class key_read{
	public:
		key_read(int,int);
		void lp_off();
		void lp_on();
		int read();
	private:
		int _btn;
		int _adc_key_in;
		int _key_pin;
		int _btn_last_pressed;
		int _longpress;
		int _longpress_rate;
		unsigned long _debounce;
		unsigned long _btn_timestamp;
	};
#endif 