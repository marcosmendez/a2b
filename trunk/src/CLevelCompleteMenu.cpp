#include <CLevelCompleteMenu.h>
#include <CLevelBrowser.h>
#include <CTextButton.h>
#include <utils.h>
#include <main.h>


CLevelCompleteMenu::CLevelCompleteMenu (CGame* g) : levelbrowser (g) {
	
	setGame (g);
	
	int db = CGameDevice::stringHeight () + 5;
	int left = 10,  bottom = CGameDevice::YMAX - 5 * db;
	
	vector<string> labels;
	labels.push_back ("Play");
	labels.push_back ("Abort");	
	
	int b = bottom;
	for (int i = 0, j = labels.size (); i < j; i++) {
		menuItems.push_back (CTextButton (labels[i], left, b));
		b -= db;
	}		
	
	int s = 10;
	string s_player = "Player:";
	b =  CGameDevice::YMAX - 2 * db;	
	menuItems.push_back (CTextButton (s_player, s, b));
	
	playerName.setText (CGame::PLAYER_NAME);
	playerName.setLocation (s + CGameDevice::stringWidth (s_player) + 1, b);

}

void CLevelCompleteMenu::init () {
	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].init ();
	levelbrowser.init ();
	playerName.init ();
	esc = false;
}

void CLevelCompleteMenu::input () {	
	
	levelbrowser.input (); useLevelNumber (levelbrowser.getLevelNumber ());
	playerName.input (); CGame::PLAYER_NAME = playerName.getText (); 	
	if ( (!playerName.isSelected ()) && (CGame::PLAYER_NAME == "") ) {
		CGame::PLAYER_NAME = "unknown";
		playerName.setText (CGame::PLAYER_NAME);
	}
	
	selection = INVALID;
	for (int i = menuItems.size () - 1, j = 0; i >= j; i--) {
		CTextButton& mitem = menuItems[i];
		mitem.input ();
		if (mitem.isSelected ()) {
			selection = i;
			mitem.init ();
			break;
		}
	}
	
	if (selection == PLAYER) playerName.select(); 
	else if (selection == PLAY) doPlay ();
	else if ( (selection == ABORT) || isKeyPressed (CGameDevice::ESC, esc) ) doAbort ();					
	
}

void CLevelCompleteMenu::draw () {					

	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].draw ();			
	levelbrowser.draw ();
	playerName.draw ();

}
