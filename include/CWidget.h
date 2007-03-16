#ifndef _CWIDGET
#define _CWIDGET

#include <CBox.h>
#include <utils.h>

class CWidget {

	CBox loc;

public:
	CWidget () {}
	CWidget (int left, int bottom, int right, int top): loc (left, bottom, right, top) {}
	void setLocation (int left, int bottom, int right, int top) { loc = CBox (left, bottom, right, top); } 
	const CBox& getLocation () { return loc; }
	bool hittest (int x, int y) { return inbox (x, y, loc); }
	virtual void draw()=0;
	virtual ~CWidget() {}
};
#endif
