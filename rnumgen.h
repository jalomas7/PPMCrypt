//Jacob Massengill rnumgen.h lab4
//this file declares a distribution based random number generator

#ifndef RNUMGEN_H
#define RNUMGEN_H

#include <vector>

class rnumgen {
	public:
		rnumgen(const std::vector<int> &);
		int rand() const;

	private:
		std::vector<float> F;
};
#endif //RNUMGEN_H
