/*
	COutwardTrain.cpp
*/

#include <CTrack.h>
#include <COutwardTrain.h>

COutwardTrain::COutwardTrain (CTrack* _track) : CTrain (_track) {	

	_track->addTrain (this);
	_track->setOccupied (CTrack::OCCUPIED_OUT);
	setSpeed (SPEED);
	setAcceleration (ACCELERATION);
	install (0); // install first coach at first point of track
}

COutwardTrain::~COutwardTrain () {}

void COutwardTrain::install (int trackPosition) {
	
	/*
		installs train such that first coach is at track position,
		last coach towards end point of the track.
	*/

	CTrack* track = getTrack ();
	const vector<int>& vx = track->get_x ();
	const vector<int>& vy = track->get_y ();
	int n = track->get_n ();

	if (trackPosition < 0) {
		end (CTrain::RETURN_TO_BASE);
		return;
	}	else if (trackPosition >= n) {
		end (CTrain::ARRIVED);
		return;
	}

	int j = 0; setCoachPosition (j++, trackPosition); 
	int x1 = vx [trackPosition], y1 = vy [trackPosition];
	for (int i = trackPosition + 1; i < n; i++) {
		int x2 = vx [i], y2 = vy [i];
		int dx = x2 - x1;
		int dy = y2 - y1;
		int distance_2 = dx * dx + dy * dy;
		if (distance_2 >= COACH_SEPARATION_2) {
			setCoachPosition (j++, i);
			if (j == NUM_COACHES) { // coaches installed; calc bounding box that will enclose train.
				calcBBox ();
				return;
			}
			x1 = x2;
			y1 = y2;
		}
	}		

	end (CTrain::ARRIVED);
	
}
