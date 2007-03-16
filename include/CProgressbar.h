#ifndef _CPROGRESSBAR
#define _CPROGRESSBAR

#include <CWidget.h>
#include <utils.h>

class CProgressbar : public CWidget {

	int current, max;
   static const int DEFAULT_MAX = 100;
	
	int start, end;
	double step;   
	
   static const int d = 5;

	void calc_end () { end = (int)(start + step * current + 0.5); }


public:
	
	CProgressbar () {
		max = DEFAULT_MAX;
		current = 0;
	}
	CProgressbar (int left, int bottom, int right, int top, int max=DEFAULT_MAX, int cur=0);
	void setLocation (int left, int bottom, int right, int top);

	void draw ();
	void reset () { setCurrent (0); }

	int getMax () const { return max; }
	int getCurrent () const { return current; }

	void setCurrent (int c) {       
      current = c;
      clip (0, current, max);
      calc_end ();      
	}

	void input () {}

};

#endif
