#include <main.h>
#include <CGameDevice.h>
#include <CGame.h>
#include <CLoop.h>
#include <CPlay.h>
#include <CProfiler.h>

void CPlay::init (bool paused) {
	
	// init input state
	esc = lmbExists = rmbExists = false;
	
	// if game was paused, resume game else start game
	if (paused == false) init ();
}

void CPlay::init () {
	
	recentEvalTimes.clear (); numRecentEvals = 0;
	adjustedEvalTime = 0;	
	
	game->play ();	

}

void CPlay::eval () {

	game->draw ();

	if (game->isLevelComplete()) {
		doExitLevel();
		return;
	}		
	
	//
	// eval game when EVAL_INTERVAL has elapsed.
	// this ensures the game plays at the same speed 
	// on all machines.		
	//
	
	bool evaled = false;
	while (adjustedEvalTime >= CGame::EVAL_INTERVAL) {

		adjustedEvalTime -= CGame::EVAL_INTERVAL;
		evaled = true;	
		
		game->eval (lmbExists); 

		if (CGameDevice::MOUSE_RMB) rmbExists = true;	else if (rmbExists) { doPause (); return; }							
		if (isKeyPressed (CGameDevice::ESC, esc)) { doAbort (); return; }				

	}
	if (evaled) adjustedEvalTime = 0; // discard remaining time for smooth motion					

}

void CPlay::time (int64_t t) {

	//
	// t = time elapsed to run CPlay::eval (..)
	//
	

	//
	// average recent eval times to prevent a one off long
	// eval (eg., disk access by another program ) spoil the
	// smooth motion.
	//
	
	int64_t t0 = clock64 ();

	if (numRecentEvals == MAX_RECENT_EVALS) {
		recentEvalTimes.pop_front (); 
		--numRecentEvals;
	}

	recentEvalTimes.push_back (t); 
	++numRecentEvals;

	int64_t sumRecentEvalTime = 0; 
	for (ListOfInt64Iterator iter = recentEvalTimes.begin (), jter = recentEvalTimes.end (); iter != jter; ++iter) sumRecentEvalTime += *iter;
	adjustedEvalTime += (sumRecentEvalTime / numRecentEvals);
	
	// cout << "adjusted = " << adjustedEvalTime << endl;
	
	CLoop::time (t + clock64() - t0);	

}
