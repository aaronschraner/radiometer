#include <iostream>
#include "shift166.h"
#include "common.h"
#include "radioArray.h"
#include "logger.h"
#include <iomanip>
#include <sstream>

#ifndef DEFAULT_LOG_PATH
#define DEFAULT_LOG_PATH "radiation.log"
#endif

using namespace std;

//display a 4-bit binary value to cout
void bindisp(byte value)
{
	for(byte x=0x8; x; x>>=1)
		cout << (value & x ? '1' : '0');
}

//TODO: de-crappify
int main(int argc, char* argv[])
{
	//setup wiringPi
	if(wiringPiSetup() == -1)
		return -1;

	//create radiometer array object and initialize pins
	int dataPins[] = {0, 5};
	vector<Pin> vDataPins(dataPins, dataPins + sizeof(dataPins) / sizeof(Pin));

	RadioArray geiger(2, 3, 4, vDataPins);
	geiger.init();
	
	Logger countLog;
	const char* filename = argc > 1 ? argv[1] : DEFAULT_LOG_PATH;
	try
	{
		countLog.open(filename);
	}
	catch (Logger::Exception e)
	{
		switch(e)
		{
			case Logger::_openFailed:
				//TODO: handle more gracefully
				cout << "Failed to open logfile. Exiting." << endl;
				return -1;
			case Logger::_alreadyOpen:
				cout << "Warning: log file appears to already be open." << endl;
				break;
			default:
				cout << "Unknown exception occurred when opening logfile." << endl;
		}
	}

	stringstream ss;
	//    [          ]
	ss << "          ";
	for(int i=0; i<geiger.numShifters(); i++)
	{
		ss << setw(2) << right << vDataPins[i] <<   "/A      " <<
			setw(2) << right << vDataPins[i] << "/B      ";
	}
	countLog.note(ss.str());
	const int FLUSH_PERIOD = 60*1000; //milliseconds
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
		cout << setw(10 * geiger.numCounters()) << left << " } ";
		try
		{
			countLog.append(counts, millis());
			cout << "[LOGGED]";
		}
		catch(Logger::Exception e)
		{
			if(e == Logger::_appendFailed)
				cout << "[LOG ERROR]";
			else
				cout << "[UNKNOWN ERROR]";
		}
		cout << endl;


	}
	return 0;
}


