#ifndef _CCOLORPRODUCER
#define _CCOLORPRODUCER

#include <CSpline.h>
#include <utils.h>
#include <vector>
#include <string>
using namespace std;

class CColorProducer { 

	/*
		used for producing track colors. doesnt produce greeny colors bcos it 
		conflicts with selected train/track. 
	*/
	
	// color info needs to be loaded only once 
	static bool isComponentsLoaded;		
	static struct tagComponent {
		
		CSpline spline; // component curve
		CBox box; // box holding component curve
		void load (ifstream& file); // component loader			
		
	} component [3];	
	
	static int greenOut1, greenOut2; // section of greens to ignore
	static int componentMin, componentMax, componentRange; // extents of color components
	static int COMPONENT_MAX; // max possible value for component	
	static double COMPONENT_MAX_INVERSE; // 1.0 / COMPONENT_MAX
	
	vector<int> degrees;
	int degree, complement;	
	int picked;
	
	int getComponent (int i, int x);	
	
	public:
		
		CColorProducer (string filename="../data/color.txt");
		void operator() (double& r, double& g, double& b);				
	
};

#endif
