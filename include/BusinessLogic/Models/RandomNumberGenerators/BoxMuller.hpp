// Author: Ben Kovalick
// Purpose: Implement the BoxMuller method for generating random numbers.

#ifndef BoxMuller_HPP
#define BoxMuller_HPP

#include "Rng.hpp"
#include <random>

class BoxMuller : public Rng
{
private:
	double U1, U2; // Uniform numbers
	double N1, N2; // Normal numbers -> product of BoxMuller

public:
	BoxMuller(); // Default Constructor
	virtual ~BoxMuller(); // Destructor

	double gen(); // Generate a random number.
};

#endif // !BoxMuller_HPP
