//Jacob Massengill support.cpp

#include "support.h"
using namespace std;

PPM::PPM() {
  magicid = "P6";
  nrows = ncols = 0;
  maxvalue = 255;
  good = true;
}

PPM::~PPM() { }

//read a file describing a .ppm file into a matrix
void PPM::read(const string & fname){

  	// extract suffix: must be ppm
	string suf = fname.substr(fname.size()-4,fname.size()-1);
	if(suf != ".ppm"){
		cerr << "not a .ppm file\n\n";
		good = false; return;
	}
	
  	// open fname: check status
	ifstream file(fname.c_str());
	if(file.fail()){
		cerr << "could not open .ppm file\n\n";
		good = false; return;
	}

  	// read magicid: must be P6
	getline(file,magicid);
	if(magicid !="P6"){
		cerr << "incorrect .ppm header\n\n";
		good = false; return;
	}

	file >> nrows;
	file >> ncols;

  	// read maxvalue: must be 255
	file >> maxvalue;

	img.assign(ncols, nrows);
	
	while(file.get() != '\n') { }

	if(maxvalue == 255){
  		int npixels = ncols*nrows;
		file.read((char*)img.data(), sizeof(RGB)*npixels);
	}
	else{
		cerr << "maxvalue not correct\n\n";
		good = false;
	}	
	file.close();
}

//write a .ppm file from a matrix
void PPM::write(const string & fname) 
{
	
  	// new_fname: image.ppm -> image_addon.ppm
	string temp = fname.substr(0,fname.size()-4);
	temp+="_wmsg.ppm";

	ofstream file(temp.c_str());
	
  	// write header
	file << magicid << endl;
	file << nrows << " " << ncols << endl;
	file << maxvalue << endl;

 	int npixels = ncols*nrows;	

	file.write((char*)img.data(), sizeof(RGB)*npixels);
	file.close();
}
