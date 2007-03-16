#ifndef _CEVENT
#define _CEVENT

#include <typedefs.h>

class CEvent {
	bool finished;
public:
	CEvent () { finished = false; }
	bool isFinished () { return finished; }
	void setFinished (bool f) { finished = f; }
	virtual void eval ()=0;
	virtual void draw ()=0;
	virtual ~CEvent () {}
};

template<class T> void evalEvents (T& events) {
	/*
		eval and remove finished events
	*/
	
	typedef typename T::iterator Ti;	
	int i = 0;
	for (Ti iter = events.begin (), jter, eter = events.end (); iter != eter; ++iter, ++i) {
		CEvent *e = *iter;
		e->eval ();
		if (e->isFinished ()) {
			events.erase (iter);
			delete e;
			if (i == 0) iter = events.begin (); else iter = jter;
		}
		jter = iter;
	}
	
}

template<class T> void drawEvents (T& events) {
	typedef typename T::iterator Ti;
	for (Ti i = events.begin (), j = events.end(); i != j; ++i) (*i)->draw ();	
}

#endif
