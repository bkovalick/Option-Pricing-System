// Author: Ben Kovalick
// Purpose: Source file for Mersenne Twister

#include "MersenneTwister.hpp"

// Default Constructor
MersenneTwister::MersenneTwister()
{}

// Destructor
MersenneTwister::~MersenneTwister()
{}

// Return a random number
double MersenneTwister::gen()
{
	// Non-deterministic psuedo-random number generator
	static thread_local std::random_device rd;
	// Mersenne_twister engine with seed rd(); -> static so it's only created once
	static thread_local std::mt19937 generator(rd()); 
	// Normal distribution with mean 0 and stddev 1.
	static thread_local std::normal_distribution<double> dis(0.0, 1.0);
	
	return dis(generator); // Return the random number.
}