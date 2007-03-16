#ifndef _CTRACKGENERATOR
#define _CTRACKGENERATOR

#include <CGrow.h>
#include <CColorProducer.h>
#include <typedefs.h>

class CNetwork;
class CGrow;

class CTrackGenerator {	

	/*
		
		the track generator is reponsible for feeding the game 
		with required number of tracks each level.
		
		to do this, it builds networks (trees in graph theory) 
		of nodes. each node is a point in 2d-space. tracks 
		(paths in graph theory) are then harvested from these 
		nodes. because the network is a tree, there is only one 
		path (and hence a track) between two end points 
		(nodes that have only 1 connection).
		
		a network initially contains two nodes (both are end points). 
		one is sown on the border of box CGameDevice::ARENA, the other 
		shoots up (up being relative to where its sown on the box). 
		
		on each iteration, we grow the end points (except the sown end point) 
		using the growing methods to produce more end points. after a desired 
		number of iterations, we harvest the tracks from this network and add
		them to the list of available tracks and provide the required number
		to the game.
		
	*/

	int iterations;	// number of times to grow the end points of a network 
		
	ListOfNetworks networks; // list of all networks used
	ListOfNodes endpoints; // end points of current network
	unsigned int numEndPoints; // number of end points in current network
	list<CGrow> grows; // list of growing methods applied to end points of network each iteration to produce more end points
	
	int twigLength; // every new network is sown as a twig with 2 nodes. twig length is the distance between these nodes.			
	
	unsigned int numAvailableTracks;
	ListOfTracks tracks; // list of available tracks
	CColorProducer colorProducer;	 // determines track color		
	
	public:				
		enum { NUM_INITIAL_TRACKS = 7, TRACKS_INC = 2 }; // could have got from settings.txt - defined here to not let user access arbitrary level thru settings		
		CTrackGenerator (string filename);
		~CTrackGenerator ();
		bool operator() (ListOfTracks& gameTracks, int level); // load tracks into game for given level
		void newNetwork (); // create new network of nodes (tracks will be harvested from them later)
		bool getTrack (CNode* ancestor, CNode* parent, CNode* child, ListOfNodes& trackPoints); // get a unique track issuing out of parent
		bool isTrackAvailable (CNode* desiredEndPoint0, CNode* desiredEndPoint1); // is track with desired end points already on list of tracks?
	
};

#endif
