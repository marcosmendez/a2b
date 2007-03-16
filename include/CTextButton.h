#ifndef _CTEXTBUTTON
#define _CTEXTBUTTON

#include <CButton.h>
#include <CGameDevice.h>
#include <GL/gl.h>

class CTextButton : public CButton {

	string label;

public:
	
	CTextButton (string l) : label(l) { 
		init(); 
	}

	CTextButton (string l, int left, int bottom) : label (l) {
		setLocation (left, bottom);
	}	

	const string& getLabel () { return label; }

	void setLabel (const string& l) { 
		label = l; 
		const CBox& loc = getLocation ();
		setLocation (loc.left, loc.bottom);
	}

	void setLocation (int left, int bottom) {
		CWidget::setLocation (left, bottom, left + CGameDevice::stringWidth (label), bottom + CGameDevice::stringHeight ());		
	}
	
	void draw () {      
      
      if (isHovered()) glColor3f (1, 1, 0); else glColor3f (0, 1, 1);
      const CBox& loc = getLocation ();
      CGameDevice::drawString (label, loc.left, loc.bottom);
      
	}

};
#endif
