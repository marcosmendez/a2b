#ifndef _CLABEL
#define _CLABEL

#include <CWidget.h>
#include <CGameDevice.h>

#include <string>
using namespace std;

class CLabel : public CWidget {
	
	string label;	

public:
	
	CLabel (const string& l, int left, int bottom) {
		label = l;
		setLocation (left, bottom, left + CGameDevice::stringWidth (label), bottom + CGameDevice::stringHeight());
	}
	CLabel (const string& l) : label (l) {}	
	CLabel () {}
	
	const string& get () { 	
		return label; 		
	}	
	
	void set (const string& l) { 	
		label = l; 		
	}	
	
	void draw () {
		
		CGameDevice::setColor (CGameDevice::gray); 
		const CBox& loc = getLocation ();
		CGameDevice::drawString (label, loc.left, loc.bottom);
		
	}

};
#endif
