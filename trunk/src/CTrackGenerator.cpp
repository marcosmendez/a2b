#include <CTrackGenerator.h>
#include <CGrow.h>
#include <CNode.h>
#include <CNetwork.h>
#include <CTrack.h>
#include <CColorProducer.h>
#include <CGameDevice.h>
#include <utils.h>

#include <fstream>
#include <string>
using namespace std;

CTrackGenerator::CTrackGenerator (string filename) {	

	/*
		load params from file
	*/
	
	string ignore;	
	ifstream generatorFile (filename.c_str(), ios::in);
	
	generatorFile >> ignore >> iterations;	
	generatorFile >> ignore >> twigLength;	
	
	int numbranches; 
	generatorFile >> ignore >> numbranches;
	for (int i = 0; i < numbranches; i++) {	
		int startAngle, endAngle, minLength, maxLength;
		generatorFile >> ignore >> ignore >> startAngle >> ignore >> endAngle >> ignore >> minLength >> ignore >> maxLength;
		grows.push_back (CGrow (startAngle, endAngle, minLength, maxLength));
	}
	
	numAvailableTracks = 0;	
	numEndPoints = 0;		
	
}

CTrackGenerator::~CTrackGenerator () {

	clear <ListOfTracks, CTrack> (tracks); 
	clear <ListOfNetworks, CNetwork> (networks); 
	
}

bool CTrackGenerator::operator() (ListOfTracks& gameTracks, int level) { 

	/*
		load tracks into game for level
	*/
	
	unsigned int numTracksRequired = NUM_INITIAL_TRACKS + TRACKS_INC * level; // number of tracks required for level 
	
	while (numAvailableTracks < numTracksRequired) {		
		
		if (numEndPoints == 0) newNetwork (); 						
		
		//
		// get a random track from the current network
		//
		
		unsigned int pick = getRand16 () % numEndPoints;
		CNode* root = get<ListOfNodes, CNode*> (endpoints, pick);		
		ListOfNodes trackpoints; 
		if (getTrack (root, root, root, trackpoints) == false) {
			erase (endpoints, pick);
			--numEndPoints;
		}	else ++numAvailableTracks;		
		
	}	
	
	gameTracks.clear ();
	unsigned int numGameTracks = 0;
	for (ListOfTracksIterator i = tracks.begin (), j = tracks.end (); i != j; ++i) {
		gameTracks.push_back (*i);
		if (++numGameTracks == numTracksRequired) break;
	}
	
	cout <<"created level "<< level << " with "<< numGameTracks << " tracks " << endl;	
	
	return true;
	
}		

bool CTrackGenerator::getTrack (CNode* ancestor, CNode* parent, CNode* child, ListOfNodes& trackpoints) {		
		
		/*
			gets a track issuing out of node parent
		*/		
		
		trackpoints.push_back (child);			
		
		int numChildConnections = child->getNumConnections();		
		
		if ((numChildConnections > 1) || (parent == child)) {
			
			list<int> childConnections; 
			for (int i = 0; i < numChildConnections; i++) childConnections.push_back (i);			
			
			while (numChildConnections) {				
			
				unsigned int pick = getRand16 () % numChildConnections--;
				unsigned int ci = get <list<int>, int> (childConnections, pick);				
				erase (childConnections, pick); 
				
				CNode* childChild = child->getConnectedNode (ci); 
				if (childChild != parent) if (getTrack (ancestor, child, childChild, trackpoints)) return true; 
				
			}
			
		} else if (isTrackAvailable (ancestor, child) == false) {
			
			double r, g, b; colorProducer (r, g, b); 
			CTrack* t = new CTrack (trackpoints, r, g, b);
			tracks.push_back (t);
			return true;			
		} 
		
		trackpoints.pop_back ();		
		return false;

}

bool CTrackGenerator::isTrackAvailable (CNode* desiredEndPoint0, CNode* desiredEndPoint1) {

	/*
		true if a track with desired end points is found
	*/
	
	for (ListOfTracksIterator i = tracks.begin (), j = tracks.end(); i != j; ++i) {
		CTrack* t = *i;
		CNode* endPoint0 = t->getNode(0);
		CNode* endPoint1 = t->getNode(1);
		if ( 
				((endPoint0 == desiredEndPoint0) || (endPoint0 == desiredEndPoint1)) &&
				((endPoint1 == desiredEndPoint0) || (endPoint1 == desiredEndPoint1))
			 ) return true;
	}
	return false;
	
}

void CTrackGenerator::newNetwork () {		
	
	CNetwork* network = new CNetwork ();
		
	unsigned int xx[2] = {CGameDevice::ARENA.left, CGameDevice::ARENA.right};
	unsigned int yy[2] = {CGameDevice::ARENA.bottom, CGameDevice::ARENA.top};
	
	unsigned int x1, y1, x2, y2;
	unsigned int dx2, dy2;					
	
	if (getRandBit()) {	
		x1 = xx[getRandBit()];
		y1 = CGameDevice::ARENA.bottom + getRand16() % CGameDevice::ARENA.height;
		if (x1 == xx[0]) dx2 = twigLength; else dx2 = -twigLength;
		dy2 = 0;
	} else {
		y1 = yy[getRandBit()];
		x1 = CGameDevice::ARENA.left + getRand16() % CGameDevice::ARENA.width;		
		if (y1 == yy[0]) dy2 = twigLength; else dy2 = -twigLength;
		dx2 = 0;
	}						
	x2 = x1 + dx2;
	y2 = y1 + dy2;	

	CNode* n1 = network->addNode (x1, y1);
	CNode* n2 = network->addNode (x2, y2);
	network->addEdge (n1, n2);				
	
	for (int i = 0; i < iterations; i++) {
		getEndPoints (endpoints, network);
		for (ListOfNodesIterator iter = ++endpoints.begin(), jter = endpoints.end(); iter != jter; ++iter)
			for (list<CGrow>::iterator pter = grows.begin (), qter = grows.end(); pter != qter; ++pter)
				(*pter)(*iter, network);		
	}		
	
	networks.push_back (network);							
	getEndPoints (endpoints, network);
	numEndPoints = endpoints.size ();					
	
}
