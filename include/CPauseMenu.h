#ifndef _CPAUSEMENU
#define _CPAUSEMENU

#include <CMenu.h>
#include <CTextButton.h>
#include <vector>
using namespace std;

class CPauseMenu : public CMenu {	

	int left, bottom; 
	int selection; 
	vector <CTextButton> menuItems;

public:
	CPauseMenu ();
	void draw ();
	void init ();
	void input ();
	enum {INVALID = -1, RESUME = 0, REPLAY = 1, ABORT = 2};
};

#endif
