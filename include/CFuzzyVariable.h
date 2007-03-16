#ifndef _CFUZZYVARIABLE
#define _CFUZZYVARIABLE

#include <CMembershipFunction.h>
#include <vector>
#include <string>
using namespace std;

class CFuzzyVariable {
	
	vector <CMembershipFunction *> mfs; 

public:
	CFuzzyVariable (const string& filename);
	~CFuzzyVariable ();
	void load (ifstream& file);
	void fuzzify (int x);
	double getCrisp ();
	void setQuantity (int fn, double q) { mfs[fn]->setQuantity (q); }
	CBox box;

};

#endif
