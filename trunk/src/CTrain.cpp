/*
	CTrain.cpp
*/
#include <CTrack.h>
#include <CTrain.h>
#include <CGame.h>
#include <CGameDevice.h>
#include <math.h>

// static vars
int CTrain::MAX_SPEED, CTrain::MIN_SPEED;
int CTrain::SPEED, CTrain::ACCELERATION;
int CTrain::FIRST_COACH_SIZE, CTrain::DELTA_COACH_SIZE, CTrain::FINAL_COACH_SIZE;
int CTrain::COACH_SEPARATION, CTrain::COACH_SEPARATION_2;
int64_t CTrain::TIME_TO_REACH_MAX_SPEED;

CTrain::CTrain (CTrack* _track) : track (_track) {	
	
	beginTime = endTime = 0;
	points = 0;
	selected = false;
	state = WAITING;
	
	int n = track->get_n ();
	va = new int [n];		

}

CTrain::~CTrain () {

	track->removeTrain (this);	
	delete[] va;

}

void CTrain::move () {	
	
	coachPositions [0] += speed;
	install (coachPositions[0]);

}

void CTrain::drawCoaches (const color& outlineColor) {
	
	/*
		draws all coaches of the train
	*/		
	
	const vector<int>& vx = track->get_x ();
	const vector<int>& vy = track->get_y ();
	
	const double f = 4.5; 
	double fr = outlineColor.r / f, fg = outlineColor.g / f, fb = outlineColor.b / f;
	int dc = FIRST_COACH_SIZE;
	for (int i = 0; i < NUM_COACHES; i++) {
		int cp = coachPositions [i];
		int x = vx[cp], y = vy[cp];
		int left = x - dc, bottom = y - dc, right = x + dc, top = y + dc;
		
		// coach outline
		CGameDevice::setColor (outlineColor);
		glBegin (GL_LINE_LOOP);
			glVertex2i (left, bottom);
			glVertex2i (right, bottom);
			glVertex2i (right, top);
			glVertex2i (left, top);
		glEnd ();
		
		// fill coach
		glColor3f (fr, fg, fb);
		glRecti (left, bottom, right, top);
		
		dc += DELTA_COACH_SIZE;
	}	
	glEnd ();
	
}

void CTrain::drawWaypoints (const color& c) {
	/*
		draws way points of the train on the track
	*/
	
	// get waypoint params
	int i1, j1, inc1, i2, j2, inc2; track->getWaypointParams (this, i1, j1, inc1, i2, j2, inc2);		
	
	//
	// draw front waypoints
	//			
	
	const vector<int>& vx = track->get_x ();
	const vector<int>& vy = track->get_y ();
	
	int i = 0, nv = 0;

	int x0 = vx [i1], y0 = vy [i1];
	int px = x0, py = y0;
	int d = 0;
	bool draw = false;
	while (i1 != j1) {
		i1 += inc1;
		int x1 = vx [i1], y1 = vy [i1];
		int cx = x1, cy = y1;
		int dx = cx - px, dy = cy - py;
		d += (int) (sqrt ((double) (dx * dx + dy * dy)));
		if (d > CTrack::WAYPOINT_SPACING) {
			draw = !draw;
			if (draw == false) {
				glEnableClientState (GL_VERTEX_ARRAY);
					CGameDevice::setColor (c);
					glVertexPointer (2, GL_INT, 0, va);
					glDrawArrays (GL_LINE_STRIP, 0, nv);
				glDisableClientState (GL_VERTEX_ARRAY);
			}
			i = 0; nv = 0;
			d = 0;
			x0 = x1; y0 = y1;
		} 
		va [i++] = x1; va [i++] = y1; ++nv;		
		px = cx; py = cy; 
	}	

	//
	// draw back waypoints only for selected train (avoids clutter)
	//
	
	i = nv = 0;
	if (isSelected()) {
		while (1) {
			int x0 = vx [i2], y0 = vy [i2];			
			va [i++] = x0; va [i++] = y0; ++nv;
			i2 += inc2;
			if (inc2 > 0) {
				if (i2 > j2) break; 
			} else if (i2 < j2) break;
		}
		
		CGameDevice::setColor (CGameDevice::weakGreen);		
		glEnableClientState (GL_VERTEX_ARRAY);
			glVertexPointer (2, GL_INT, 0, va);
			glDrawArrays (GL_LINE_STRIP, 0, nv);
		glDisableClientState (GL_VERTEX_ARRAY);
	}

}

