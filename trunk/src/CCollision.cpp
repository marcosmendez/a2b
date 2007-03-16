#include <CTrain.h>
#include <CTrack.h>
#include <CCollision.h>
#include <CGameDevice.h>
#include <utils.h>

int64_t CCollision::EVAL_INTERVAL = 0;

CCollision::CCollision (CTrain* train1, CTrain* train2) : gravity (-1) {
	
	CTrack* track1 = train1->getTrack ();
	const vector<int>& vx1 = track1->get_x ();
	const vector<int>& vy1 = track1->get_y ();

	CTrack* track2 = train2->getTrack ();
	const vector<int>& vx2 = track2->get_x ();
	const vector<int>& vy2 = track2->get_y ();

	for (int i = 0; i < CTrain::NUM_COACHES; i++) {
		
		int cp1 = train1->getCoachPosition (i);
		int x1 = vx1 [cp1], y1 = vy1 [cp1];

		int cp2 = train2->getCoachPosition (i);
		int x2 = vx2 [cp2], y2 = vy2 [cp2];

		train1PosX [i] = x1; train1PosY [i] = y1;
		train2PosX [i] = x2; train2PosY [i] = y2;

		int n = getRand7 ();
		train1SpeedX [i] = n % 10;
		train1SpeedY [i] = 11 + n % 10;
		train2SpeedX [i] = -n % 8;
		train2SpeedY [i] = 9 + n % 10;

	}
	
	if (train1->isSelected ()) {
		train1Color = CGameDevice::green; 
		train2Color = color (track2->getColor ());
	} else {
		train1Color = color (track1->getColor());
		if (train2->isSelected ()) train2Color = CGameDevice::green; else train2Color = color (track2->getColor ());
	}
	
	static const double f = 4.5; // must sync with f in CTrain::draw ()
	train1FillColor = color (train1Color.r/f, train1Color.g/f, train1Color.b/f);
	train2FillColor = color (train2Color.r/f, train2Color.g/f, train2Color.b/f);
	
	extern int64_t TIME;
	startTime = TIME;

}

void CCollision::eval () {
	
	extern int64_t TIME;
	int64_t elapsedTime = TIME - startTime;
	if (elapsedTime > EVAL_INTERVAL) {	
		startTime = TIME;
		int ymaxReached = 0;
		for (int i = 0; i < CTrain::NUM_COACHES; i++) {
				
				train1SpeedY [i] += gravity;
				train1PosX [i] += train1SpeedX [i];
				train1PosY [i] += train1SpeedY [i];

				train2SpeedY [i] += gravity;
				train2PosX [i] += train2SpeedX [i];
				train2PosY [i] += train2SpeedY [i];

				if (((train1PosY [i] - CTrain::FINAL_COACH_SIZE) < 0) && 
						((train2PosY [i] - CTrain::FINAL_COACH_SIZE) < 0)) ++ymaxReached;

		}
		if (ymaxReached >= CTrain::NUM_COACHES) setFinished (true); 
	}	

}

void CCollision::draw () {	
	
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

		for (int i = 0, cs = CTrain::FIRST_COACH_SIZE; i < CTrain::NUM_COACHES; i++) {
			int left = train2PosX[i] - cs, bottom = train2PosY [i] - cs, right = train2PosX [i] + cs, top = train2PosY [i] + cs;
			CGameDevice::setColor (train2Color);
				glBegin (GL_LINE_LOOP);
					glVertex2i (left, bottom);
					glVertex2i (right, bottom);
					glVertex2i (right, top);
					glVertex2i (left, top);
				glEnd ();
			CGameDevice::setColor (train2FillColor);
			glRecti (left, bottom, right, top);				
			cs += CTrain::DELTA_COACH_SIZE;
		}

}
