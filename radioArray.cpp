#include "radioArray.h"


// initialize all the pins connected to the radiometer array
void RadioArray::init()
{
	//set pins as outputs
	pinMode(clk, OUTPUT);
	pinMode(sh_ld, OUTPUT);
	pinMode(reset, OUTPUT);
	
	//set up initial pin states (high or low)
	setPin(clk, LOW);
	setPin(sh_ld, HIGH);
	setPin(reset, LOW);

	//configure all data pins as inputs
	for(int i=0; i < numShifters(); i++)
		pinMode(datapins[i], INPUT);
	
	//reset counter hardware	
	hardReset();
	hardcounts = getCounterVals();
	lastFlushTime = millis();
}

// snapshot and retrieve counter values
vector <byte> RadioArray::getCounterVals()
{
	snapshot();
	return fetchCounterVals();
}

// get the values of every counter in the array
vector <byte> RadioArray::fetchCounterVals()
{
	//raw binary values of 74hc393 shift registers
	vector<byte> rawcounts(numShifters(), 0);
	
	//actual counts for each counter
	vector<byte> results(numCounters(), 0);

	//get the raw values from the SRs
	for(byte x = 0x80; x; x >>= 1)
	{
		delayMicroseconds(2);
		//for each SR in the array
		for(int i=0; i < numCounters(); i++)
			//get the value of that SR
			rawcounts[i] |= digitalRead(datapins[i]) ? x : 0;

		//send a clock pulse
		setPin(clk, HIGH);
		delayMicroseconds(2);
		setPin(clk, LOW);
	}

	//rip the actual timer values out of the raw SR values
	for(int i=0; i<numShifters(); i++)
	{
		results[2 * i] = rawcounts [i] >> 4;
		results[2 * i + 1] = rawcounts[i] & 0xF;
	}

	return results;
}

//scan the radio array for geiger counter updates and apply them to the 
// object's [counts] vector.
void RadioArray::poll()
{
	//scan the array and find out how much each counter has changed
	vector<byte> newCounterVals = getCounterVals();
	for(int i=0; i < numCounters(); i++)
	{
		counts[i] += (newCounterVals[i] + 16 - hardcounts[i]) % 16;
	}
	hardcounts = newCounterVals;
}

// return the current accumulated count for each counter
// and reset the accumulated count vector
vector<unsigned int> RadioArray::flush()
{
	vector <unsigned int> countcopy = counts;
	softReset();
	lastFlushTime = millis();
	return countcopy;
}

//reset the counter accumulator vector to zeros
void RadioArray::softReset()
{
	for(int i=0; i<numCounters(); i++)
		counts[i] = 0;
}

// reset the external counter hardware
void RadioArray::hardReset()
{
	//reset hardware count vector to 0's
	for(int i=0; i < hardcounts.size(); i++)
		hardcounts[i] = 0;

	//send a pulse to the reset pin
	setPin(reset, HIGH);
	delayMicroseconds(2);
	setPin(reset, LOW);
}

//perform soft (accumulators) and hard (counter hardware) reset
void RadioArray::allReset()
{
	softReset();
	hardReset();
}

//take a snapshot of all hardware counter values simultaneously
// (by sending a pulse to external H/W)
void RadioArray::snapshot()
{
	setPin(sh_ld, LOW);
	setPin(clk, HIGH);
	setPin(clk, LOW);
	setPin(sh_ld, HIGH);
	delayMicroseconds(2);
}


