#ifndef _CGAME
#define _CGAME

#include <typedefs.h>

#include <CTrack.h>
#include <CTrain.h>
#include <CTrackGenerator.h>
#include <CTrainLauncher.h>
#include <CFuzzyVariable.h>
#include <CProgressbar.h>
#include <CEvent.h>

#include <iostream>
#include <string>
using namespace std;


class CGame {
	
	int seed; // game id
	
	//
	// track data
	//
	ListOfTracks tracks;
	CTrackGenerator generateTracks;
	
	//
	// train data
	//
	
	ListOfTrains trains;
	int numTrains; // used because trains is a <list> and trains.size () is inefficient
	CTrain* selectedTrain; 		
	CTrainLauncher* trainLauncher; // launches trains			
	
	int despatched, derailed;
	int lastDespatched, lastDerailed;
	int despatchedThisLevel; 

	void selectTrain (int x, int y); // select train nearest to x,y			
	void findCollidedTrains ();
	void removeTrains (); // remove trains that have completed/failed their journey		
	void moveTrains () {

		// move all trains
		for (ListOfTrainsIterator iter = trains.begin (), jter = trains.end (); iter != jter; ++iter) {
			CTrain* t = *iter; 
			if (t->isWaiting() == false) t->move();
		}

	}
	
	void launchTrain () { // try to launch a train 
		
		CTrain* train = trainLauncher->eval();
		if (train) {
			trains.push_back (train);
			train->begin ();
			++numTrains;		
		}		

	}						
	
	//
	// hud data
	//
	int level, levelsCompleted;
	int score;
	double efficiency;	
	int lastScore;
	double lastEfficiency;		
	CProgressbar despatchBar;		
	
	//
	// misc game data
	//
	
	ListOfEvents collisions; 
	
public:

	static string PLAYER_NAME;
	static int LAST_BROWSED_LEVEL;
	
	static int64_t EVAL_INTERVAL; // interval at which game is evaluated to ensure constant frame rate on different machines
	static int TRAINS_PER_LEVEL; // number of trains to despatch to complete a level
	
	CGame (int s); 
	~CGame ();
	
	ListOfTracks& getTracks () { return tracks; }
	
	ListOfTrains& getTrains () { return trains; }
	int getNumTrains () { return numTrains; }
	CTrain* getQTrain () { return trainLauncher->getTrainQ().get(); }

	void drawTracks ();
	void drawHUD ();
	void draw (); 

	void input (bool& lmbExists);
	
	void eval (bool& lmbExists) {
		evalEvents (collisions);
   	input (lmbExists);
		moveTrains ();
		findCollidedTrains ();
   	removeTrains ();
   	launchTrain ();

	}
		
	void setLevel (int i);
	int getLevel () { return level; }
	
	int getScore () { return score; }
	double getEfficiency () { return efficiency; }
	int getLevelsCompleted () { return levelsCompleted; }
	
	void play ();
	void stop ();
	void abort ();
	void levelComplete ();
	
	bool isLevelComplete () { return (despatchedThisLevel >= TRAINS_PER_LEVEL); }	
		
	// void autoplay ();

};

#endif
