#ifndef _CEDGE
#define _CEDGE

class CEdge {

public:

	CNode* n1;
	CNode* n2;

	CEdge (CNode* _n1, CNode* _n2) {

		n1 = _n1;
		n2 = _n2;
		connect (n1, n2);		
	}
	
	~CEdge () {
		disconnect (n1, n2);
	}


};

#endif
