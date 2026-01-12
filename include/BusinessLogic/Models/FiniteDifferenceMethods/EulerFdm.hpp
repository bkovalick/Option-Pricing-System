// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the Euler FD scheme.

#ifndef EulerFdm_HPP
#define EulerFdm_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

// Implement the euler finite difference scheme
class EulerFdm : public FdmBase
{
public:
	EulerFdm(); // Default Constructor
	EulerFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions); // Argument Constructor
	virtual ~EulerFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !EulerFdm_HPP