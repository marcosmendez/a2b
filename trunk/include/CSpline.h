#ifndef _CSPLINE
#define _CSPLINE

#include <vector>
#include <fstream>
using namespace std;

class CSpline {
	
	vector<int> cvx, cvy; // cvs
	vector<int> itx, ity; // in tangents
	vector<int> otx, oty; // out tangents
	
	// points of the spline
	vector<int> lox, loy; // lo res points
	vector<int> hix, hiy; // hi res points	
	
	void losubd (int x1, int y1, int x2, int y2, int tx1, int ty1, int tx2, int ty2, int lx1, int ly1, int lx2, int ly2, double s1, double s2, int pos, int nsubd);
	void hisubd (int x1, int y1, int x2, int y2, int pos);


public:

	void addCV (int x, int y) { cvx.push_back (x); cvy.push_back (y);}
	void addInTangent (int x, int y) { itx.push_back (x); ity.push_back (y); }
	void addOutTangent (int x, int y) { otx.push_back (x); oty.push_back (y); }

	void setCV (unsigned int i, int x, int y) { 
		if (i < cvx.size ()) { 
			cvx[i] = x; cvy[i] = y; 
		} 
	}

	void setInTangent (unsigned int i, int x, int y) { 
		if (i < itx.size ()) { 
			itx[i] = x; ity[i] = y; 
		} 
	}

	void setOutTangent (unsigned int i, int x, int y) { 
		if (i < otx.size ()) { 
			otx[i] = x; oty[i] = y; 
		} 
	}

	
	const vector<int>& get_cvx () const {return cvx;}
	const vector<int>& get_cvy () const {return cvy;}
	unsigned int numcvs () const {return cvx.size ();}	
	void clear_cvs () { cvx.clear (); cvy.clear (); }
	

	const vector<int>& get_itx () const {return itx;}
	const vector<int>& get_ity () const {return ity;}	
	const vector<int>& get_otx () const {return otx;}
	const vector<int>& get_oty () const {return oty;}
	unsigned int num_tangents () const { return itx.size (); }	
	void clear_tangents () { 
		itx.clear (); ity.clear ();
		otx.clear (); oty.clear ();
	}
	
	void clear_all () {
		clear_cvs ();
		clear_tangents ();
		lox.clear (); loy.clear ();
		hix.clear (); hiy.clear ();
	}	

	const vector<int>& get_lox () const { return lox; }
	const vector<int>& get_loy () const { return loy; }
	int get_ln () const { return lox.size (); }

	const vector<int>& get_hix () const { return hix; }
	const vector<int>& get_hiy () const { return hiy; }
	int get_hn () const { return hix.size (); }
	
	void load (ifstream& file);
	void build ();
	
   CSpline (string filename) { ifstream file (filename.c_str(), ios::in); load (file); }
	CSpline (ifstream& file) { load(file); }
	CSpline () {}	

};

// utils
bool getY (int& y, const CSpline& s, int x); // get y on spline given x
void calcTangent (int& tx, int& ty, int x1, int y1, int x2, int y2, int fac, int minLength, int maxLength); // used by makeCatmullRomSpline(..)
void makeCatmullRomSpline (CSpline& s, int fac=4, int minLength=10, int maxLength=100); // converts spline to catmull rom spline

#endif
