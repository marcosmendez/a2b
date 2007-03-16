#ifndef _CTRACK 
#define _CTRACK
	
#include <CSpline.h>
#include <typedefs.h>

class CNode;
class CTrain;

class CTrack {		

	public:	
		
		static int WAYPOINT_SPACING;
		
		enum {FREE = 0, OCCUPIED_OUT = 1, OCCUPIED_RETURN = 2};

		CTrack (ListOfNodes& trackPoints, double r, double g, double b);
		~CTrack () { delete[] va; }
		
		void addTrain (CTrain* train);
		void removeTrain (CTrain* train);
		bool isLastTrain (CTrain* train);
		ListOfTrains& getTrains () {return trains;}
		void getWaypointParams (CTrain* train, int& start1, int& end1, int& inc1, int& start2, int& end2, int& inc2);
		void setOccupied (int _occupied) { occupied = _occupied;}
		int getOccupied () {return occupied;}
		
		int64_t getMinTime () { return MIN_TIME; }
		int64_t getMaxTime () { return MAX_TIME; }
		
		const vector<int>& get_x () { return spline.get_hix(); }			
		const vector<int>& get_y () { return spline.get_hiy(); }
		int get_n () { return spline.get_hn(); }						
		
		void draw ();						
		
		CNode* getNode (int i) {
			return nodes[i];				
		}				
		
		CSpline& getSpline () { return spline; }
	
		const color& getColor () { return c; }
		
		
private:
	
	ListOfTrains trains; // trains on this track
	int numTrains; // num trains on this track
	int64_t MIN_TIME, MAX_TIME; // min/max time required by train to complete track
	int occupied; // is track occupied by train?	
	
	color c; // track color
	
	CSpline spline;
	
	// vertex array of low-res points of spline which is used to draw the track
	int* va;
	int nv;
	
	CNode* nodes[2]; 		
	
};

void getTracksContainingNode (ListOfTracks& nodeTracks, CNode* node, ListOfTracks& tracks);

#endif
