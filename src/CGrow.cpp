#include <CGrow.h>
#include <CNode.h>
#include <CNetwork.h>
#include <utils.h>
#include <CGameDevice.h>
#include <list>
using namespace std;


CGrow::CGrow (int sa, int ea, int minl, int maxl) {
	
	startAngle = sa;
	endAngle = ea;		
	minLength = minl;
	maxLength = maxl;
	
	double rtheta = deg2rad (stepAngle);
	cosTheta = cos (rtheta);
	sinTheta = sin (rtheta);		
	
}

CNode* CGrow::operator() (CNode* node, CNetwork* network) { 
	/*
		grow another node (ie grown node) from node
	*/
	
	if (node->getNumConnections ()) {			
	
		CNode* connectedNode = node->getConnectedNode (0); 		
		double uvx, uvy; findUnitVector<double>(uvx, uvy, connectedNode->x, connectedNode->y, node->x, node->y); // vector joining connected node and node
		double unx = uvy, uny = -uvx;	// normal, x is +ve, normal end point is 0 degrees 
		list<int> lx, ly;
		
		//
		// find possible grown nodes
		//
		
		double urx, ury;
		double rsa = deg2rad (startAngle);
		urx = unx * cos (rsa) - uny * sin (rsa);
		ury = uny * cos (rsa) + unx * sin (rsa);
		
		for (int i = startAngle; i <= endAngle; i++) {
			
			urx = urx * cosTheta - ury * sinTheta;
			ury = ury * cosTheta + urx * sinTheta;
			
			// possible grown node ie CNode (gx, gy)
			double gx = node->x + maxLength * urx;
			double gy = node->y + maxLength * ury;
			clip<double>(gx, gy, node->x, node->y, CGameDevice::ARENA);
			if ( findMagnitude<double> (gx, gy, node->x, node->y) >= minLength)  { // valid grown node
				lx.push_back (gx);
				ly.push_back (gy);
			}						
			
		}
		
		// randomly pick a candidate grown node
		unsigned int n = lx.size (); 
		if (n) {
			unsigned int q = getRand16() % n;
			list<int>::iterator lxi = lx.begin(), lyi = ly.begin();
			for (unsigned int p = 0; p < q; p++, ++lxi, ++lyi);
			CNode* grownNode = network->addNode(*lxi, *lyi);
			network->addEdge (node, grownNode);
			return grownNode;
		}									
		
	}		
	return 0;			
	
}
