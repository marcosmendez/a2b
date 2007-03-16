#include <CFuzzyVariable.h>
#include <utils.h>

CFuzzyVariable::CFuzzyVariable (const string& filename) {	
	ifstream file (filename.c_str (), ios::in);	
	load (file);
}

CFuzzyVariable::~CFuzzyVariable () {
	clear<vector<CMembershipFunction *>, CMembershipFunction>(mfs);
}

void CFuzzyVariable::load (ifstream& file) {
	
	string ignore; file >> ignore;
	
	int left, top, right, bottom;
	file >> ignore >> left >> top >> right >> bottom;
	box = CBox (left, top, right, bottom);	
	
	int n; file >> ignore >> n;
	for (int i = 0; i < n; i++) mfs.push_back (new CMembershipFunction(file));
}

void CFuzzyVariable::fuzzify (int x) {
	
	for (int i = 0, j = mfs.size (); i < j; i++) mfs[i]->fuzzify (x); 

}

double CFuzzyVariable::getCrisp () {
	
	/*
		crisp is the output of membership function with max membership
	*/
	
	int n = mfs.size (); if (n == 0) return 0; 
	CMembershipFunction* mf0 = mfs[0];
	int maxMembership = mf0->getMembership ();
	double maxProduct = mf0->getProduct ();			
	for (int i = 0; i < n; i++) {
		int membership = mfs[i]->getMembership ();
		if (membership > maxMembership) {
			maxMembership = membership;
			maxProduct = mfs[i]->getProduct ();
		}
	}
	
	return (maxProduct / box.height );	
	
}
