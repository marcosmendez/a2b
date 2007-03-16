#include <CColorProducer.h>
#include <algorithm>

#include <iostream>

CColorProducer::tagComponent CColorProducer::component[3];
int CColorProducer::greenOut1, CColorProducer::greenOut2;
int CColorProducer::componentMin, CColorProducer::componentMax, CColorProducer::componentRange;
int CColorProducer::COMPONENT_MAX;
double CColorProducer::COMPONENT_MAX_INVERSE;
bool CColorProducer::isComponentsLoaded = false;

CColorProducer::CColorProducer (string filename) {
	
	// load color info only once
	if (isComponentsLoaded == false) {
		
		string ignore;
		ifstream file (filename.c_str(), ios::in);
		
		file >> ignore >> greenOut1;
		file >> ignore >> greenOut2;		
		
		file >> ignore;	
		for (int i = 0; i < 3; i++) {
			string componentFile; file >> componentFile;
			ifstream cf (componentFile.c_str(), ios::in);
			component[i].load (cf);
		}	
		
		file >> ignore >> componentMin;
		file >> ignore >> componentMax;
		componentRange = componentMax - componentMin;		
		
		file >> ignore >> COMPONENT_MAX;
		COMPONENT_MAX_INVERSE = 1.0 / COMPONENT_MAX;
		
		isComponentsLoaded = true;
		
	}
	
	picked = -1;
	complement = -1;	
				
}

void CColorProducer::operator() (double& r, double& g, double& b) {	

	/*
		randomly pick a degree (or complement if available) on the 
		color wheel, find the color by interpolating component splines. 				
	*/			
	
	if (complement == -1) {
		
		int nd = degrees.size();
		if (nd == 0) {
			for (int i = greenOut1; i <= greenOut2; i++) degrees.push_back (i);
			nd = degrees.size();
		}		
		
		static const int advance = 30;
		if (picked == -1) picked = getRand16() % nd; else picked = (picked + advance) % nd; 
		degree = degrees [picked]; 		
		complement = 180 + degree;
		if (complement > 360) complement -= 360;
		if ((complement < greenOut1) || (complement > greenOut2)) complement = -1;  // cut out green bcos conflicts with selected train/track 
		erase< vector<int> >(degrees, picked);
		
		
	}	else {
	
		degree = complement;
		vector<int>::iterator iter = find (degrees.begin(), degrees.end(), complement);	
		degrees.erase(iter);		
		complement = -1;		
	}	
	
	// get color at degree on wheel 
	int ir = getComponent (0, degree);
	int ig = getComponent (1, degree);
	int ib = getComponent (2, degree);
	
	// randomly move color to min (black) or max (white) to get unique color
	int c; if (getRandBit()) c = componentMin; else c = componentMax;
	int dr = c - ir, dg = c - ig, db = c - ib;
	int bias = getRand7() >> 1;
	ir = ir + ((bias * dr) >> 7);
	ig = ig + ((bias * dg) >> 7);
	ib = ib + ((bias * db) >> 7);
	
	// convert int color to range <0,1> for use in opengl
	r = COMPONENT_MAX_INVERSE * ir;
	g = COMPONENT_MAX_INVERSE * ig;
	b = COMPONENT_MAX_INVERSE * ib;
				
}
	
int CColorProducer::getComponent (int i, int x) {
	
	int y = componentMin; 
	if (getY (y, component[i].spline, x)) 
		return (componentMin + y * componentRange / component[i].box.height); 
	else return y;	
	
}

void CColorProducer::tagComponent::load (ifstream& file) { // load component

	string ignore;
	int left, bottom, right, top;
	file >> ignore >> left >> bottom >> right >> top;
	box = CBox (left, bottom, right, top);
	spline.load (file);			
	
}
