#ifndef _UTILS
#define _UTILS

#include <math.h>
#include <string>
#include "CBox.h"
using namespace std;

template <class T> inline bool inbox (T x, T y, T left, T bottom, T right, T top) {

	/*
		returns true if x,y is inside box. false otherwise.
	*/
	return (  ( (x >= left) && (x <= right) ) && ( (y >= bottom) && (y <= top) ) );

}

template <class T> inline bool inbox (T x, T y, const CBox& box) {

	/*
		returns true if x,y is inside box. false otherwise.
	*/

	return ( inbox<T> (x, y, box.left, box.bottom, box.right, box.top) );

}

inline bool isColliding (const CBox& box1, const CBox& box2) {
	/*
		returns true if box1 is colliding with box2, false otherwise
	*/

	if (box1.inside (box2) || box2.inside (box1)) 
		return true; 
	else 
		return false;

}

template<class T> inline void findVector (T& dx, T& dy, const T& x1, const T& y1, const T& x2, const T& y2) {
	
	// find vector joining x1,y1 to x2,y2
	
	dx = x2 - x1;
	dy = y2 - y1;
	
}

template <class T> inline double findMagnitude (const T& vx, const T& vy) {

	// find magnitude of vector vx,vy
	
	return sqrt (vx*vx + vy*vy);
	
}

template <class T> inline double findMagnitude (const T& x1, const T& y1, const T& x2, const T& y2) {

	// find magnitude of vector joining x1,y1 to x2,y2
	
	T dx, dy; findVector<T>(dx, dy, x1, y1, x2, y2);
	return findMagnitude<T>(dx, dy);
	
}

template<class T> inline void findUnitVector (T& ux, T& uy, const T& x1, const T& y1, const T& x2, const T& y2) {
	
	// unit vector joining x1,y1 & x2,y2
	T dx, dy; findVector<T> (dx, dy, x1, y1, x2, y2);
	double mag = findMagnitude<T> (dx, dy);
	ux = dx / mag;
	uy = dy / mag;	
	
}

template <class T> inline void findUnitVector (T& ux, T& uy, const T& vx, const T& vy) {
	// turn vx,vy into unit vector ux,uy
	double mag = findMagnitude<T>(vx, vy);
	ux = vx / mag;
	uy = vy / mag;	
}

template <class T> inline int inrange (T lo, T val, T hi) {
	// is lo <= val <= hi
	int result = 0;
	if (val < lo) result = -1; else if (val > hi) result = 1;
	return result;
}

inline double deg2rad (int degree) {

	// degree to radians
	
	static const double piby180 = 3.1415961 / 180;
	return (degree * piby180);
	
}

template<class T> inline void clip (const T& lo, T& val, const T& hi) {

	// ensures lo <= val <= hi
	if (val < lo) val = lo; else if (val > hi) val = hi;
	
}

