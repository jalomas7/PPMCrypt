//Jacob Massengill rnumgen.cpp lab4
//this file defines the functions of the distribution based
//random number generator declared in rnumgen.h

#include "rnumgen.h"
#include <algorithm>
#include <functional>
#include <numeric>
using namespace std;

rnumgen::rnumgen(const vector<int> &v){
	F.resize(v.size());
	partial_sum(v.begin(), v.end(), F.begin());
	transform(F.begin(), F.end(), F.begin(),
	          bind2nd(divides<float>(),*(F.end()-1)));
}

int rnumgen::rand() const {
	const float randnorm = RAND_MAX+1.0f;
	const float p = (float)std::rand()/randnorm;
	return upper_bound(F.begin(), F.end(), p) - F.begin();
}
