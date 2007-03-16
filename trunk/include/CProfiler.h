#ifndef _CPROFILER
#define _CPROFILER

#include <string>
#include <iostream>
#include <clock64.h>
#include <CGameDevice.h>
using namespace std;

class CProfiler {

	string label;
	bool fps;

	int numProfiles;

	int64_t startTime, elapsedTime;
	int64_t minElapsedTime, maxElapsedTime, avgElapsedTime, totalElapsedTime;

public:

	CProfiler (const string& _label, bool _fps=false) : label (_label), fps (_fps) {
		numProfiles = 1;
		totalElapsedTime = 0;
		minElapsedTime = maxElapsedTime = -1; 
	}

	void start () {

		startTime = clock64 ();

	}

	void stop () {
		
		int64_t elapsedTime = clock64 () - startTime;
		if ((minElapsedTime == -1) || elapsedTime < minElapsedTime) minElapsedTime = elapsedTime; 
		else if ((maxElapsedTime == -1) || elapsedTime > maxElapsedTime) maxElapsedTime = elapsedTime;
		totalElapsedTime += elapsedTime;
		avgElapsedTime = totalElapsedTime / numProfiles;
		++numProfiles;

	}

	~CProfiler () {

		if (fps) {	// print timing in fps
			cout << label << "(fps):- max: " << CGameDevice::CLOCKS_PER_SECOND * 1.0 / minElapsedTime << " min: " << CGameDevice::CLOCKS_PER_SECOND * 1.0 / maxElapsedTime << " avg: " << CGameDevice::CLOCKS_PER_SECOND * 1.0 / avgElapsedTime  << endl;
		} else { // print timing in msecs (default)
			double T = 1000.0 / CGameDevice::CLOCKS_PER_SECOND;
			cout << label << "(msecs):- min: " << minElapsedTime * T << " max: " << maxElapsedTime * T << " avg: " << avgElapsedTime * T << endl;
		}

	}

};

#endif
