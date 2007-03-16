#ifndef _CRETURNTRAIN
#define _CRETURNTRAIN
	
	
	#include <CTrain.h>

	class CTrack;
	class CReturnTrain : public CTrain {
			void install (int trackPosition);
		public:
			CReturnTrain (CTrack* _track);
			~CReturnTrain ();			
	};

#endif
