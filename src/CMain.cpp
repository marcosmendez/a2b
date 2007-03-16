
#include <CMain.h>
#include <CGame.h>
#include <main.h>

void CMain::init () {

	menu.init (); 
	levelbrowser.init (); 
	esc = false; 

}

void CMain::eval () {

	game->drawTracks (); 
	menu.draw (); 
	levelbrowser.draw (); 
	
	levelbrowser.input (); 
	menu.input ();	
	if (isKeyPressed (CGameDevice::ESC, esc)) doExit ();

}
