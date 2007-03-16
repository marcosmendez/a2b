#ifndef _CHISCORES
#define _CHISCORES

#include <CCheckBox.h>
#include <CTextButton.h>
#include <CLoop.h>
#include <vector>

class CHiScores : public CLoop {
	
	CCheckBox score, efficiency;
	CTextButton back;	
	bool esc;	

public:
	CHiScores ();
	virtual ~CHiScores () {}
	void init ();
	void eval ();
	void draw ();
};

#endif
