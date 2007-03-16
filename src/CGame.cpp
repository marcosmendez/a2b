#include <CGameDevice.h>
#include <CTrack.h>
#include <CTrain.h>
#include <CTrainQ.h>
#include <CTrainLauncher.h>
#include <CCollision.h>
#include <CExplosion.h>
#include <CGame.h>
#include <main.h>

int64_t CGame::EVAL_INTERVAL = 0;
int CGame::TRAINS_PER_LEVEL = 0;

int CGame::LAST_BROWSED_LEVEL = 0;
string CGame::PLAYER_NAME = "";

CGame::CGame (int s=0) : 
	seed (s),
	generateTracks ("../data/tree.txt"), 
	trainLauncher(0)	
{
	
	lastScore = 0;
	lastEfficiency = 0;
	lastDespatched = lastDerailed = 0;	
	levelsCompleted = 0;
	level = LAST_BROWSED_LEVEL;	
	setSeed (s);
	setLevel (level);
	
	int y = CGameDevice::stringHeight () + 5;
	despatchBar.setLocation (CGameDevice::XMAX - 175, y, CGameDevice::XMAX - 5, y + 15); 
	
}

CGame::~CGame () {	

	if (trainLauncher) delete trainLauncher;
	clear<ListOfTrains, CTrain>(trains);
	clear<ListOfEvents, CEvent>(collisions);
		
}

void CGame::setLevel (int i) {

	/*
		set i as current level and load tracks of the current level
	*/
	
	if (i < 0) i = 0; 
	
	level = i;
	LAST_BROWSED_LEVEL = i;	
	generateTracks (tracks, level);	
		
}

void CGame::play () {	

	/*
		play current level
	*/		
	
	setSeed (seed + level); 
	trainLauncher = new CTrainLauncher (this);	
	
	score = lastScore;
	efficiency = lastEfficiency;
	despatched = lastDespatched;
	derailed = lastDerailed;
	despatchedThisLevel = 0;
	
	numTrains = 0;
	selectedTrain = 0;	
	despatchBar.reset ();		
	
}

void CGame::stop () {
	
	/*
		stop playing current level
	*/
	
	if (trainLauncher) {
		delete trainLauncher;
		trainLauncher = 0;
	}
	clear<ListOfTrains, CTrain>(trains);
	clear<ListOfEvents, CEvent>(collisions);	
	
}

void CGame::abort () {

	/*
		abort game
	*/

	stop ();
	
	if (levelsCompleted) doAddScore (lastScore, lastEfficiency, levelsCompleted);
	
	lastScore = 0;
	lastEfficiency = 0;
	lastDespatched = lastDerailed = 0;	
	levelsCompleted = 0;	
	
}

void CGame::levelComplete () {	

	/*
		level complete
	*/	
	
	stop ();
	
	lastScore = score;
	lastEfficiency = efficiency;
	lastDespatched = despatched;
	lastDerailed = derailed;
	++levelsCompleted;
	
	setLevel (++level);
	
}

void CGame::input (bool& lmbExists) {

	/*
		apply input to mod game state
	*/	
	
	// select train if left clicked
	if (CGameDevice::MOUSE_LMB) {
		if (lmbExists == false) {
			selectTrain (CGameDevice::MOUSE_X, CGameDevice::MOUSE_Y);			
			lmbExists = true;
		}
	}	else lmbExists = false;		
	
	// accelerate/brake/stop selected train 	
	if (selectedTrain && (selectedTrain->isWaiting() == false)) {
		
		if (isKeyDown (CGameDevice::UP) || isKeyDown (CGameDevice::W)) selectedTrain->accelerate (); 
			
		if (isKeyDown (CGameDevice::DOWN) || isKeyDown (CGameDevice::S)) selectedTrain->brake (); 
			
		if (isKeyDown (CGameDevice::RIGHT) || isKeyDown (CGameDevice::D) || 
			 isKeyDown (CGameDevice::LEFT) || isKeyDown (CGameDevice::A) ) selectedTrain->stop (); 	
	}

}

