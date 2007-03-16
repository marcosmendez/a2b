#ifndef _COUTWARDTRAIN
#define _COUTWARDTRAIN

	#include <CTrain.h>	

	class CTrack;
	class COutwardTrain : public CTrain {
		void install (int trackPosition);
		public:
			COutwardTrain (CTrack* _track);
			~COutwardTrain ();			
	};

#endif
