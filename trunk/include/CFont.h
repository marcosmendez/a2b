#ifndef _CFONT
#define _CFONT

#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <GL/gl.h>
using namespace std;

class CFont {
   
   string name;       
   
   double cellWidth, cellHeight;    
   double charWidth, charHeight; 
   
   // map holds chars of this font. 
   // given a char, we can get the char's graphical description
   map < char, vector<double> >  characters; 
   
   public:
      
      CFont (const string& filename) {
         ifstream file (filename.c_str(), ios::in);
         load (file);
      }      

      void load (ifstream& file);

      double getCellWidth () const { return cellWidth; }
      double getCellHeight () const { return cellHeight; }
      double getCharWidth () const { return charWidth; }
      double getCharHeight () const { return charHeight; }        
      
      void drawChar (char c, double x, double y, double z=0) {
         
         const vector<double>& chardesc = characters[c];
         if (chardesc.size()) {
            glBegin (GL_LINES);
            for (int i = 0, j = (int) chardesc [0], k = 1; i < j; i++, k+=2) {
               glVertex3f (x + chardesc[k] * cellWidth, y + chardesc[k+1] * cellHeight, z);
            }
            glEnd ();                       
         }         
         
      }           
   
      bool isCharAvailable (char c) {         
         const vector<double>& chardesc = characters [c];         
         if (chardesc.size()) return true; else return false;                  
      }
};

#endif
