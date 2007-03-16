#ifndef _CGAMEDEVICE
#define _CGAMEDEVICE

#include <CFont.h>
#include <clock64.h>
#include <GL/freeglut.h>
#include <CBox.h>
#include <typedefs.h>

class CGameDevice {   
   
   static double MOUSE_XFACTOR, MOUSE_YFACTOR;	
   
   public:      
   
   static int64_t CLOCKS_PER_SECOND;
	static double INVERSE_CLOCKS_PER_SECOND;	
   
   static int MOUSE_X, MOUSE_Y;
   static int MOUSE_LMB, MOUSE_RMB;
   
   static int WINDOW_WIDTH, WINDOW_HEIGHT;
   static int SCREEN_WIDTH, SCREEN_HEIGHT;
	static int XMAX, YMAX;
	
	static CBox ARENA;	

	static void findClockSpeed () {		
		
		int t32, dt;
		int q = 2;
		int timeportion = CLOCKS_PER_SEC / q;   
		int64_t T64 = 0, t64 = 0;
		t32 = clock ();
		t64 = clock64();
		while (true) {         
			dt = (clock() - t32); 
			if (dt >= timeportion) break;
		}
		T64 = clock64() - t64;   
		CLOCKS_PER_SECOND = q * T64;
		INVERSE_CLOCKS_PER_SECOND = 1.0 / CLOCKS_PER_SECOND;
		cout << "processor clock speed = " << CLOCKS_PER_SECOND / 1000000 << " Mhz" << endl;   
		
	}   

	static void mousePositionChanged (int x, int y) {
	
		MOUSE_X = (int) (x * MOUSE_XFACTOR); 
		MOUSE_Y = (int) (MOUSE_YFACTOR * ( SCREEN_HEIGHT - y));   
	
	}

	static void mouseClicked (int button, int state, int x, int y) {
	
		mousePositionChanged (x, y);
		MOUSE_LMB = MOUSE_RMB = 0;
		if (button == GLUT_LEFT_BUTTON) {
			if (state == GLUT_DOWN) MOUSE_LMB = 1;
		} else if (button == GLUT_RIGHT_BUTTON) if (state == GLUT_DOWN) MOUSE_RMB = 1;   
		
	}

	static void windowResized (int w, int h) {
		
		SCREEN_WIDTH = w;
		SCREEN_HEIGHT = h; 		  
		
		cout << "screen width = " << SCREEN_WIDTH << " screen height = " << SCREEN_HEIGHT << endl;
		
		MOUSE_XFACTOR = WINDOW_WIDTH / (double) SCREEN_WIDTH;
		MOUSE_YFACTOR = WINDOW_HEIGHT / (double) SCREEN_HEIGHT;		
		
		glClearColor (0, 0, 0, 0);		
		glShadeModel (GL_FLAT);
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity ();
		glOrtho (0, WINDOW_WIDTH, 0, WINDOW_HEIGHT, -1, 1);
		glViewport (0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);			
		glMatrixMode (GL_MODELVIEW);
		glLoadIdentity ();
		glLineWidth (2);		
		
	}

	static unsigned int KEYS [256];

	static void charPressed (unsigned char c, int x, int y) { KEYS[c] = 1; KEY = c; }
	static void charReleased (unsigned char c, int x, int y) { KEYS[c] = KEY = REPEAT_KEY = 0;}
	static void specialKeyPressed (int k, int x, int y) {	KEYS[k] = 1; KEY = k; }
	static void specialKeyReleased (int k, int x, int y) { KEYS[k] = KEY = REPEAT_KEY = 0; }   

	static unsigned int KEY;		
	
	/*
		fudge to handle key repeat correctly. see CTextField
	*/
	static unsigned int REPEAT_KEY;
	static unsigned int repeatKey () {		
		if (REPEAT_KEY != KEY) {
			REPEAT_KEY = KEY;
			return KEY; 
		} else return 0;				
	}

   
   static CFont FONT;
	
	static void CGameDevice::drawString (const string& s, double x, double y, double z=0) {     
		
		for (int p = 0, q = s.length(); p < q; p++) {
			char c = s[p];
			FONT.drawChar (c, x, y, z);
			x += FONT.getCharWidth ();
		}
		
	}
	
	static bool CGameDevice::isCharAvailable (char c) {
		return FONT.isCharAvailable (c);
	}
	
   static int stringWidth (const string& s) { return (int)(FONT.getCharWidth () * s.length() + 0.5); }
	static int stringWidth (int numchars) { return (int)(FONT.getCharWidth () * numchars + 0.5); }
	static int stringHeight () { return (int)(FONT.getCharHeight()); }

	inline static void setColor (const color& c) {
		glColor3f (c.r, c.g, c.b);
	}	
   
   static const unsigned int ENTER = 13;
   static const unsigned int BACKSPACE = 8;
   static const unsigned int DELETE = 127;
	static const unsigned int ESC = 27;
	static const unsigned int LEFT = 100;
	static const unsigned int UP = 101;
	static const unsigned int RIGHT = 102;
	static const unsigned int DOWN = 103;
	static const unsigned int W = 119;
	static const unsigned int S = 115;
	static const unsigned int A = 97;
	static const unsigned int D = 100;	
	
	static const color weakGray;
	static const color gray;
	static const color weakGreen;
	static const color green;
	static const color red;
	static const color yellow;
	static const color gold;
	
   
};

inline bool isKeyPressed (unsigned int key, bool& press) {
	int current = CGameDevice::KEYS[key];
	if (current) {
		press = true;
		return false;
	} else if (press) return true; else return false;	
}

inline int isKeyDown (unsigned int key) {
	return (CGameDevice::KEYS[key]);
}

#endif
