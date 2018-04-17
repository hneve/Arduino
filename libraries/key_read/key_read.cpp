#include <Arduino.h>
#include <key_read.h>

key_read::key_read(int pin, int lp_rate){
_key_pin = pin;
_longpress_rate = lp_rate;
_longpress = true;

}

int key_read::read(){
	if(millis() < _debounce + 100) return btn_none;  // Debounce for 10ms
	
	if(_longpress){
			if (millis() >= (_btn_timestamp + _longpress_rate)) {
				_btn_last_pressed = btn_none;
				_btn_timestamp = millis();
			}
	}
	int _btn;
	int _adc_key_in = analogRead(_key_pin); 
	if (_adc_key_in > 1000)		{_btn = btn_none;	}
	else if (_adc_key_in < 50)	{_btn = btn_right;  }
	else if (_adc_key_in < 250)	{_btn = btn_up;		}
	else if (_adc_key_in < 350)	{_btn = btn_down;	}
	else if (_adc_key_in < 550)	{_btn = btn_left;	}
	else if (_adc_key_in < 790)	{_btn = btn_select;	}
		
	if (_btn == _btn_last_pressed) return btn_none;
	_btn_last_pressed = _btn;
	_btn_timestamp = millis();
	_debounce = millis();
	return _btn;

}

void key_read::lp_off(){
	_longpress = false;
}
void key_read::lp_on(){
	_longpress = true;
}
