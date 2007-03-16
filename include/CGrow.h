#ifndef _CGROW
#define _CGROW

class CNetwork;
class CNode;

class CGrow { 

	/*
		grows another node from a node
		
		180* --------growableNode------- 0* 
							*	| *
				ea	*		|   * sa
								connectedNode    
							
			sa = start angle 
			ea = end angle 
			* * = branch length		
			angle range = -90 to 270
			and sa <= ea
			
	*/	
	
	int minLength, maxLength;	
	int startAngle, endAngle;	
	static const int stepAngle = 1;
	
	double cosTheta, sinTheta;
	
	public:
	
		CGrow (int sa, int ea, int minl, int maxl);
		CNode* operator() (CNode* node, CNetwork* network);	

};

#endif
