#ifndef __CMAIN
#define __CMAIN

#include <CMainMenu.h>
#include <CLoop.h>
#include <CLevelBrowser.h>

class CGame;

class CMain : public CLoop {

	CGame* game;
	CMainMenu menu;
	CLevelBrowser levelbrowser;	
	bool esc;

public:
	void init ();
	void eval ();
	void setGame (CGame* g) { game = g; levelbrowser.setGame (g); }
	CMain (CGame* g) : levelbrowser (g) { setGame (g); }
	virtual ~CMain () {}
};

#endif
