#include <CBox.h>
#include <utils.h>

bool CBox::inside (const CBox& box) const {

	if (inbox(left, bottom, box)) return true;
	else if (inbox(right, top, box)) return true;
	else if (inbox(left, top, box)) return true; 
	else if (inbox(right, bottom, box)) return true;
	else return false;

}
