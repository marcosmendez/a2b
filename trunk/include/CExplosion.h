#include <CEvent.h>
#include <CTrain.h>
#include <CGameDevice.h>
#include <typedefs.h>

class CExplosion : public CEvent {
	
	int train1PosX [CTrain::NUM_COACHES], train1PosY [CTrain::NUM_COACHES];
	int train1SpeedX [CTrain::NUM_COACHES], train1SpeedY [CTrain::NUM_COACHES]; 

	int gravity; // gravity to influence explosion
	int64_t startTime; // time when collision started
	
	color train1Color;

public:
	static int64_t EVAL_INTERVAL;
	CExplosion (CTrain* train1, const color& _train1Color = CGameDevice::gray);
	void draw ();
	void eval ();

};
