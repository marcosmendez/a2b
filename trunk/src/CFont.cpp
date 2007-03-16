#include <CFont.h>
#include <CGameDevice.h>
#include <GL/gl.h>

void CFont::load (ifstream& file) {         
   
   string ignore;
   file >> ignore >> name;
   
	int numchars;
   file >> ignore >> numchars;
   
   int xcells, ycells; 
   file >> ignore >> xcells >> ignore >> ycells;
   
   file >> ignore >> charWidth >> ignore >> charHeight;
   
	cellWidth = charWidth / xcells;
	cellHeight = charHeight / ycells;	
   
   for (int i = 0; i < numchars; i++) {
      
      char c;
      int numpoints;
      
      file >> c >> numpoints;
		
      
      vector<double>& chardesc = characters [c];      
      chardesc.push_back (numpoints);
      
      for (int j = 0; j < numpoints; j++) {
         
         double cx, cy; file >> cx >> cy;
         chardesc.push_back (cx);
         chardesc.push_back (cy);         
         
      }
      
   }
	
	vector<double> vs; vs.push_back (0);
	characters[' '] = vs;
	++numchars;
   
}
