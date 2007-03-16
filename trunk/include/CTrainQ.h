#ifndef _CTRAINQ
#define _CTRAINQ

class CTrain;

class CTrainQ {

	CTrain* qtrain;
	bool empty;
	
	int cdval;
	int64_t cdtimenow;
	
	int64_t cdtime; // time per count down
	int maxcd; // max countdown

public:
	
	CTrainQ (int _maxcd=3);
	CTrain* eval ();
	bool isEmpty () { return empty; }
	
	void set (CTrain* train, int64_t qtime);
	CTrain* get () const { return qtrain; };

	void drawCountdown () const;	

};

#endif
