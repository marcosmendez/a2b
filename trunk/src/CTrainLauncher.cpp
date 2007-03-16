#include <CTrack.h>
#include <COutwardTrain.h>
#include <CReturnTrain.h>
#include <CTrainLauncher.h>
#include <CGame.h>
#include <CGameDevice.h>
#include <utils.h>
#include <CProfiler.h>
#include <main.h>
#include <algorithm>

int CTrainLauncher::TRAINS_FOR_UNIT_LAUNCH_INTERVAL = 0;												 
double CTrainLauncher::OPPOSING_TRAIN_FREQUENCY = 0;

CFuzzyVariable CTrainLauncher::success[2] = {CFuzzyVariable("../data/despatch.suc"), CFuzzyVariable ("../data/opposition.suc")};
CFuzzyVariable CTrainLauncher::failure[2] = {CFuzzyVariable("../data/despatch.fail"), CFuzzyVariable ("../data/opposition.fail")};

CTrainLauncher::CTrainLauncher (CGame* g) : game(g), availableTracks (game->getTracks()) {	

	numAvailableTracks = availableTracks.size (); 
	
	// initial launch interval is the (slowest) fastest time required for a train to complete a track
	ListOfTracksIterator i = availableTracks.begin (), j = availableTracks.end();			
	launchInterval = (*i)->getMinTime ();
	for (++i ; i != j ; ++i) {
		int64_t minTime = (*i)->getMinTime();
		if (minTime > launchInterval) launchInterval = minTime;
	}			
	launchInterval += CTrain::TIME_TO_REACH_MAX_SPEED; 	
	
	/*
		set quantities of fuzzy variables that will determine how the launch interval will change. 
		we want to bring the launch interval to 1.0 second when TRAINS_FOR_UNIT_LAUNCH_INTERVAL trains 
		are despatched. this ensures the game plays equally brisk on all levels irrespective of track lengths.
	*/
	int64_t q = (launchInterval - CGameDevice::CLOCKS_PER_SECOND) / TRAINS_FOR_UNIT_LAUNCH_INTERVAL;
	int64_t qmin = CGameDevice::CLOCKS_PER_SECOND / 100; 
	if (q < qmin) q = qmin;
		
	//console()<<"despatch success 100% " << -q * 1.0 / CGameDevice::CLOCKS_PER_SECOND << " 0% = " << q/4 * 1.0 /CGameDevice::CLOCKS_PER_SECOND<<endl;
	success[0].setQuantity (0, q / 4);
	success[0].setQuantity (1, -q);	
	failure[0].setQuantity (0, q);	
	
	oppositionFrequency = OPPOSING_TRAIN_FREQUENCY;	
	double o = (oppositionFrequency - 1) * 1.0 / TRAINS_FOR_UNIT_LAUNCH_INTERVAL;
	success[1].setQuantity (0, o / 4);
	success[1].setQuantity (1, -o);
	failure[1].setQuantity (0, o);	
	
	normalLaunches = 1;	
	ndebriefs = 0;	

}

CTrainLauncher::~CTrainLauncher () {

	CTrain* qtrain = trainq.get();
	if (qtrain) delete qtrain;

}

CTrain* CTrainLauncher::eval () {
	
	if ( trainq.isEmpty () ) { 	
		
		CTrain* train = 0;	
		
		int iof1 = (int) oppositionFrequency; 
		//console()<< "normal launches = "<< normalLaunches << " opposition frequency = "<< oppositionFrequency;
		if (normalLaunches == iof1) { 
		
			int r = getRand7(), iof2 = (oppositionFrequency - iof1) * 128; 		
			
			if (r >= iof2) { 
			
				train = launchOpposingTrain();
				if (!train) {					
					//console()<<"failed opposition: launched normal train instead"<<endl;
					train = launchNormalTrain ();
					++normalLaunches;
					
				} else {/*console()<<"launched opposing train"<<endl;*/ normalLaunches = 1;	}
				
			}	else {				
				
				//console()<<"launched normal train"<<endl;
				train = launchNormalTrain ();
				++normalLaunches;				
				
			}				
			
		} else if (normalLaunches > iof1) {
		
			train = launchOpposingTrain ();
			if (!train) {			
				//console()<<"failed opposition: launched normal train instead"<<endl;
				train = launchNormalTrain ();
				++normalLaunches;				
			} else {/*console()<<"launched opposing train"<<endl;*/ normalLaunches = 1;	}
			
		} else {
			//console()<<"launched normal train"<<endl;
			train = launchNormalTrain ();
			++normalLaunches;
			
		}		
		
		// q the train and intro into the game when a countdown runs out 
		trainq.set (train, launchInterval);			
		
	}				
		
	return trainq.eval ();

}

