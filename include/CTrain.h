#ifndef _CTRAIN
#define _CTRAIN
	
#include <typedefs.h>
#include <utils.h>

class CTrack;
class CTrain {

	public:	
		static const int NUM_COACHES = 5;
		static const int LAST_COACH = NUM_COACHES - 1;
		static int SPEED, ACCELERATION;
		static int MIN_SPEED, MAX_SPEED;			
		static int FIRST_COACH_SIZE, DELTA_COACH_SIZE, FINAL_COACH_SIZE;
		static int COACH_SEPARATION, COACH_SEPARATION_2;
		static int64_t TIME_TO_REACH_MAX_SPEED;

		
		CTrain (CTrack* track);
		virtual ~CTrain ();
		
		void setSpeed (int _speed) {speed = _speed;}
		int getSpeed () const {return speed;}
		void setAcceleration (int _acceleration) {acceleration = _acceleration;}
		int getAcceleration () const {return acceleration;}

		void CTrain::accelerate () {

			speed += acceleration;	
			if (speed > MAX_SPEED) speed = MAX_SPEED; else if (speed < MIN_SPEED) speed = MIN_SPEED;

		}

		void CTrain::brake () {

			speed -= acceleration;	
			if (speed < MIN_SPEED) speed = MIN_SPEED; else if (speed > MAX_SPEED) speed = MAX_SPEED;

		}			

		void stop () { 

			speed = 0; 

		}

		void move ();

		void select () {selected = true;} 
		void deselect () {selected = false;}
		bool isSelected () const {return selected;}						

		enum { WAITING = 1, TRAVELING = 2, ARRIVED = 3, RETURN_TO_BASE = 4, COLLIDED = 5 }; // train states
		bool isWaiting () { return (state==WAITING); }
		bool isTraveling () { if (state == TRAVELING) return true; else return false; }
		bool hasArrived () { if (state == ARRIVED) return true; else return false; }		
		bool hasDerailed () { if ((state == RETURN_TO_BASE) || (state == COLLIDED)) return true; else return false; }
		bool hasFinishedJourney () { return (hasArrived () || hasDerailed ()); }

		void begin () { 
			extern int64_t TIME;
			beginTime = TIME; 
			state = TRAVELING; 
		}
		void end (int _state) { 
			extern int64_t TIME;
			endTime = TIME; 
			state = _state;
		}
		int64_t getJourneyTime () {
			if (hasFinishedJourney ()) return (endTime - beginTime); else return -1;
		}

		bool maybeCollided (CTrain* train); // maybe collided with given train? (quick/approximate test)
		bool surelyCollided (CTrain* train); // surely collided with given train? (detailed/accurate test)
		
		
		CTrack* getTrack () const {return track;}
		int getCoachPosition (int coachId) const {return coachPositions [coachId];}
		void setCoachPosition (int coachId, int position) {coachPositions [coachId] = position;}

		void mouseTest (int x, int y, int& d2);

		void drawCoaches (const color& c);	
		void drawWaypoints (const color& c);	
		
		void setPoints (int p) { points = p; }
		int getPoints () const { return points; }		

	private:


		int coachPositions [NUM_COACHES];	
		CBox bbox; // bounding box		
		
		int speed;
		int acceleration;		
		
		bool selected; // is train selected?
		
		int points; // points scored

		int state;
		int64_t beginTime, endTime; // begin/end times of journey
		

		CTrack* track; // track of the train
		int* va; // vertex array used for drawing waypoints			

		virtual void install (int trackPosition)=0;	// installs train on the track

	protected:

		void calcBBox (); 
		const CBox& getBBox ();


};	

#endif
