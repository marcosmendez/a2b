#ifndef _CMAINMENU
#define _CMAINMENU

#include <CMenu.h>
#include <CTextButton.h>
#include <vector>

class CMainMenu : public CMenu {

	vector <CTextButton> menuItems;	
	enum {INVALID = -1, NEW_GAME=0, PLAY=1, HISCORES=2, ABOUT=3, EXIT=4};	
	int selection;		

public:
	
	CMainMenu ();
	void init ();		
	void input ();
	void draw ();				

};

#endif
