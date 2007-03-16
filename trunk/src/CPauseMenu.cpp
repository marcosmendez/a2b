#include <CPauseMenu.h>
#include <CGameDevice.h>
#include <CTextButton.h>
#include <CLabel.h>
#include <main.h>
#include <string>
using namespace std;

CPauseMenu::CPauseMenu () {
	
	vector<string> labels;
	labels.push_back ("Resume");
	labels.push_back ("Replay");
	labels.push_back ("Abort");
	
	left = 10;
	int db = CGameDevice::stringHeight () + 2;
	bottom = CGameDevice::YMAX - 2 * db;
	
	for (int i = 0, j = labels.size (); i < j; i++) {
		menuItems.push_back (CTextButton (labels[i], left, bottom));
		bottom -= db;
	}			

	
}

void CPauseMenu::init () {
	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems [i].init ();
}

void CPauseMenu::draw () {
	
	static const string paused = "Game paused.";		
	CGameDevice::setColor (CGameDevice::red);
	CGameDevice::drawString (paused, left, CGameDevice::YMAX - CGameDevice::stringHeight ());
	
	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].draw ();

}

void CPauseMenu::input () {	

	selection = INVALID;
	for (int i = 0, j = menuItems.size (); i < j; i++) {
		menuItems[i].input ();
		if (menuItems[i].isSelected ()) {
			selection = i;
			menuItems[i].init ();
			break;
		}
	}
		
	switch (selection) {

		case ABORT: // abort game
			doAbort ();
			break;

		case RESUME: // resume play 
			doPlay (true);
			break;
			
		case REPLAY: // replay level
			doReplay ();
			break;

	}			

}
