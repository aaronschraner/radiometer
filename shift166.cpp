#include "shift166.h"

//initialize pins
void Shift166::init()
{
	pinMode(clk, OUTPUT);
	pinMode(sh_ld, OUTPUT);
	pinMode(Qh, INPUT);
	setPin(clk, LOW);
	setPin(sh_ld, HIGH);
}

// load pin states from the outside world into shift register
void Shift166::load()
{
	setPin(sh_ld, LOW); //put in LOAD mode
	setPin(clk, HIGH); //load at this instant
	setPin(clk, LOW); //reset clock and SH/LD
	setPin(sh_ld, HIGH);
	delayMicroseconds(2);
}

// pull the data out of the shift register serially
// return it as a uint8_t (byte)
byte Shift166::fetch()
{
	byte result = 0;

	//change to x=0x01; x; x<<=1 for LSBFIRST
	for(byte x=0x80; x; x >>= 1)
	{
		//wait for hardware to catch up
		delayMicroseconds(2);

		//read the current data bit
		/*
		   if digitalRead(Qh)
		   	bit N becomes true
		    else
		    	bit N stays false
		*/
		result |= digitalRead(Qh) ? x : 0;

		//tick the clock to get the next bit
		setPin(clk, HIGH);
		delayMicroseconds(2);
		setPin(clk, LOW);
	}
	return result;
}

