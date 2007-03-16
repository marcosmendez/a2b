#include <CHiScores.h>
#include <CHiScoreTable.h>
#include <CTextButton.h>
#include <CCheckBox.h>
#include <main.h>
#include <CGameDevice.h>

extern CHiScoreTable hiscoretable;

CHiScores::CHiScores () : score ("Sort by score"), efficiency ("Sort by efficiency"), back ("Back") { 	
	
	int s = 10, dt = CGameDevice::stringHeight () + 5, t = CGameDevice::YMAX - dt;
	score.setLocation (s, t); t -= dt;
	efficiency.setLocation (s, t); t -= dt;
	back.setLocation (s, t);
	
}

void CHiScores::init () {
	
	score.init (); 
	efficiency.init ();
	back.init ();
	
	if (hiscoretable.isSortByScore()) {
		score.select ();
		efficiency.deselect ();
		hiscoretable.sortByScore ();
	} else {
		efficiency.select ();
		score.deselect ();
		hiscoretable.sortByEfficiency ();
	}
	
	esc = false;
	
}

void CHiScores::eval () {

	draw ();
	
	// read and apply input	
	bool before = score.isSelected(); score.input (); bool after = score.isSelected ();
	if (after != before) {
		if (after) {
			hiscoretable.sortByScore ();
			efficiency.deselect();
		} else score.select ();
	}
	
	before = efficiency.isSelected (); efficiency.input (); after = efficiency.isSelected ();
	if (after != before) {		
		if (after) {
			hiscoretable.sortByEfficiency ();	
			score.deselect ();
		} else efficiency.select ();		
	}
	
	back.input ();
	if (back.isSelected ()) {
		back.init ();		
		doMain ();
		return;
	}
					
	if ( isKeyPressed (CGameDevice::ESC, esc) ) doMain ();
	
}

void CHiScores::draw () {	
		
	score.draw ();
	efficiency.draw ();
	back.draw ();		
	hiscoretable.draw ();		
	
}
