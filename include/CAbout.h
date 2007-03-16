#ifndef _CABOUT
#define _CABOUT

#include <CTextButton.h>
#include <CLoop.h>

class CGame;

class CAbout : public CLoop {

	CTextButton goback;
	bool esc;

public:
	CAbout ();
	virtual ~CAbout () {}
	void init ();
	void eval ();
	void draw ();
};

#endif
