#include <CGame.h>
#include <CLevelBrowser.h>

#include <sstream>

CLevelBrowser::CLevelBrowser (CGame* g) : game (g) {

	menuItems.push_back (CTextButton ("<-"));
	menuItems.push_back (CTextButton ("->"));	

}

void CLevelBrowser::setLevelNumber (int i) {		
	
	stringstream ss; ss << "Level:" << i;
	string s; ss >> s; levelNumber.set (s);
	int sw = CGameDevice::stringWidth (s);
	
	int y0 = 5, y1 = y0 + CGameDevice::stringHeight ();
	int dw = 10;
	int x1 = (CGameDevice::XMAX - sw) / 2, x0 = x1 - CGameDevice::stringWidth ("<-") - dw, x2 = x1 + sw;	
	menuItems[0].setLocation (x0 - dw, y0);		
		levelNumber.setLocation (x1, y0, x2, y1);	
	menuItems[1].setLocation (x2 + dw, y0);		
	
}	


void CLevelBrowser::init () {		
	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].init ();
	setLevelNumber (game->getLevel ());
}

void CLevelBrowser::input () {	

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
	
	switch (selection) {	
		case NEXT_LEVEL:
			operator++();
			break;			
		case PREV_LEVEL:
			operator--();
			break;			
	}	
	
	if (isKeyDown (CGameDevice::LEFT)) operator--(); else if (isKeyDown (CGameDevice::RIGHT)) operator++();
	
}

void CLevelBrowser::draw () {			
	
	for (int i = 0, j = menuItems.size (); i < j; i++) menuItems[i].draw ();			
	levelNumber.draw ();	

}
