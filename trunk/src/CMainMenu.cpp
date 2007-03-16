#include <CMainMenu.h>
#include <CGameDevice.h>
#include <utils.h>
#include <main.h>

CMainMenu::CMainMenu () {

	// construct main menu	
	
	vector<string> labels;
	labels.push_back ("New game");
	labels.push_back ("Play");
	labels.push_back ("Top players");
	labels.push_back ("About");
	labels.push_back ("Exit"); 
	
	int db = CGameDevice::stringHeight () + 2;
	int left = 10, bottom = CGameDevice::WINDOW_HEIGHT - db; 
	for (int i = 0, j = labels.size (); i < j; i++) {
		menuItems.push_back (CTextButton (labels[i], left, bottom));
		bottom -= db;
	}		

}

void CMainMenu::init () {		
	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].init ();
}

void CMainMenu::input () {	

	selection = INVALID;
	for (int i = 0, j = menuItems.size (); i < j; i++) {
		CTextButton& mitem = menuItems[i];
		mitem.input ();
		if (mitem.isSelected ()) {
			selection = i;
			mitem.init ();
			break;
		}
	}
	
	switch (selection) {				
			
		case NEW_GAME: // new game
			doNewGame ();
			break;

		case PLAY: // play
			doPlay ();
			break;	
			
		case HISCORES: // show hi scores
			doHiScores ();
			break;
			
		case ABOUT:
			doAbout ();
			break;		

		case EXIT: // exit
			doExit ();
			break;			
			
	}				

}

void CMainMenu::draw () {			

	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].draw ();			

}
