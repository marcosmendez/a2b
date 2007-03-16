#ifndef _HISCORE
#define _HISCORE

#include <string>
using namespace std;

typedef struct tagHiScore {

	string player;
	int score;
	double efficiency;			
	int levelsCompleted;
	
	tagHiScore (string& p, int s, double e, int l) : player(p), score(s), efficiency(e), levelsCompleted (l) {}
	tagHiScore () {}		
		
} HiScore;


struct SEfficiency  {
	bool operator() (const HiScore& hs1, const HiScore& hs2) {
		if (hs1.efficiency < hs2.efficiency) return true; else return false;
	}
};

struct SScore {
	bool operator() (const HiScore& hs1, const HiScore& hs2) {
		if (hs1.score < hs2.score) return true; else return false;
	}
};

#endif
