#include <CNode.h>
#include <CTrack.h>
#include <CTrain.h>
#include <CGame.h>
#include <CGameDevice.h>

int CTrack::WAYPOINT_SPACING = 0;

CTrack::CTrack (ListOfNodes& trackPoints, double r, double g, double b) : c(r, g, b) {
	
	nodes[0] = *trackPoints.begin(); // source
	nodes[1] = *--trackPoints.end(); // destination

	// construct the spline 
	for (ListOfNodesIterator i = trackPoints.begin (), j = trackPoints.end (); i != j; ++i) {
		CNode *ni = *i;
		int x, y; ni->getSlot(x, y);
		spline.addCV (x, y); 
		spline.addInTangent (x, y); 
		spline.addOutTangent (x, y);
	}					
	makeCatmullRomSpline (spline);
	
	// construct vertex array of low-res points of spline which is used to draw track
	const vector<int>& lox = spline.get_lox ();
	const vector<int>& loy = spline.get_loy ();
	
	nv = spline.get_ln ();	
	va = new int [2 * nv];	
	for (int i = 0, j = 0; i < nv; i++) {
		va [j++] = lox [i];
		va [j++] = loy [i];		
	}
	
	// min/max time required to complete journey on this track
	int n = get_n ();
	MIN_TIME = (n / CTrain::MAX_SPEED) * CGame::EVAL_INTERVAL;	
	MAX_TIME = (n / CTrain::SPEED) * CGame::EVAL_INTERVAL;	
	
	setOccupied (FREE);
	numTrains = 0;			

}

void CTrack::addTrain (CTrain *train) {

	/*
		add train on track
	*/

	trains.push_back (train);
	++numTrains;

}

void CTrack::removeTrain (CTrain *train) {
	/*
		remove train from track
	*/

	for (ListOfTrainsIterator iter = trains.begin (), jter = trains.end (); iter != jter; ++iter) {
		if (*iter == train) {
			trains.erase (iter);
			--numTrains;
			break;
		}
	}

	if (numTrains == 0) setOccupied (CTrack::FREE); // track is free
}

void CTrack::draw () {		
	
	
	CGameDevice::setColor (c);
	glEnableClientState (GL_VERTEX_ARRAY);
		glVertexPointer (2, GL_INT, 0, va);
		glDrawArrays (GL_LINE_STRIP, 0, nv);
	glDisableClientState (GL_VERTEX_ARRAY);		

}

bool CTrack::isLastTrain (CTrain *train) {
	/*
		returns true if last train else false
	*/

	if (occupied) {
		ListOfTrainsIterator iter = trains.end (); --iter;
		if (*iter == train) return true; else return false;
	}
	return false;

}

void CTrack::getWaypointParams (CTrain *train, int& start1, int& end1, int& inc1, int& start2, int& end2, int& inc2) {	

	/*

		waypoints show the path the train will take on the track

		waypoints drawn front of the train show where its going and
		waypoints drawn back of the train show where its been. the back
		waypoints are drawn only for the last train. the front waypoints
		are drawn upto the first coach of the previous train. if no 
		previous train is present, waypoints are drawn until termination 
		of track.

		start1 - start drawing front waypoints from here
		end1 - stop drawing front waypoints here
		inc1 - increment to move start1 to end1


		start2 - start drawing back waypoints from here
		end2 - stop drawing back waypoints here
		inc2 - increment to move start2 to end2

	*/
	
	//
	// find start1/end1/inc1
	//
	int n = get_n ();
	int lastTrackPoint = n - 1;
	CTrain *prevTrain = 0;
	for (ListOfTrainsIterator iter = trains.begin (), jter = trains.end (); iter != jter; ++iter) {
		CTrain *curTrain = *iter;
		if (curTrain == train) {
			start1 = train->getCoachPosition (CTrain::LAST_COACH); // front of train
			if (iter == trains.begin ()) { // this train is first train. 
				if (occupied == CTrack::OCCUPIED_OUT) // train is outward train
					end1 = lastTrackPoint; // waypoints can be drawn till end of track
				else // train is return train
					end1 = 0; // waypoints can be drawn till start of track
			} else { // other trains before this train on the track
				end1 = prevTrain->getCoachPosition (0); // waypoints can be drawn upto the back of previous train on track
			}			
			if (start1 <= end1) inc1 = 1; else inc1 = -1;
			break;
		}
		prevTrain = curTrain;
	}

	//
	// find start2/end2/inc2
	//
	if (isLastTrain (train)) { // last train
		end2 = train->getCoachPosition (0); 
		const int inc_c = 20;
		if (occupied == CTrack::OCCUPIED_OUT) { start2 = 0; inc2 = inc_c; } else {	start2 = n - 1; inc2 = -inc_c; }
	} else {
		start2 = end2 = 0;
		inc2 = 1;
	}

}

void getTracksContainingNode (ListOfTracks& nodeTracks, CNode* node, ListOfTracks& tracks) {

	/*
		get tracks that contain node
	*/

	for (ListOfTracksIterator i = tracks.begin (), j = tracks.end(); i != j; ++i) {
		CTrack* t = *i;
		if ((t->getNode(0) == node) || (t->getNode(1) == node)) nodeTracks.push_back (t);
	}	
	
}