void CTrain::calcBBox () {

	/*
		calculates the bounding box for this train
	*/
	

	const vector<int>& vx = track->get_x ();
	const vector<int>& vy = track->get_y ();

	int dc = FIRST_COACH_SIZE;
	int cp0 = coachPositions [0];
	int x0 = vx [cp0], y0 = vy [cp0];
	bbox.left = x0 - dc; bbox.bottom = y0 - dc;
	bbox.right = x0 + dc; bbox.top = y0 + dc;
	dc += FIRST_COACH_SIZE;

	for (int i = 1; i < NUM_COACHES; i++) {
		int cpi = coachPositions [i];
		int xi = vx [cpi], yi = vy [cpi];
		int left = xi - dc, bottom = yi - dc;
		int right = xi + dc, top = yi + dc;
		if (left < bbox.left) bbox.left = left;
		if (top > bbox.top) bbox.top = top;
		if (right > bbox.right) bbox.right = right;
		if (bottom < bbox.bottom) bbox.bottom = bottom;
		
		/* bbox width/height not calculated bcos not used in collision detection */
				
		dc += DELTA_COACH_SIZE;
	}

}

const CBox& CTrain::getBBox () {

	// return bounding box sorrounding train

	return bbox;

}

bool CTrain::maybeCollided (CTrain* otherTrain) {

	/*
		returns true if bounding box sorrounding this train collides
		with bounding box sorrounding other train, false otherwise.

		colliding bounding boxes dont mean collision has occured. however, if 
		bounding boxes dont collide, collision has not occured. hence, this 
		function is a quick elimination test when looking for colliding trains.

	*/

	const CBox& bbox0 = getBBox ();
	const CBox& bbox1 = otherTrain->getBBox ();
	if ( isColliding (bbox0, bbox1) ) return true; else return false;

}

bool CTrain::surelyCollided (CTrain* otherTrain) {

	/*

		returns true if a coach of the train collides with a coach
		of the other train, false otherwise.

	*/

	const vector<int>& vx0 = track->get_x ();
	const vector<int>& vy0 = track->get_y ();

	CTrack* track2 = otherTrain->getTrack ();
	const vector<int>& vx1 = track2->get_x ();
	const vector<int>& vy1 = track2->get_y ();
	
	//
	// run thru coaches (biggest to smallest) of this train and check if any collides
	// with the coaches (biggest to smallest) of other train
	//
	// we check biggest to smallest because the biggest coaches are more likely to collide.
	//

	CBox cbox0, cbox1;
	for (int i = LAST_COACH, cs0 = FINAL_COACH_SIZE; i > -1; i--, cs0 -= DELTA_COACH_SIZE) {
		int cp0 = coachPositions [i];
		int x0 = vx0 [cp0], y0 = vy0 [cp0];

		// ith coach bbox of this train
		cbox0.left = x0 - cs0;
		cbox0.bottom = y0 - cs0;
		cbox0.right = x0 + cs0;
		cbox0.top = y0 + cs0;

		for (int j = LAST_COACH, cs1 = FINAL_COACH_SIZE; j > -1; j--, cs1 -= DELTA_COACH_SIZE) {

			int cp1 = otherTrain->getCoachPosition (j);
			int x1 = vx1 [cp1], y1 = vy1 [cp1];

			// jth coach bbox of other train
			cbox1.left = x1 - cs1;
			cbox1.bottom = y1 - cs1;
			cbox1.right = x1 + cs1;
			cbox1.top = y1 + cs1;
			
			// do the coaches collide?
			if ( isColliding (cbox0, cbox1) ) return true;

		}		

	}
	return false;
	
}


void CTrain::mouseTest (int x, int y, int& d2) {

	/*
		find how far (d2) mouse x,y is from the nearest coach of the train
	*/	

	const vector<int>& vx = track->get_x ();
	const vector<int>& vy = track->get_y ();

	int cp0 = coachPositions [0];
	int x0 = vx[cp0], y0 = vy[cp0];
	int dx = x0 - x;
	int dy = y0 - y;
	d2 = dx * dx + dy * dy;

	for (int i = 1; i < NUM_COACHES; i++) {
		int j = coachPositions [i];		
		int xj = vx [j], yj = vy [j];
		dx = xj - x;
		dy = yj - y;
		int _d2 = dx * dx + dy * dy;
		if (_d2 < d2) d2 = _d2;
	}

}
