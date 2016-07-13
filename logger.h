#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <vector>
#include "common.h"

using std::ofstream;
using std::string;
using std::vector;

class Logger
{
	private:
		ofstream logStream;
	public:
		void open(string filename);
		void append(vector<unsigned int> counts, unsigned long timestamp);
		void note(string note);
		void close();
		enum Exception
		{
			_openFailed,
			_alreadyOpen,
			_appendFailed,
			_unknownError
		};
};

#endif
