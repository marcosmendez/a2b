#ifndef _CNODE
#define _CNODE

#include <typedefs.h>
#include <utils.h>
#include <math.h>
#include <algorithm>

class CNode {
	
	int numConnections;
	ListOfNodes connections;
	
	list<int> slotx, sloty;
	void createSlots ();

public:

	static int NUM_SLOTS;
	static int SLOT_DISTANCE;
	
	int x, y;

	CNode (int _x, int _y);	

	void connect (CNode* node) {
		connections.push_back (node);
		++numConnections;
	}

	void disconnect (CNode* node) {
		
		ListOfNodesIterator end = connections.end(), cter = find (connections.begin(), end, node);
		if (cter != end) {
			connections.erase (cter);
			--numConnections;
		}
		
	}
	
	int getNumConnections () const { return numConnections; }		
	
	CNode* getConnectedNode (unsigned int j) { return get<ListOfNodes, CNode*> (connections, j); }	
	
	void getSlot (int& x, int& y);	

};

void connect (CNode* n1, CNode* n2);
void disconnect (CNode* n1, CNode* n2);

#endif
