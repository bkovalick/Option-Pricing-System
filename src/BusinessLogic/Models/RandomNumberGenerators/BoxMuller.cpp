// Author: Ben Kovalick
// Purpose: Source file for BoxMuller

#include "BusinessLogic/Models/RandomNumberGenerators/BoxMuller.hpp"
#include <iostream>

// Default Constructor
BoxMuller::BoxMuller()
{}

// Destructor
BoxMuller::~BoxMuller()
{}

double BoxMuller::gen()
{
	do {
		U1 = (double)std::rand() / RAND_MAX;
		U2 = (double)std::rand() / RAND_MAX;
	} while (U1 <= 0.0);


	N1 = std::sqrt(-2.0 * std::log(U1));
	N2 = std::cos(2.0 * 3.1415159 * U2);

	return N1 * N2;
}