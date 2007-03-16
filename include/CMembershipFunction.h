#ifndef _CMEMBERSHIPFUNCTION
#define _CMEMBERSHIPFUNCTION

#include <CSpline.h>
#include <CBox.h>

#include <fstream>
using namespace std;

class CMembershipFunction {

	double quantity;
	int membership;
	CSpline spline;

public:
	
	CMembershipFunction () {
		quantity = 1;
		membership = 0;
	}
	
	CMembershipFunction (ifstream& file);
	void load (ifstream& file);
	void fuzzify (int x);
	void setQuantity (double q) { quantity = q; }
	int getMembership () { return membership; }
	double getProduct () { return quantity * membership; }

};
#endif
