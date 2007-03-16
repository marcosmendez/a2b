#ifndef _CLEVELBROWSER
#define _CLEVELBROWSER

#include <CMenu.h>
#include <CTextButton.h>
#include <CLabel.h>
#include <CGame.h>
#include <vector>
using namespace std;

class CLevelBrowser : public CMenu {

	CGame* game;
	CLabel levelNumber;

	enum {INVALID = -1, PREV_LEVEL=0, NEXT_LEVEL=1};
	int selection;		

	vector <CTextButton> menuItems;
	
	void setLevelNumber (int i);

public:
	
	CLevelBrowser (CGame* g);	
	void setGame (CGame* g) { game = g; }

	void init ();
	void input ();
	void draw ();	
	int getLevelNumber () const { return game->getLevel (); }	
	void operator++ () {
		game->setLevel (game->getLevel() + 1);		
		setLevelNumber (game->getLevel ());
		
	}	
	void operator-- () {
		game->setLevel (game->getLevel() - 1);
		setLevelNumber (game->getLevel ());		
	}
	
};

#endif
