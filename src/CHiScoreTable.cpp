#include <CGameDevice.h>
#include <CHiScoreTable.h>
#include <fstream>
#include <algorithm>
using namespace std;

CHiScoreTable::CHiScoreTable (const string& f) : filename(f) {
	
	/*
		read hiscores 
	*/
	string ignore;	
	ifstream hiscoresFile (filename.c_str(), ios::in); 
	
	string ss; hiscoresFile >> ignore >> ss;
	if (ss == "score") sortstyle = SCORE; else sortstyle = EFFICIENCY;
	
	
	int entry = 0;
	while (1) {
		HiScore hs;	
		hiscoresFile >> ignore >> entry; if (entry == -1) break; 
		hiscoresFile >> ignore >> hs.player;
		hiscoresFile >> ignore >> hs.score;
		hiscoresFile >> ignore >> hs.efficiency;
		hiscoresFile >> ignore >> hs.levelsCompleted;	
		vector<HiScore>& v = hiscores [hs.levelsCompleted]; v.push_back (hs);
	}

}

CHiScoreTable::~CHiScoreTable () {

	/*
		write hiscores
	*/

	ofstream hiscoresFile (filename.c_str(), ios::out);	
	if (isSortByScore()) hiscoresFile << "sortby score" << endl; else hiscoresFile << "sortby efficiency" << endl;
		
	int nentries = 0;
	
	for (map<int, vector<HiScore> >::reverse_iterator i = hiscores.rbegin (), j = hiscores.rend (); i != j; ++i) {
		vector<HiScore>& v = (*i).second;
		for (int s = v.size() - 1; s > -1; s--) {
			const HiScore& hs = v[s];
			hiscoresFile << "entry "<< nentries << endl;		
			hiscoresFile << "player "<< hs.player << endl;
			hiscoresFile << "score "<< hs.score << endl;
			hiscoresFile << "efficiency " << hs.efficiency << endl;		
			hiscoresFile << "levels_completed " << hs.levelsCompleted << endl;
			if (++nentries == MAX_ENTRIES) goto out;
		}				
	}	
			
	out:
		hiscoresFile << "entry " << -1 << endl;
		
}

void CHiScoreTable::sortByScore () {
	sortstyle = SCORE;
	sortTable (SScore());
}

void CHiScoreTable::sortByEfficiency () {
	sortstyle = EFFICIENCY;
	sortTable (SEfficiency());
}

template <class T> void CHiScoreTable::sortTable (const T& sortPolicy) {
	for (map< int, vector<HiScore> >::reverse_iterator i = hiscores.rbegin(), j = hiscores.rend (); i != j; ++i) {
		vector<HiScore>& v = (*i).second;
		sort (v.begin(), v.end(), sortPolicy);		
	}		
}

void CHiScoreTable::addEntry (const HiScore& hs) {

	/*
		add entry to hiscores table
	
		entries are sorted by score or efficiency. players with more
		completed levels are preferred to those with less.
	*/
	
	vector<HiScore>& v = hiscores [hs.levelsCompleted]; v.push_back (hs); 	 
	if (isSortByScore ()) sortByScore (); else sortByEfficiency (); 
	
}

void CHiScoreTable::draw () {

	int p = 25, dp = 100, q = p + dp, r = q + dp, s = r + 3 * dp / 2, t = CGameDevice::YMAX - 6 * CGameDevice::stringHeight (), dt = 3 * CGameDevice::stringHeight() / 2;
	const char *s_player = "Who.", *s_score = "Score.", *s_efficiency = "Efficiency.", *s_levels = "Levels played.";
	
	CGameDevice::setColor (CGameDevice::gray); 
	CGameDevice::drawString (s_player, p, t);	
	CGameDevice::drawString (s_score, q, t);  
	CGameDevice::drawString (s_efficiency, r, t); 			

	CGameDevice::drawString (s_levels, s, t);			
	
	t -= dt;
	
	CGameDevice::setColor (CGameDevice::gold);
	static char buf[256];
	int n = 0;
	for (map<int, vector<HiScore> >::reverse_iterator i = hiscores.rbegin(), j = hiscores.rend (); i != j; ++i) {
		vector<HiScore>& v = (*i).second;
		for (int i = v.size()-1; i > -1; --i) {
			HiScore& ihs = v[i];			
			
			CGameDevice::drawString (ihs.player, p, t);		
			
			sprintf (buf, "%d", ihs.score);			
			CGameDevice::drawString (buf, q, t);						
			
			sprintf (buf, "%.1f%%", ihs.efficiency);
			CGameDevice::drawString (buf, r, t);						
			
			int levelsCompleted = ihs.levelsCompleted;
			sprintf (buf, "%d", levelsCompleted);
			CGameDevice::drawString (buf, s, t);			
						
			t -= dt;		
			
			if (++n == MAX_ENTRIES) return;			
			
		}		
	}		
}
