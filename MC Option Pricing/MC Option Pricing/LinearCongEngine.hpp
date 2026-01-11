// Author: Ben Kovalick
// Purpose: Concrete implementation of Rng

#ifndef LinearCongEngine_HPP
#define LinearCongEngine_HPP

#include <random>
#include "Rng.hpp"

class LinearCongEngine : public Rng
{
public:
	LinearCongEngine(); // Default Constructor
	virtual ~LinearCongEngine(); // Destructor

	double gen(); // Generates random numbers
};

#endif // !LinearCongEngine_HPP