template<class T> bool clip (T& x, T& y, T cx, T cy, const CBox& box) {
	
	/*
		clips x,y to box. cx,cy is assumed clipped.
	*/
	
	if (inbox (x, y, box)) return false;
	
	T dx, dy; 
	findVector<T>(dx, dy, cx, cy, x, y);
	
	if (dx == 0) {
		clip<T> (box.bottom, y, box.top);
		return true;
	} else if (dy == 0) {
		clip<T> (box.left, x, box.right);
		return true;
	} else {

		double m = dy * 1.0 / dx;
		
		int s = inrange<T> (box.left, x, box.right);
		int t = inrange<T> (box.bottom, y, box.top);
		
		switch (s) {
			case 0: // x in range			
				switch (t) {
					case 1: // y > box.top
						y = box.top;
						break;
					case -1: // y < box.bottom
						y = box.bottom;
						break;
				}			
				x = cx + (y - cy) / m;
				return true;
				
			case 1: // x > box.right			
				switch (t) {				
					case 0: // y in range				
						x = box.right;
						y = cy + m * (x - cx);
						return true;					
					case 1: // y > box.top
						x = box.right;
						y = cy + m * (x - cx);						
						if (inbox<T>(x, y, box) == false) {					
							y = box.top;										
							x = cx + (y - cy) / m;
						}
						return true;					
					case -1: // y < box.bottom				
						x = box.right;
						y = cy + m * (x - cx);						
						if (inbox<T>(x, y, box) == false) {
							y = box.bottom;																
							x = cx + (y - cy) / m;
						}					
						return true;										
				}
				break;
				
			case -1: // x < box.left			
				switch (t) {				
					case 0: // y in range					
						x = box.left;
						y = cy + m * (x - cx);					
						return true;									
					case 1: // y > box.top
						x = box.left;
						y = cy + m * (x - cx);						
						if (inbox<T> (x, y, box) == false) {					
							y = box.top;										
							x = cx + (y - cy) / m; 
						}
						return true;										
					case -1: // y < box.bottom					
						x = box.left;
						y = cy + m * (x - cx);						
						if (inbox<T> (x, y, box) == false) {
							y = box.bottom;
							x = cx + (y - cy) / m;
						}					
						return true;				
						
				}
				break;
				
		}	
	
		return false;

		
	}
	
	
}

template<class S, class T> inline void clear (S& container) {   
   
	// clears container of pointers. deletes the pointers.
   typedef typename S::iterator Si;
	for (Si i = container.begin(), j = container.end(); i != j; ++i) {
		T* t = *i;
		delete t;		
	}
	container.clear ();
}

template<class T> inline void erase (T& container, unsigned int q) {   
   
	// erases qth item in container
   
	typedef typename T::iterator Ti;
	
	Ti iter = container.begin();
		for (unsigned int p = 0; p < q; ++p, ++iter); 
	container.erase (iter);	
	
}

template<class S, class T> inline T& get (S& container, unsigned int q) {
   
	// gets qth item in container
   
   typedef typename S::iterator Si;

	Si iter = container.begin ();
		for (unsigned int p = 0; p < q; ++p, ++iter);
	return *iter;
	
}

template<class S, class T, class I> inline T& get (S& container, I& iter, unsigned int q) {

	// gets qth item of container and its iterator
	iter = container.begin ();
	for (unsigned int p = 0; p < q; p++, ++iter);
	return *iter;
	
}


/* 
   A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.
*/

const int N = 624;
const int M = 397;
const int MATRIX_A = 0x9908b0df; /* constant vector a */
const int UPPER_MASK = 0x80000000; /* most significant w-r bits */
const int LOWER_MASK = 0x7fffffff; /* least significant r bits */

static unsigned int mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

/* initializes mt[N] with a seed */
inline void setSeed (unsigned int s)
{
    mt[0]= s & 0xffffffff;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253 * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        //mt[mti] &= 0xffffffff;
        /* for >32 bit machines */
    }    
}

inline unsigned int getRand32 (void)
{
    unsigned int y;
    static unsigned int mag01[2]={0x0, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk, nm=N-M, mn=-nm, n1=N-1, m1=M-1;

        if (mti == N+1)   /* if init_genrand() has not been called, */
           setSeed (0); 

        for (kk=0;kk<nm;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        for (;kk<n1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+mn] ^ (y >> 1) ^ mag01[y & 0x1];
        }
        y = (mt[n1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[n1] = mt[m1] ^ (y >> 1) ^ mag01[y & 0x1];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680;
    y ^= (y << 15) & 0xefc60000;
    y ^= (y >> 18);

    return y;
    
}

inline unsigned int getRand16 (void) {
	return getRand32()&0xffff;
}

inline unsigned int getRand8 (void) {
	return getRand32()&0xff;
}

inline unsigned int getRand7 (void) {
	return getRand32()&0x7f;
}

inline unsigned int getRandBit (void) {
	return getRand32()&0x01;
}

#endif
