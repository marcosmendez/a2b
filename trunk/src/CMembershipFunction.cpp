#include <CMembershipFunction.h>
#include <string>
using namespace std;

CMembershipFunction::CMembershipFunction (ifstream& file) : membership (0) {
	load (file);
}

void CMembershipFunction::load (ifstream& file) {		

	string ignore; file >> ignore;	
	file >> ignore >> quantity;
	spline.load (file);

}

void CMembershipFunction::fuzzify (int x) {

	membership = 0;
	int y; if (getY (y, spline, x)) membership = y;
	
}
