#ifndef SHIFT166_H 
#define SHIFT166_H
#include "common.h"

// simple class for the sn74hc166n SIPO shift register
// http://www.ti.com/lit/ds/symlink/sn74hc166.pdf
class Shift166
{
	public:
		Shift166(Pin clk, Pin sh_ld, Pin Qh):
			clk(clk), sh_ld(sh_ld), Qh(Qh)
		{}
		void load(); //load all inputs into SR
		byte fetch(); //retrive data from SR
		void init(); //initialize pins

	private:
		Pin clk, sh_ld, Qh;
		//clock, shift/_load, Qh (from datasheet)
};

#endif

