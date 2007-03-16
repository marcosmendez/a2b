#include <CGame.h>
#include <CLevelComplete.h>
#include <main.h>

void CLevelComplete::init (int c) {
	
	completedLevel = c;	
	menu.init ();

}

void CLevelComplete::eval () {

	draw ();	
	menu.input ();	

}

void CLevelComplete::draw () {		

	game->drawTracks (); 	
		
	const static char *s_score = "Score: ", *s_efficiency = "Efficiency: ";
	
	int dt = CGameDevice::stringHeight () + 5, s = 10, t = CGameDevice::YMAX - dt;	
	static char buf [256];	
	sprintf (buf, "Level %d complete.", completedLevel);
	CGameDevice::setColor (CGameDevice::gray);
	CGameDevice::drawString (buf, s, t);
	
	s = 20;
	t -= 2 * dt;	
	
	CGameDevice::drawString (s_score, s, t);
	int score = game->getScore ();
	if (score < 0) CGameDevice::setColor (CGameDevice::red); else CGameDevice::setColor (CGameDevice::green);
	sprintf (buf, "%d", score);
	CGameDevice::drawString (buf, s + CGameDevice::stringWidth(s_score), t); t -= dt;
	
	CGameDevice::setColor (CGameDevice::gray);
	CGameDevice::drawString (s_efficiency, s, t); 	
	sprintf (buf, "%.1f%%", game->getEfficiency ());
	CGameDevice::setColor (CGameDevice::yellow);
	CGameDevice::drawString (buf, s + CGameDevice::stringWidth (s_efficiency), t);
	
	menu.draw (); 
	
}
