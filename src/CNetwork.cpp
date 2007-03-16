#include <CNetwork.h>
#include <CNode.h>
#include <CEdge.h>
#include <utils.h>

#include <iostream>

CNetwork::~CNetwork () {

	clear<ListOfEdges, CEdge>(edges); // must clear edges b4 nodes since ~CEdge disconnects nodes
	clear<ListOfNodes, CNode>(nodes);				
}								


CNode* CNetwork::addNode (int x, int y) {

	CNode* n = new CNode (x, y);
	nodes.push_back (n);
	return n;
	
}

CEdge* CNetwork::addEdge (CNode* n1, CNode* n2) {

	CEdge* e = new CEdge (n1, n2);
	edges.push_back (e);
	return e;
	
}
		
void getEndPoints (ListOfNodes& endpoints, CNetwork* network) {
	
	endpoints.clear ();
	ListOfNodes& nodes = network->getNodes ();
	for (ListOfNodesIterator i = nodes.begin (), j = nodes.end (); i != j; ++i) {
		CNode* n = *i;
		if (n->getNumConnections () < 2) endpoints.push_back (n);
	}	
	
}
