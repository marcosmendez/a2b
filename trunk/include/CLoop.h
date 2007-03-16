#ifndef _CLOOP
#define _CLOOP
class CLoop {
public:
	virtual void init ()=0;
	virtual void eval ()=0;
	virtual void time (int64_t t) {
		extern int64_t TIME;
		TIME += t;
	}
};
#endif
