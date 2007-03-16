#ifndef _CTRAINLAUNCHER
#define _CTRAINLAUNCHER
	
#include <CTrainQ.h>
#include <CFuzzyVariable.h>
#include <typedefs.h>

class CGame;
class CTrain;	

class CTrainLauncher {	
	
	CGame* game;
	CTrainQ trainq;				
	
	int numAvailableTracks;		
	ListOfTracks availableTracks;
	ListOfTracks launchedTracks;		

	int normalLaunches;	
	
	int64_t launchInterval;
	double oppositionFrequency;						
	
	std::list <bool> debriefs;	
	int ndebriefs; 			
	static CFuzzyVariable success[2], failure[2];		

public:
	
	static int TRAINS_FOR_UNIT_LAUNCH_INTERVAL; 
	static double OPPOSING_TRAIN_FREQUENCY; 		
	
	CTrainLauncher (CGame* g);		
	~CTrainLauncher ();
	CTrain* eval ();
	CTrain* launchNormalTrain ();
	CTrain* launchOpposingTrain ();
	bool debriefTrain (CTrain* train);		
	
	CTrainQ& getTrainQ () {return trainq;}				
	int64_t getLaunchInterval () { return launchInterval; }
	double getOppositionFrequency () { return oppositionFrequency; }		

};
#endif
