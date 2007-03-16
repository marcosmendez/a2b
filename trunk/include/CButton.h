#ifndef _CBUTTON
#define _CBUTTON

#include <CBox.h>
#include <CWidget.h>
#include <CGameDevice.h>

class CButton : public CWidget {

	bool pressed;
	bool hovered;
	bool selected;

public:
	
	CButton () {
		init ();
	}
	
	CButton (int left, int bottom, int right, int top) : CWidget (left, bottom, right, top) {
		init ();			
	}
	
	virtual ~CButton () {}

	void press () { pressed = true; }
	void release () { pressed = false; }
	bool isPressed () { return pressed; }

	void hover () { hovered = true; }
	void dehover () { hovered = false; }
	bool isHovered () { return hovered; }

	virtual void select () { selected = true; }
	void deselect () { selected = false; }
	bool isSelected () { return selected; }

	virtual void init () {
		
		deselect ();
		release ();
		dehover ();
	}	

	void input () {      

		if (hittest (CGameDevice::MOUSE_X, CGameDevice::MOUSE_Y)) {
			hover ();
			if (CGameDevice::MOUSE_LMB) 
				press ();
			else
				if (isPressed ()) {
					select ();
					release ();
				} 
		} else {
			dehover ();
			release ();
		}

	}	

};
#endif
