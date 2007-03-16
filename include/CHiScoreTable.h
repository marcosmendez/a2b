#ifndef _CHISCORETABLE
#define _CHISCORETABLE

#include <HiScore.h>
#include <CWidget.h>
#include <vector>
#include <map>
#include <string>
using namespace std;

class CHiScoreTable : public CWidget {
	
	string filename;

	map < int, vector<HiScore> > hiscores;		
		
	int sortstyle;
	
	static const int MAX_ENTRIES = 10;
	enum {SCORE=0, EFFICIENCY=1};
	
	template <class T> void sortTable (const T& sortPolicy);

	public:	
		
		CHiScoreTable (const string& filename);
		~CHiScoreTable ();		
		void addEntry (const HiScore& entry);
		void sortByScore ();
		void sortByEfficiency ();
		void draw ();
		bool isSortByScore () { if (sortstyle == SCORE) return true; else return false; }	
	
};

#endif