CTrain* CTrainLauncher::launchNormalTrain () {
	/*
		launch a train on a randomly picked track. trains are launched 
		on all available tracks before being launched on a previously 
		launched track.
	*/

	// randomly pick an available track. 
	int t = getRand8 () % numAvailableTracks; 
	ListOfTracksIterator iter;
	CTrack* track = get <ListOfTracks, CTrack*> (availableTracks, iter, t);			
	
	// launch a train on this track
	CTrain *train = 0;
	int occupied = track->getOccupied ();
	if (occupied == CTrack::FREE) { // generate an outward/return train
		if (getRandBit()) train = new COutwardTrain (track); else train = new CReturnTrain (track);
	} else 
		// if track is occupied, generate a train of the type of occupying train. this prevents an
		// impossible to play condition: 2 trains going in opposite directions on the same track
		if (occupied == CTrack::OCCUPIED_OUT)
			train = new COutwardTrain (track); 
		else 
			train = new CReturnTrain (track);						
	
	launchedTracks.push_back (track);	
	availableTracks.erase (iter);
	if (--numAvailableTracks == 0) { 
		availableTracks = launchedTracks;	launchedTracks.clear ();
		numAvailableTracks = availableTracks.size (); 
	}		
	
	return train;
	
}

CTrain* CTrainLauncher::launchOpposingTrain () {
	/*
		an opposing train departs from the destination of the most recently 
		launched train in the hope that it can cause congestion/collision making
		life difficult/challenging for the player.
	*/	
	
	ListOfTrains& trains = game->getTrains ();
	int numTrains = game->getNumTrains ();
	if (numTrains == 0) return 0; // nothing to oppose
	
	/*
		oppose the most recently launched train bcos it has just begun its journey
	*/
	CTrain* train = *--trains.end (); // most recently launched train (MRT)
	CTrack* track = train->getTrack (); // track on which the MRT is running
	
	// get destination of MRT
	CNode* destination = 0; 
	if (track->getOccupied() == CTrack::OCCUPIED_OUT) 
		destination = track->getNode (1); 
	else 
		destination = track->getNode (0);	
	
	// get tracks containing destination
	ListOfTracks destinationTracks; 
	getTracksContainingNode (destinationTracks, destination, game->getTracks());
	
	// launch the opposing train from a randomly picked track emnating from destination of MRT.
	CTrain* opposingTrain = 0; 
	CTrack* opposingTrack = 0;	
	int nd = destinationTracks.size ();
	while (nd > 0) {
		int i = getRand16 () % nd;
		CTrack* t = get<ListOfTracks, CTrack*> (destinationTracks, i);
		int occupied = t->getOccupied();
		if (destination == t->getNode(0)) { 
			if (occupied != CTrack::OCCUPIED_RETURN) {
				opposingTrain = new COutwardTrain (t);
				opposingTrack = t;	
				break;
			}
		} else {
			if (occupied != CTrack::OCCUPIED_OUT) {
				opposingTrain = new CReturnTrain (t);
				opposingTrack = t;
				break;
			}
		}		
		erase (destinationTracks, i); --nd;
	}
	
	if (opposingTrack) {
		
		// remove track launched from available tracks
		ListOfTracksIterator iter = find (availableTracks.begin (), availableTracks.end (), opposingTrack);
		if (iter != availableTracks.end()) {
			launchedTracks.push_back (opposingTrack);
			availableTracks.erase (iter); 
			if (--numAvailableTracks == 0) {
				availableTracks = launchedTracks;
				numAvailableTracks = availableTracks.size ();
				launchedTracks.clear ();
			}
		}		
		
		return opposingTrain;
		
	} else return 0;	
	
}

