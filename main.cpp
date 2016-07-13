#include <iostream>
#include "shift166.h"
#include "common.h"
#include "radioArray.h"
#include <iomanip>

using namespace std;

//display a 4-bit binary value to cout
void bindisp(byte value)
{
	for(byte x=0x8; x; x>>=1)
		cout << (value & x ? '1' : '0');
}

int main()
{
	//setup wiringPi
	if(wiringPiSetup() == -1)
		return -1;

	//create radiometer array object and initialize pins
	int dataPins[] = {0, 5};
	vector<Pin> vDataPins(dataPins, dataPins + sizeof(dataPins) / sizeof(Pin));

	RadioArray geiger(2, 3, 4, vDataPins);
	geiger.init();

	const int FLUSH_PERIOD = 60000; //milliseconds
	//continuously display the value of the counters
	while(true)
	{
		while(geiger.getFlushPeriod() < FLUSH_PERIOD)
		{
			geiger.poll();
			cout << "\r";
			vector<unsigned int> livecounts = geiger.getCounts();
			vector<byte> hwcounts = geiger.getCounterVals();
			for(int k = 0; k < geiger.numCounters(); k++)
			{
				cout << "[" << k << "]: " << livecounts[k] << " (";
				bindisp(hwcounts[k]);
				cout << ");   ";
			}
			delay(2);
		}
		vector<unsigned int> counts = geiger.flush();
		cout << " { ";
		for(int k = 0; k < geiger.numCounters(); k++)
			cout << counts[k] << "  ";
		cout << setw(10 * geiger.numCounters()) << left << " } " << endl;

	}
	return 0;
}


/*
int oldmain()
{
	//setup wiringPi
	if(wiringPiSetup() == -1)
		return -1;

	//create shift register object and initialize pins
	Shift166 myShift(2, 3, 0);
	myShift.init();

	byte result;
	int lastresult = -1;

	//continuously display the value of the two counters
	while(true)
	{
		//load the values from the outside world
		myShift.load();

		//pull them into the pi and out of the SR
		result = myShift.fetch();

		if(result != lastresult)
		{
			//display the counter values if they have updated
			cout << "Counter 1: ";
			bindisp(result);
			cout << "\t" << "Counter 2: ";
			bindisp(result >> 4);
			cout << endl;
		}
		lastresult = result;
		delayMicroseconds(100);
	}
	return 0;
}


*/
