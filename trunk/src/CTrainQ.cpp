#include <utils.h>
#include <CTrack.h>
#include <CTrain.h>
#include <CTrainQ.h>
#include <CGameDevice.h>

CTrainQ::CTrainQ (int _maxcd) : maxcd(_maxcd) {
	qtrain = 0; 
	empty = true;
}

void CTrainQ::set (CTrain* train, int64_t qtime) {

	qtrain = train;
	cdval = maxcd; 
	
	extern int64_t TIME;
	cdtimenow = TIME;
	cdtime = qtime / maxcd;
	empty = false;

}

CTrain* CTrainQ::eval () {		
		
	extern int64_t TIME;
	int64_t dt = TIME - cdtimenow;
	if (dt > cdtime) {
		if (--cdval == 0) { 			
			// countdown has run out. flush q 
			CTrain* train = qtrain;
			empty = true;			
			qtrain = 0;
			return train;
		} else 
			cdtimenow = TIME;
	}
	return 0;
}

void CTrainQ::drawCountdown () const {

	//
	// draw current countdown value on the qed train
	//

	if (qtrain) {

		CTrack* track = qtrain->getTrack ();
		const vector<int>& vx = track->get_x ();
		const vector<int>& vy = track->get_y ();
		
		int cp = qtrain->getCoachPosition (CTrain::LAST_COACH);
		int xp = vx [cp], yp = vy [cp];
		
		static char buf [11]; // upto 2^32 - 1
		sprintf (buf, "%d", cdval);
		
		const color& gray = CGameDevice::gray;
		glColor3f (gray.r, gray.g, gray.b);
		CGameDevice::drawString (buf, xp - 4, yp - 6);

	}

}
