//Jacob Massengill support.h declares a structure that can describe a PPM image
//also includes needed files for crypto

#ifndef SUPPORT_H
#define SUPPORT_H

#include "matrix.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

typedef unsigned char uchar;
struct RGB { 
	RGB(uchar r=0, uchar g=0, uchar b=0) { R = r; G = g; B = b; }
  	uchar R, G, B;
};

class PPM {
  public:
    	PPM();
    	~PPM();

	void read(const string &);
	void write(const string &);
	bool isGood()  const { return good;  }
	int  getRows() const { return nrows; }
	int  getCols() const { return ncols; }
	int  volume()  const { return nrows*ncols;}

	//allows user to grab pixels on a row-by-column basis
	RGB *operator[](int i) { return img[i]; }

  private:
	matrix<RGB> img;
	string magicid;
	bool good;
    	int nrows, ncols, maxvalue;
};
#endif //SUPPORT_H
