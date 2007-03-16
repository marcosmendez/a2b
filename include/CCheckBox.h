#ifndef _CCHECKBOX
#define _CCHECKBOX

#include <CTextButton.h>

class CCheckBox : public CTextButton {

public:

	CCheckBox (const string& l, int left, int bottom) : CTextButton (l, left, bottom) {
		init ();			
	}
	
	CCheckBox (const string& l) : CTextButton (l) {
		init ();				
	}	
	
	void select () { if (isSelected ()) deselect (); else CTextButton::select (); }
	
	void draw () {
      
	 if (isSelected()) glColor3f (0, 1, 0); 
		 else if (isHovered()) glColor3f (1, 1, 0); 
			else glColor3f (0, 1, 1);
	
    const CBox& loc = getLocation ();	
    const string& label = getLabel ();
    CGameDevice::drawString (label, loc.left, loc.bottom);

	}
	

};
#endif
