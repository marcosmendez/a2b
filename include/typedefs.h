#ifndef _TYPEDEFS
#define _TYPEDEFS

#include <list>
using namespace std;

class CTrack;
class CTrain;
class CGame;
class CEvent;
class CNode;
class CEdge;
class CNetwork;

typedef list<CTrack*> ListOfTracks;
typedef ListOfTracks::iterator ListOfTracksIterator;
typedef list <CTrain *> ListOfTrains;
typedef ListOfTrains::iterator ListOfTrainsIterator;
typedef list <int64_t> ListOfInt64;
typedef ListOfInt64::iterator ListOfInt64Iterator;
typedef list <CEvent *> ListOfEvents;
typedef ListOfEvents::iterator ListOfEventsIterator;
typedef list<CNode*> ListOfNodes;
typedef list<CEdge*> ListOfEdges;
typedef ListOfNodes::iterator ListOfNodesIterator;
typedef ListOfEdges::iterator ListOfEdgesIterator;
typedef list<CNetwork*> ListOfNetworks;
typedef ListOfNetworks::iterator ListOfNetworksIterator;

typedef struct tagColor {
   double r, g, b;
   tagColor (double rr, double gg, double bb) : r(rr), g(gg), b(bb) {}
	tagColor () {}
} color;

#endif
