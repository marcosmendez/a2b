#include <CAbout.h>
#include <main.h>
#include <CGameDevice.h>

CAbout::CAbout (): goback ("Back") { 	
	
	int left = 10, bottom = CGameDevice::YMAX - 11 * CGameDevice::stringHeight ();
	goback.setLocation (left, bottom);
	
}
void CAbout::init () {

	goback.init ();
	esc = false; 

}

void CAbout::eval () {

	draw ();	
	
	// apply input 
	goback.input ();
	
	if (goback.isSelected() || isKeyPressed (CGameDevice::ESC, esc)) {
		goback.init ();
		doMain ();
	}	

} 

void CAbout::draw () {
	
	
	int s = 20, dt = CGameDevice::stringHeight (), t = CGameDevice::YMAX - 2 * dt;
	
	extern const char* APP_NAME;
	static const char* text[] = {
		APP_NAME,
		"Copyright <c> 2004 Sampath Jagannathan.",
		"Distributed under GPL License Version 2.0.",
		"See COPYING for more information.",
		" ",
		"Uses Mersenne Twister pseudorandom number", 
		"generator kindly provided by Takuji Nishimura", 
		"and Makoto Matsumoto."				
	};				
	
	CGameDevice::setColor (CGameDevice::gold); 
	for (int i = 0; i < 8; i++) {
		CGameDevice::drawString (text[i], s, t); 
		t -= dt;
	}
			
	goback.draw ();

}
