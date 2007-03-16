#include <utils.h>
#include <CSpline.h>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

void CSpline::load (std::ifstream& file) {

	/*
		load spline from file
	*/
	
	string ignore;	

	// add cvs
	int ncvs = 0; file >> ignore >> ignore >> ncvs;
	//console()<<"numcvs="<<ncvs<<endl;
	for (int j = 0; j < ncvs; j++) {					
		int x, y; file >> ignore >> x >> y;
		addCV (x, y);
		//console()<<"x="<<x<<"y="<<y<<endl;
	}
	
	// add in and out tangents
	int ntangents; file >> ignore >> ntangents;
	ntangents >>= 1;
	for (int j = 0; j < ntangents; j++) {
		int x, y;  file >> ignore >> x >> y; 
		//console()<<"it x="<<x<<"y="<<y<<endl;
		addInTangent (x, y); 
		file >> ignore >> x >> y;
		//console()<<"ot x="<<x<<"y="<<y<<endl;
		addOutTangent (x, y);
	}

	build (); 

}

void CSpline::build () {
	
	// generate lo res points
  lox = cvx;
	loy = cvy;

	int n = cvx.size ();
	for (int j = n - 1, i = j - 1; j > 0; --j, --i) {
		
		int x1 = cvx [i], y1 = cvy [i];
		int tx1 = otx [i], ty1 = oty [i];

		int x2 = cvx [j], y2 = cvy [j];
		int tx2 = itx [j], ty2 = ity [j];		
		
		losubd (x1, y1, x2, y2, tx1, ty1, tx2, ty2, x1, y1, x2, y2, 0.0, 1.0, j, 0);

	}		
	
	// generate hi res points
	
	hix = lox;
	hiy = loy;

	for (int j = lox.size () - 1, i = j - 1; j > 0; j--, i--) {

		int x1 = lox[i], y1 = loy[i];
		int x2 = lox[j], y2 = loy[j];
		hisubd (x1, y1, x2, y2, j);

	}

}

void CSpline::losubd (int x1, int y1, int x2, int y2, 
								 int tx1, int ty1, int tx2, int ty2, 
								 int lx1, int ly1, int lx2, int ly2, 
								 double s0, 
								 double s1, 
								 int pos, 
								 int nsubd
								 ) {
	
	// for bezier blending fuctions	 
	double S = (s0 + s1) / 2;
	double S1 = 1 - S;
	double S1_2 = S1 * S1;
	double S1_3 = S1_2 * S1;
	double S_2 = S * S;
	double S_3 = S_2 * S;		
	double BP1 = S1_3;
	double BT1 = 3 * S * S1_2; 
	double BT2 = 3 * S_2 * S1;
	double BP2 = S_3;	

	// find point on track at S
	int cx = (int) (BP1 * x1 + BT1 * tx1 + BT2 * tx2 + BP2 * x2);
	int cy = (int) (BP1 * y1 + BT1 * ty1 + BT2 * ty2 + BP2 * y2);

	int mx = (lx1 + lx2) / 2, my = (ly1 + ly2) / 2;	
	
	int dx = cx - mx, dy = cy - my;
	int D2 = dx * dx + dy * dy;

	static const int _MAX_D2 = 2;
	if ((D2 >= _MAX_D2) || (nsubd == 0)) {

		// insert track point 
		vector <int>::iterator xter = lox.begin ();
		vector <int>::iterator yter = loy.begin ();
		for (int i = 0; i < pos; i++, ++xter, ++yter);
		lox.insert (xter, cx);
		loy.insert (yter, cy);

		losubd (x1, y1, x2, y2, tx1, ty1, tx2, ty2, cx, cy, lx2, ly2, S, s1, pos+1, 1);
		losubd (x1, y1, x2, y2, tx1, ty1, tx2, ty2, lx1, ly1, cx, cy, s0, S, pos, 1);

	} else return;								 
								 
}

void CSpline::hisubd (int x1, int y1, int x2, int y2, int pos) {

	int dx = x2 - x1;
	int dy = y2 - y1;
	int D2 = dx * dx + dy * dy;

	static const int _MAX_D2 = 4;		
	if (D2 >= _MAX_D2) {

		// insert mid point
		int mx = (x1 + x2) >> 1, my = (y1 + y2) >> 1;
		vector <int>::iterator xter = hix.begin (), yter = hiy.begin ();
		for (int i = 0; i < pos; i++, ++xter, ++yter);
		hix.insert (xter, mx);
		hiy.insert (yter, my);

		hisubd (mx, my, x2, y2, pos+1);
		hisubd (x1, y1, mx, my, pos);

	} else return;		


}

bool getY (int& y, const CSpline& s, int x) {	

	// return y on spline given x
	
	const vector<int>& vx = s.get_lox ();
	const vector<int>& vy = s.get_loy ();
	int n = s.get_ln ();		
	for (int i = 0, j = i + 1, m = n - 1; i < m; i++, j++) {		
		int x1 = vx[i], y1 = vy[i];
		int x2 = vx[j], y2 = vy[j];
		if ((x > x1) && (x < x2)) {
			//console()<<"came here"<<endl;
			double m = (y2 - y1) * 1.0 / (x2 - x1);
			y = (int) (y1 + m * (x - x1));
			//console()<<"out"<<endl;
			return true;
		} else if (x == x1) {
			y = y1; 
			return true;
		} else if (x == x2) {
			y = y2;
			return true;
		}
	}
	return false;	
	
}

void makeCatmullRomSpline (CSpline& s, int fac, int minLength, int maxLength) {	

	const vector<int>& vx = s.get_cvx ();
	const vector<int>& vy = s.get_cvy ();
	int n = s.numcvs ();		
	
	if (n < 2) return;	
	
	s.clear_tangents ();
			
	// first cv in/out tangents
	int tx, ty; calcTangent (tx, ty, vx[0], vy[0], vx[1], vy[1], fac, minLength, maxLength);
	s.addInTangent (vx[0] - tx, vy[0] - ty);
	s.addOutTangent (vx[0] + tx, vy[0] + ty);	
	
	// 2nd to last-1 cv in/out tangents
	int m = n - 1;
	for (int i = 0, j = 1, k = 2; j < m; i++, j++, k++) {
		calcTangent (tx, ty, vx[i], vy[i], vx[k], vy[k], fac, minLength, maxLength);
		s.addInTangent (vx[j] - tx, vy[j] - ty);
		s.addOutTangent (vx[j] + tx, vy[j] + ty);		
	}		
	
	// last cv in/out tangents
	int l = m - 1;
	calcTangent (tx, ty, vx[l], vy[l], vx[m], vy[m], fac, minLength, maxLength);
	s.addInTangent (vx[m] - tx, vy[m] - ty);
	s.addOutTangent (vx[m] + tx, vy[m] + ty);		
	
	// rebuild the spline
	s.build ();	
	
}

void calcTangent (int& tx, int& ty, int x1, int y1, int x2, int y2, int fac, int minLength, int maxLength) {

	/*
		ensures tangent is along vector (x1,y1) to (x2,y2) whose magnitude is in range (minLength,maxLength)
	*/
	
	int dx, dy; findVector<int>(dx, dy, x1, y1, x2, y2);
	int fx = dx / fac, fy = dy / fac;
	double fmag = findMagnitude<double> (fx, fy);
	clip<double> (minLength, fmag, maxLength);
	double ux, uy; findUnitVector<double>(ux, uy, dx, dy);
	tx = fmag * ux;
	ty = fmag * uy;	
	//console()<<"fac = "<<fac<<" minlen="<<minLength<<" maxlen="<<maxLength<<endl;
	
}
