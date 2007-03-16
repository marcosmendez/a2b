#include <CEvent.h>
#include <CTrain.h>
#include <typedefs.h>

class CCollision : public CEvent {
	
	int train1PosX [CTrain::NUM_COACHES], train1PosY [CTrain::NUM_COACHES];
	int train2PosX [CTrain::NUM_COACHES], train2PosY [CTrain::NUM_COACHES];
	int train1SpeedX [CTrain::NUM_COACHES], train1SpeedY [CTrain::NUM_COACHES]; 
	int train2SpeedX [CTrain::NUM_COACHES], train2SpeedY [CTrain::NUM_COACHES]; 

	int gravity; // gravity to influence explosion
	int64_t startTime; // time when collision started
	
	color train1Color, train2Color;
	color train1FillColor, train2FillColor;

public:
	static int64_t EVAL_INTERVAL;
	CCollision (CTrain* train1, CTrain* train2);
	void draw ();
	void eval ();

};
