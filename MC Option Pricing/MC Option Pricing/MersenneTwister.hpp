// Author: Ben Kovalick
// Purpose: Concrete implementation of Rng

#ifndef MersenneTwister_HPP
#define MersenneTwister_HPP

#include <random>
#include "Rng.hpp"

class MersenneTwister : public Rng
{
public:
	MersenneTwister(); // Default Constructor
	virtual ~MersenneTwister(); // Destructor

	double gen(); // Generates random numbers
};

#endif // !MersenneTwister_HPP