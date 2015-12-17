/*Jacob Massengill crypto.cpp
	-this program has two purposes:
		1) encrypts a .ppm image file with input from stdin
		2) decrypt encrypted .ppm image files
*/

#include "support.h"
#include "rnumgen.h"
#include <cstdlib>

using namespace std;

typedef enum { funct_encode, funct_decode } funct; //encrypt or decrypt?

const string USAGE_MSG = "USAGE: ./crypto -seed=N -encode|decode image.ppm\nterminating\n";
const string DCRPT_MSG = " is not encrypted, nothing to do\n";
const string ECRPT_MSG = " is already encrypted, nothing else done\n";
const string NOPPM_MSG = " is not a .ppm file";
const string ERIMG_MSG = "image will not work for this operation\n";

//set_pixel_target returns all the valid image indices
//for encoding and decoding
vector<pair<int,int> > set_pixel_target(PPM& img){
	vector<pair<int,int> > target_list;
	RGB color;
	int twelveBitNum;

	//build a histogram from even indices and target
	//list from the odd indices 
	vector<int> histogram(4096,0);
	for(int i=0; i < img.getCols(); i++){
		for(int j=0; j < img.getRows(); j++){
			if((j%2) && (i%2)){
				target_list.push_back(pair<int,int>(i,j));
			}
			else if(!(j%2) && !(i%2)){
				color = img[i][j];
				twelveBitNum = (((color.R >> 4) << 8) | ((color.G  >> 4 ) << 4) | (color.B >> 4));
				histogram[twelveBitNum]++;
			}
		}
	}
	
	//instantiate a rnumgen object that uses the histogram
	//for its distribution model
	rnumgen generator(histogram);
	int r1, r2, num;

	//randomly distribute the target_list vector
	for(int i=target_list.size()-1; i > 0; i--){
			r1 = generator.rand(); r2 = generator.rand();

			num = ((r1 << 12) | r2);	
			
			swap(target_list[i], target_list[num % (i+1)]);
	}

	return target_list;
}

//returns a ppm object describing the image argv
PPM get_image(const char *argv){
	PPM img;
	img.read(argv);

	//check to make sure the image didn't have any errors in processing
	//before continuing to reduce garbage being created
	if(!img.isGood()){
		cerr << argv << ERIMG_MSG; exit(1);
	}return img;
}

//encrypts an image with data from stdin, then writes that data to disk
void encode(const char* argv){

	PPM    img = get_image(argv);
	string input,temp;
	RGB    pixel;
	char   c;	
	
	//read from stdin in binary format
	while(cin.get(c)) input+=c;

	//add at least one ETX char until the 
	//size of the input is divisble by three	
	do{ input+=0x3; }while(input.size()%3 != 0);

	//get the target pixels
	vector<pair<int, int> > indices	= set_pixel_target(img);

	//check to make sure that the image is large enough
	//to support the message, if not print an error
	if(!(8*input.size() < 3*indices.size())){
		cerr << ERIMG_MSG; return;
	}

	//until input is exhausted, grab three characters at
	//a time to process
	for(int i=0; i < (int)input.size()/3; i++){
		temp = input.substr(3*i, 3*i+3);

		//for each character in temp, distribute the 8 bits used to describe it across
		//8 different RGB values, using red for the first char, green for the second, 
		//and blue for the third.
		for(int j=0; j < 8; j++){
			pixel = img[indices[8*i+j].first][indices[8*i+j].second];

			pixel.R &= 0xFE; pixel.G &= 0xFE; pixel.B &= 0xFE;

			pixel.R |= ((temp[0] >> j) & 0x1);
			pixel.G |= ((temp[1] >> j) & 0x1);
			pixel.B |= ((temp[2] >> j) & 0x1);

			img[indices[8*i+j].first][indices[8*i+j].second] = pixel;
		}
	}

	//write the now encrypted image to disk
	img.write(argv);
}

//decodes encrypted .ppm file, then prints the imbeded message to stdout
void decode(const char* argv){

	PPM img = get_image(argv);
	RGB color;
	vector<pair<int, int> > indices = set_pixel_target(img);
	vector<char> output;
	string temp;

	//run through the pairs of indices grabbed until an ETX char is found
	for(int i=0; i < (int)indices.size(); i++){

		temp.clear();
		temp.resize(3,0x0);

		//grab the first bit from the next 8 RGBs' red, green, and blue values
		//and build the next 3 characters from them respectively.
		for(int j=0; j < 8; j++){
			color = img[indices[8*i+j].first][indices[8*i+j].second];
			
			temp[0]  |=  ( (color.R & 0x1) << j );
			temp[1]  |=  ( (color.G & 0x1) << j );
			temp[2]  |=  ( (color.B & 0x1) << j );
		}
		
		//add the characters to the vector, if an ETX character is found, 
		//add each character in the string until that ETX character. 
		if(temp.find(0x3) != string::npos){
			for(int j=0; j < (int)temp.find(0x3); j++)
				output.push_back(temp[j]);
			break;
		}
		
		output.push_back(temp[0]);
		output.push_back(temp[1]);
		output.push_back(temp[2]);
	}

	//print the decrypted information to the user
	for(int i=0; i < (int)output.size(); i++) cout.put(output[i]);
}

//processes information in main to either 
//encode or decode
void process(funct f, const char* argv){
	if(f==funct_encode) encode(argv);
	else 		    decode(argv);
}

//parses command line arguments and driver code
int main(int argc, char* argv[]){
	string tmp, file = argv[argc-1];
	funct  f;

	//check for proper ammount of arguments
	if(argc != 4){ cerr << USAGE_MSG; return -1; }

	//extra measure of ppm quality control
	if(file.find(".ppm") == string::npos){
		cerr << file << NOPPM_MSG; return -2;
	}
		
	//process the arguments, take actions accordingly
	for(int i=1; i<argc-1; i++){
		tmp = argv[i];
		if(tmp == "-encode"){
			
			//don't encrypt if file is already encrypted
			if(file.find("_wmsg") != string::npos){
				cerr << file  << ECRPT_MSG; return -3;
			} f = funct_encode;	
		}
		else if(tmp.find("-seed=") != string::npos)
			srand(atoi(tmp.substr(6, tmp.size()).c_str()));

		else if(tmp == "-decode"){

			//don't decrypt if file isn't encrypted
			if(file.find("_wmsg") == string::npos){
				cerr << file << ECRPT_MSG; return -3;
			} f = funct_decode;
		}
		else{ cerr << USAGE_MSG; return -1; }
	}

	//now that the arguments have been handled,
	//process the needed data	
	process(f, file.c_str());	
	return 0;
}
