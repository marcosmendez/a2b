#ifndef _CPAUSE
#define _CPAUSE

#include <CPauseMenu.h>
#include <CLoop.h>

class CGame;

class CPause : public CLoop {

	CGame* game;
	CPauseMenu menu;
	bool esc;
	bool rmbExists;

public:
	enum {RESUME=200, REPLAY, ABORT};
	virtual ~CPause () {}
	void init ();
	void eval ();
	virtual void time (int64_t t) {}
	void setGame (CGame* _game) { game = _game; }
	CPause (CGame* _game) { setGame (_game); }
};

#endif
