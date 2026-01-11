// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the Heun FD scheme.

#ifndef Heun_HPP
#define Heun_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

class Heun : public FdmBase
{
public:
	Heun(); // Default Constructor
	Heun(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions); // Argument Constructor
	virtual ~Heun(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !Heun_HPP