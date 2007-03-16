#include <CProgressbar.h>
#include <CBox.h>
#include <CGameDevice.h>
#include <GL/gl.h>

CProgressbar::CProgressbar (int left, int top, int right, int bottom, int m, int c) {
	
   if (m <= 0) m = DEFAULT_MAX; max = m;
	if (c < 0) c = 0; current = c;
	setLocation (left, bottom, right, top);	
	
}

void CProgressbar::setLocation (int left, int bottom, int right, int top) {
	
	CWidget::setLocation (left, bottom, right, top);
	step = (right - left - 2 * d) * 1.0 / max;
	start = left + d;
	calc_end ();	
	
}

void CProgressbar::draw () {  

   CGameDevice::setColor (CGameDevice::weakGray);
	const CBox& loc = getLocation ();
   glRecti (loc.left, loc.bottom, loc.right, loc.top);
   
   CGameDevice::setColor (CGameDevice::red);
   glRecti (start, loc.bottom + d, end, loc.top - d);
	

}
