
#include "logger.h"
#include <iomanip>

using std::setw;
using std::left;
using std::right;
using std::endl;

//open the output log stream.
void Logger::open(string filename)
{
	//throw an error if it's already open
	if(logStream.is_open())
		throw _alreadyOpen;

	//attempt to open the file
	logStream.open(filename.c_str(), std::ofstream::out | std::ofstream::app);

	//if that didn't work, throw an exception.
	if(!logStream.good())
		throw _openFailed;
}

//append a set of geiger counter counts to the logfile
void Logger::append(vector<unsigned int> counts, unsigned long timestamp)
{
	//do your formatting stuff here

	//append the timestamp (in milliseconds)
	logStream << "[" << setw(9) << right << timestamp << "ms] ";

	//append the counts separated by commas
	for(int i=0; i<counts.size(); i++)
		logStream << setw(8) << right << counts[i] << (i < counts.size() - 1 ? ", " : "\n");

	//flush to the file after each update (in case of unexpected power loss)
	logStream.flush();
	
	if(!logStream.good())
		throw _appendFailed;
}


//close the file
void Logger::close()
{
	if(logStream.is_open())
		logStream.close();
}

//append a note to the file
void Logger::note(string note)
{
	logStream << "[INFO]: " << note << endl;
}


