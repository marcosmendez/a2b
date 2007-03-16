#include <CNode.h>
#include <utils.h>

int CNode::NUM_SLOTS = 0;
int CNode::SLOT_DISTANCE = 0;

CNode::CNode (int _x, int _y) {
		
	x = _x;
	y = _y;
	numConnections = 0;
}

void CNode::getSlot (int& sx, int& sy) {

	int n = slotx.size (); 
	if (n == 0) { // create slots
		int dtheta = 360 / NUM_SLOTS;
		double rdtheta = deg2rad (dtheta), rtheta = 0;
		for (int i = 0, theta = 0; i < NUM_SLOTS; i++) {
			slotx.push_back (x + SLOT_DISTANCE * cos (rtheta));
			sloty.push_back (y + SLOT_DISTANCE * sin (rtheta));
			rtheta += rdtheta;					
		}
		n = slotx.size(); 
	}
		
	list<int>::iterator i = slotx.begin (), j = sloty.begin ();
	for (int p = 0, q = getRand8() % n; p < q; p++, ++i, ++j);		
	sx = *i; sy = *j;
	slotx.erase (i);
	sloty.erase (j);
		
}


void connect (CNode* n1, CNode* n2) {

	n1->connect (n2);
	n2->connect (n1);
	
}

void disconnect (CNode* n1, CNode* n2) {

	n1->disconnect (n2);
	n2->disconnect (n1);
	
}