bool CTrainLauncher::debriefTrain (CTrain* train) {
	/*
		debriefs train and reduces or increases the launch interval & opposition 
		frequency based on how well the train was despatched.  
		
		if the train was despatched well, launch interval reduces a bit, opposition
		frequency increases a bit making life a bit more difficult for the player.
		
		if the despatch failed, launch interval increases a bit, opposition
		frequency decreases a bit making life a bit easier for the player.
		
		how much the launch interval and opposition frequency reduce or increase is
		determined by fuzzy variables success and failure. they are loaded from files
		specified above at start of the game.		
		
	*/

	bool arrived = train->hasArrived ();
	
	static const int MAX_DEBRIEFS = 6;	
	if (ndebriefs >= MAX_DEBRIEFS) {
		debriefs.pop_front (); --ndebriefs;
	}	
	debriefs.push_back (arrived);	++ndebriefs;	
	
	int input;
	int64_t deltaLaunchInterval;
	double deltaOppositionFrequency;
	int points;
	
	if (arrived) {		

		CTrack* track = train->getTrack ();
		int64_t journeyTime = train->getJourneyTime ();
		int64_t minTime = track->getMinTime (), maxTime = track->getMaxTime ();
		if (journeyTime < minTime) journeyTime = minTime; else if (journeyTime > maxTime) journeyTime = maxTime;
		double factor = (maxTime - journeyTime) * 1.0 / (maxTime - minTime);		
		
		input = success[0].box.left + success[0].box.width * factor;
		success[0].fuzzify (input); success[1].fuzzify (input);
		
		deltaLaunchInterval = success[0].getCrisp ();
		deltaOppositionFrequency = success[1].getCrisp ();
		points = factor * 100;		
		
		/*console()<<"success: delta = "<<deltaLaunchInterval * 1.0 / CGameDevice::CLOCKS_PER_SECOND;		
		
		for (int i = 0; i < 640; i++) {
			success[0].fuzzify(i);
			//console()<<"i = "<<i*100/639<<"out = "<<success[0].getCrisp ()*1.0/CGameDevice::CLOCKS_PER_SECOND<<endl;
		}*/		

	} else {				
	
		
		int successes = 0, failures = 0;
		for (list<bool>::iterator i = debriefs.begin (), j = debriefs.end (); i != j; ++i) if (*i) ++successes; else ++failures;
		int result = successes - failures;			
		static const int MAX_RESULT = MAX_DEBRIEFS - 2;
		static const int NUM_RESULTS = 2 * MAX_DEBRIEFS - 1;				
		
		input = failure[0].box.left + failure[0].box.width * (MAX_RESULT - result) / NUM_RESULTS;
		failure[0].fuzzify (input); failure[1].fuzzify(input);		
		deltaLaunchInterval = failure[0].getCrisp (); 
		deltaOppositionFrequency = failure[1].getCrisp ();
		points = -100;						
		
		/*console()<<"successes = " << successes << " failure = " << failures << " result = "<< result << " delta = "<< deltaLaunchInterval * 1.0 / CGameDevice::CLOCKS_PER_SECOND <<endl;
		
		
		for (int rr = 4; rr > -7; rr-=2) {
			int rsk = failure[0].box.left + failure[0].box.width * (MAX_RESULT - rr) / NUM_RESULTS;
			failure[0].fuzzify (rsk); 
			console()<<"result " << rr << " crisp = "<< failure[0].getCrisp () * 1.0 / CGameDevice::CLOCKS_PER_SECOND<<endl;
			
		}
		
		////console()<<"failure"<<endl;
		for (int i = 0; i < 640; i++) {
			failure[0].fuzzify(i);
			//console()<<"i = "<<i*100/639<<"out = "<<failure[0].getCrisp ()<<endl;
		}*/						
		
	}	
	
	launchInterval += deltaLaunchInterval; if (launchInterval < 0) launchInterval = 0;
	oppositionFrequency += deltaOppositionFrequency;	if (oppositionFrequency < 1) oppositionFrequency = 1;	
	//console()<<"delta launch interval = "<<deltaLaunchInterval*CGameDevice::INVERSE_CLOCKS_PER_SECOND<< " delta opp = "<<deltaOppositionFrequency<<endl;
	train->setPoints (points);
	//console()<<"launch interval = "<<launchInterval * 1.0 / CGameDevice::CLOCKS_PER_SECOND<<endl;
	
	
	
	return arrived;	

}
