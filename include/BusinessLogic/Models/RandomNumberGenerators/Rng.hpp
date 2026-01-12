// Author: Ben Kovalick
// Purpose: Header file for Rng base class.

#ifndef Rng_HPP
#define Rng_HPP

// Subsystem for generation random numbers
class Rng
{
public:
	virtual double gen() = 0;
};

#endif // !Rng_HPP
