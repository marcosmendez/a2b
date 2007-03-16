#include <CPause.h>
#include <CPauseMenu.h>
#include <CGame.h>
#include <main.h>

void CPause::init () {

	menu.init ();
	esc = false;
	rmbExists = false;

}

void CPause::eval () {
	
	game->draw (); 
	menu.draw (); 	
	menu.input ();
	
	if (CGameDevice::MOUSE_RMB) rmbExists = true; else if (rmbExists) { doPlay (true); return; }		
	if (isKeyPressed (CGameDevice::ESC, esc)) doAbort ();
	
}
