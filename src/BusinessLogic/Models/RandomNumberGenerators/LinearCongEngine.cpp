// Author: Ben Kovalick
// Purpose: Source file for the Linear Congruential Engine.

#include "include/BusinessLogic/Models/RandomNumberGenerators/LinearCongEngine.hpp"

// Default Constructor
LinearCongEngine::LinearCongEngine()
{}

// Destructor
LinearCongEngine::~LinearCongEngine()
{}

// Return a random number
double LinearCongEngine::gen()
{
	static thread_local std::random_device rd; // Used to obtain a seed for the random number generator
	static thread_local std::minstd_rand generator(rd()); // Linear Congruential Engine engine with seed rd();
	static thread_local std::normal_distribution<double> dis(0.0, 1.0); // Distribution with mean 0 and stddev 1.

	return dis(generator); // Return the random number.
}