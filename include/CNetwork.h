#ifndef _CNETWORK
#define _CNETWORK

#include <typedefs.h>

class CNetwork {
	
	ListOfNodes nodes;
	ListOfEdges edges;
	
	public:
		
		CNode* addNode (int x, int y);
		CEdge* addEdge (CNode* n1, CNode* n2);				
		ListOfNodes& getNodes() { return nodes; }		
		~CNetwork ();		
				
};

void getEndPoints (ListOfNodes& endpoints, CNetwork* network);

#endif
