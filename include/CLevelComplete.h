#ifndef _CLEVELCOMPLETE
#define _CLEVELCOMPLETE

#include <CLoop.h>
#include <CLevelCompleteMenu.h>

class CGame;

class CLevelComplete : public CLoop {

	CGame* game;		
	CLevelCompleteMenu menu;
	int completedLevel;

public:
	
	virtual ~CLevelComplete () {}		
	void init (int c);
	void init () {}
	void eval ();
	void draw ();
	void setGame (CGame* g) { game = g; menu.setGame (g); }
	CLevelComplete (CGame* g) : menu (g) {setGame (g);}
	
};

#endif
