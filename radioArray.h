#ifndef RADIOARRAY_H
#define RADIOARRAY_H

#include "common.h"
#include <vector>

using std::vector;

/* all-in-one class for an array of geiger counters based on 4 bit binary counters (74hc393) and 
 * parallel-in-serial-out shift registers (74hc166n)
 */

class RadioArray
{
	private:
		Pin clk, sh_ld, reset; //pins for clock, shift/loed (PE), and MR
		vector<Pin> datapins; //vector of SR data pins (Q7)
		vector<unsigned int> counts; //calculated accumulated count for each tube
		vector<byte> hardcounts; //last recorded value on digital counter hardware (74hc393)
		unsigned long lastFlushTime; //last recorded time counters were flushed

	public:
		//constructor
		RadioArray(Pin clk, Pin sh_ld, Pin reset, vector<Pin> datapins):
			clk(clk), sh_ld(sh_ld), datapins(datapins), // copy the pin numbers into the class
			reset(reset),
			counts(datapins.size() * 2, 0) //initialize the size to 2 * length(datapins) and full of zeros
		{
		}
		unsigned long getFlushPeriod() { return millis() - lastFlushTime; }
		void init(); //initialize radiometer array pins
		vector<byte> getCounterVals(); //snapshot and retrieve (fetch) raw values of all counters
		vector<byte> fetchCounterVals(); //just retrieve the raw values of each counter (after snapshot)
		void snapshot(); //simultaneously copy all counter values into the SRs (ext. hardware function)
		void poll(); //update the accumulating counter vector
		vector<unsigned int> flush(); //dump the values in all the accumulated counters and reset timer
		vector<unsigned int> getCounts() const { return counts; } //get the accumulated counts (don't modify them)
		void softReset(); //reset internal accumulated counter vector
		void hardReset(); //reset external counter hardware
		void allReset(); //reset both
		int numCounters() const { return counts.size(); }
		int numShifters() const { return datapins.size(); }
};

#endif




