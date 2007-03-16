#include <CTrain.h>
#include <CTrack.h>
#include <CExplosion.h>
#include <CGameDevice.h>
#include <utils.h>

int64_t CExplosion::EVAL_INTERVAL = 0;

CExplosion::CExplosion (CTrain* train1, const color& tc) : gravity (-1), train1Color (tc) {
	
	CTrack* track1 = train1->getTrack ();
	const vector<int>& vx1 = track1->get_x ();
	const vector<int>& vy1 = track1->get_y ();

	for (int i = 0; i < CTrain::NUM_COACHES; i++) {
		
		int cp1 = train1->getCoachPosition (i);
		int x1 = vx1 [cp1], y1 = vy1 [cp1];

		train1PosX [i] = x1; train1PosY [i] = y1;

		int n = getRand7 ();
		train1SpeedX [i] = n % 10;
		train1SpeedY [i] = 11 + n % 10;

	}	
	
	extern int64_t TIME;
	startTime = TIME;

}

void CExplosion::eval () {
	
	extern int64_t TIME;
	int64_t elapsedTime = TIME - startTime;
	if (elapsedTime > EVAL_INTERVAL) {	
		startTime = TIME;
		int ymaxReached = 0;
		for (int i = 0; i < CTrain::NUM_COACHES; i++) {
				
				train1SpeedY [i] += gravity;
				train1PosX [i] += train1SpeedX [i];
				train1PosY [i] += train1SpeedY [i];

				if ((train1PosY [i] - CTrain::FINAL_COACH_SIZE) < 0) ++ymaxReached;

		}
		if (ymaxReached >= CTrain::NUM_COACHES) setFinished (true); 
	}	

}

void CExplosion::draw () {	
		
	static const double f = 4.5; // must sync with f in CTrain::draw ()
	
	color train1FillColor = color (train1Color.r/f, train1Color.g/f, train1Color.b/f);
	for (int i = 0, cs = CTrain::FIRST_COACH_SIZE; i < CTrain::NUM_COACHES; i++) {
		int left = train1PosX[i] - cs, bottom = train1PosY [i] - cs, right = train1PosX [i] + cs, top = train1PosY [i] + cs;
		CGameDevice::setColor (train1Color);
			glBegin (GL_LINE_LOOP);
				glVertex2i (left, bottom);
				glVertex2i (right, bottom);
				glVertex2i (right, top);
				glVertex2i (left, top);
			glEnd ();
		CGameDevice::setColor (train1FillColor);
		glRecti (left, bottom, right, top);				
		cs += CTrain::DELTA_COACH_SIZE;
	}

}
