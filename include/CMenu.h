#ifndef _CMENU
#define _CMENU

class CMenu {
public:
	virtual void draw ()=0;
	virtual void input ()=0;
	virtual void init ()=0;
	virtual ~CMenu () {}
};

#endif
