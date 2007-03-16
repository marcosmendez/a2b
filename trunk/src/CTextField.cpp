#include <CTextField.h>
#include <CGameDevice.h>
#include <clock64.h>
#include <GL/gl.h>

CTextField::CTextField (const string& t, int left, int bottom, int n) : text(t), maxChars (n) {
		
	setLocation (left, bottom);
	init ();
	
}

CTextField::CTextField (int n) : maxChars (n) {
	init ();	
}

void CTextField::setLocation (int left, int bottom) {

	int width = CGameDevice::stringWidth (maxChars + 1);
	int right = left + width, top = bottom + CGameDevice::stringHeight();
	CWidget::setLocation (left, bottom, right, top);

}

void CTextField::init () {
   
	CButton::init ();
	showCursor = false;
	t = clock64 ();   
}

void CTextField::draw () {
   
	static const int64_t ct = CGameDevice::CLOCKS_PER_SECOND >> 1;
	int64_t dt = clock64() - t;
	
	if (dt > ct) {
		showCursor = !showCursor;
		t = clock64 ();
	}	
	
	const CBox& loc = getLocation ();		
   glColor3f (0.75, 0.75, 0.75);
   CGameDevice::drawString (text, loc.left, loc.bottom);   
   if (isSelected() && showCursor) {
      int left = loc.left + (int) (CGameDevice::stringWidth (text) + 0.5) + 1;      
      glBegin (GL_LINES);
         glVertex3f (left, loc.bottom, 0);
         glVertex3f (left, loc.top, 0);         
      glEnd ();
	}
}

void CTextField::input () {
	
	CButton::input ();
	if (isSelected ()) {
		unsigned int key = CGameDevice::repeatKey ();
		if (key == CGameDevice::BACKSPACE || key == CGameDevice::DELETE) delLastChar();
			else if (key == CGameDevice::ENTER) init();
				else if (CGameDevice::isCharAvailable (key)) addChar (key);
	}
	
}

void CTextField::addChar (char c) {
	
	if (text.length () < maxChars) text.append (1, c);
	
}

void CTextField::delLastChar () {
	int len = text.length ();
	if (len) text.erase (len-1, 1);
}