void CGame::findCollidedTrains () {

	/*
		find all the colliding trains
	*/

	for (ListOfTrainsIterator iter = trains.begin (), jter = --trains.end (); iter != jter;) {
		CTrain* thisTrain = *iter;
		if (thisTrain->isTraveling ()) {
			for (ListOfTrainsIterator pter = ++iter, qter = trains.end (); pter != qter; ++pter) {
				CTrain* thatTrain = *pter;
				if (thatTrain->isTraveling ()) {
					if (thisTrain->maybeCollided (thatTrain)) {
						if (thisTrain->surelyCollided (thatTrain)) {
							thisTrain->end (CTrain::COLLIDED);
							thatTrain->end (CTrain::COLLIDED);
							collisions.push_back (new CCollision (thisTrain, thatTrain));
							return;
						}
					}
				}
			}
		} else ++iter;
	}

}

void CGame::removeTrains () {

	/*
		remove trains that have finished their journey 
	*/

	ListOfTrainsIterator iter = trains.begin (), jter;
	for (int i = 0; i < numTrains; i++) {
		CTrain *ithTrain = *iter;
		if (ithTrain->hasFinishedJourney ()) {					
			if (trainLauncher->debriefTrain (ithTrain)) {
				++despatched;
				++despatchedThisLevel;
			} else ++derailed;
			
			score += ithTrain->getPoints ();
			efficiency = despatched * 100.0 / (despatched + derailed);				
			
			if (ithTrain->isSelected ()) selectedTrain = 0; 
			trains.erase (iter); delete ithTrain; 
			if (i == 0) iter = trains.begin (); else { iter = jter; ++iter; }
			i--; --numTrains;

		} else {

			jter = iter;
			++iter;

		}
	}
}



void CGame::selectTrain (int mousex, int mousey) {
	/*
		selects train nearest to mouse location.
		deselects previously selected train.				
	*/			
	
	CTrain *lastSelectedTrain = selectedTrain;		
		
	//
	// find the nearest running train (NRT)
	//
		
	CTrain* qtrain = getQTrain ();
	ListOfTrainsIterator iter = trains.begin (), jter = trains.end();	
	if (iter == jter) {
		if (qtrain) {
			selectedTrain = qtrain;
			selectedTrain->select ();
		}
	} else {
		
		// assume first train as NRT
		selectedTrain = *iter;
		int sd2 = 0; selectedTrain->mouseTest (mousex, mousey, sd2);
		
		// run thru all running trains to find the real NRT
		for (++iter; iter != jter; ++iter) {
			CTrain *train = *iter; 
			int d2 = 0; train->mouseTest (mousex, mousey, d2);
			if (d2 < sd2) {
				sd2 = d2;
				selectedTrain = train;
			}
		}							
		
		/*
			select qed train if its nearer than NRT and the NRT is far enough.
			the idea is the user mostly wants to select a running train not a qed train.
		*/
		if (qtrain) {
			int qd2 = 0; qtrain->mouseTest (mousex, mousey, qd2);		
			static const int D2 = 2500; 
			if ((qd2 < sd2) && (sd2 > D2)) selectedTrain = qtrain;				
		}		
		
		selectedTrain->select ();
		
	}			
		
	// deselect previous selection
	if (lastSelectedTrain && (selectedTrain != lastSelectedTrain)) lastSelectedTrain->deselect ();		

}

