#ifndef _CLEVELCOMPLETEMENU
#define _CLEVELCOMPLETEMENU

#include <CMenu.h>
#include <CTextButton.h>
#include <CTextField.h>
#include <CLevelBrowser.h>
#include <CLabel.h>
#include <vector>
using namespace std;

class CGame;

class CLevelCompleteMenu : public CMenu {	

	CGame* game;
	
	enum {INVALID = -1, PLAY=0, ABORT=1, PLAYER=2}; 
	int selection;

	CTextField playerName;	
		
	vector <CTextButton> menuItems;
	CLevelBrowser levelbrowser;
	
	void useLevelNumber (int i) {			
	
		static char ln[256]; 
		if (i == -1) sprintf (ln, "Play Level %d", game->getLevel());
		else sprintf (ln, "Play Level %d", i); 
		menuItems[0].setLabel(ln);
	}		
	
	bool esc;

public:
	
	CLevelCompleteMenu (CGame* g);
	
	void setGame (CGame* g) { game = g; levelbrowser.setGame(g); }	
	
	void init ();
	void input ();
	void draw ();	
};

#endif
