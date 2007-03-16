#ifndef _CPLAY
#define _CPLAY

#include "CLoop.h"
#include <list>

class CGame;

class CPlay: public CLoop {

	CGame* game;

	bool esc, lmbExists, rmbExists;

	int64_t adjustedEvalTime;
	static const int MAX_RECENT_EVALS = 10;
	int numRecentEvals;
	std::list<int64_t> recentEvalTimes;

public:
	
	CPlay (CGame* g) { setGame (g); }
	virtual ~CPlay () {}		
	void init (bool paused);	
	void init ();
	void eval ();
	void time (int64_t t);
	void setGame (CGame* g) { game = g; }	

};

#endif
