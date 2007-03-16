#ifndef _CEXITLEVELLOOP
#define _CEXITLEVELLOOP

#include <typedefs.h>
#include <CLoop.h>

class CGame;

class CExitLevel : public CLoop {

	CGame* game;
	ListOfEvents events;

public:

	void init ();
	void eval ();
	void setGame (CGame* g) { game = g; }
	CExitLevel (CGame* g) { setGame (g); }	
	virtual ~CExitLevel ();
	
	
};

#endif
