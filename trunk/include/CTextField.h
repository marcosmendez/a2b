#ifndef _CTEXTFIELD
#define _CTEXTFIELD

#include <CButton.h>
#include <string>

class CTextField : public CButton {

	std::string text;
	bool showCursor;
	int64_t t;
	unsigned int maxChars;
	static const int NUM_CHARS = 25;
	
	public:
		CTextField (const string& t, int left, int bottom, int n=NUM_CHARS);
		CTextField (int n=NUM_CHARS);
		
		void init ();
		void input ();
		void draw ();		
		
		void setLocation (int left, int bottom);
		void addChar (char c);
		void delLastChar ();
		const string& getText () { return text; }
		void setText (const string& t) { text = t; }
	
};
#endif