void CGame::draw () {

	/*
	
		draw game

		order of drawing:
			heads-up-display (hud)
			waypoints of q'ed train
			waypoints of active trains
			coaches of q'ed train
			collisions 
			coaches of active trains
	*/
	
	drawHUD ();	
	
	// get q'ed train
	const CTrainQ& trainq = trainLauncher->getTrainQ(); 
	CTrain* qtrain = trainq.get();		
	
	
	// draw waypoints of q'ed train 
	if (qtrain) {
		if (qtrain->isSelected ()) 
			qtrain->drawWaypoints (CGameDevice::green);
		else
			qtrain->drawWaypoints (CGameDevice::gray);
	}
	
	// draw waypoints of active trains
	for (ListOfTrainsIterator iter = trains.begin (), jter = trains.end (); iter != jter; ++iter) {
		CTrain *itrain = *iter;
		if (itrain->isSelected ()) {
			itrain->drawWaypoints (CGameDevice::green);			
		} else itrain->drawWaypoints (itrain->getTrack()->getColor());
	}
	
	// draw coaches of q'ed train 
	if (qtrain) {
		if (qtrain->isSelected ()) {
			qtrain->drawCoaches (CGameDevice::green);
			trainq.drawCountdown (); 
		} else {
			qtrain->drawCoaches (CGameDevice::gray);
			trainq.drawCountdown (); 
		}
	}
	
	// draw collisions
	drawEvents (collisions);
	
	// draw coaches of active trains.
	for (ListOfTrainsIterator iter = trains.begin (), jter = trains.end (); iter != jter; ++iter) {
		CTrain *itrain = *iter;
		if (itrain->isSelected ()) 
			itrain->drawCoaches (CGameDevice::green);
		else 
			itrain->drawCoaches (itrain->getTrack () -> getColor ());
	}
	
}

void CGame::drawHUD () {

	//
	// draws heads-up-display
	//	
	
	
		
	static char buf [256];
	static const char *s_despatched = "Despatched:";
	static const char *s_score = "Score:", *s_efficiency = "Efficiency:";
	static const int w_score = CGameDevice::stringWidth (s_score);
	static const int w_efficiency = CGameDevice::stringWidth (s_efficiency);
	static const int strHeight = CGameDevice::stringHeight ();
	
	// draw despatch and next train due info
	despatchBar.setCurrent (despatchedThisLevel); 
	despatchBar.draw ();	
	
	CGameDevice::setColor (CGameDevice::gray);
	const CBox& loc = despatchBar.getLocation ();
	int y = loc.bottom - strHeight;
	CGameDevice::drawString (s_despatched, loc.left, y);
	sprintf (buf, "%03d", despatchedThisLevel);
	CGameDevice::drawString (buf, CGameDevice::XMAX - CGameDevice::stringWidth (buf), y);	
	
	// draw score 
	CGameDevice::drawString (s_score, 0, y);
	if (score < 0) CGameDevice::setColor (CGameDevice::red); else CGameDevice::setColor (CGameDevice::green);
	sprintf (buf, "%d", score); CGameDevice::drawString (buf, w_score, y);	
	
	// draw efficiency
	int xe = (CGameDevice::XMAX - w_efficiency) / 3; 
	CGameDevice::setColor (CGameDevice::gray);
	CGameDevice::drawString (s_efficiency, xe, y);
	CGameDevice::setColor (CGameDevice::yellow);
	sprintf (buf, "%.1f%%", efficiency); 
	CGameDevice::drawString (buf, xe + w_efficiency + 1, y);
	
	
	// draw next train data
	sprintf (buf, "Next train: %02.03f sec", trainLauncher->getLaunchInterval() * CGameDevice::INVERSE_CLOCKS_PER_SECOND);
	int xn = (CGameDevice::XMAX - CGameDevice::stringWidth (buf)) / 2;
	CGameDevice::setColor (CGameDevice::gray);	
	int yt = CGameDevice::YMAX - CGameDevice::stringHeight ();
	CGameDevice::drawString (buf, xn, yt);
	
}

void CGame::drawTracks () {
	
	/*
		draw tracks 
	*/
	
	for (ListOfTracksIterator i = tracks.begin(), j = tracks.end(); i != j; ++i) (*i)->draw();		

}

/* void CGame::autoplay () {
	if (numTrains > 0) {
		CTrain* train = *--trains.end();
		CTrain* ls = selectedTrain;
		if (ls) ls->deselect ();
		selectedTrain = train;
		selectedTrain->select ();
		selectedTrain->accelerate();
	}
}*/
