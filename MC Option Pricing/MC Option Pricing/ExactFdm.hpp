// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the Exact FD scheme.

#ifndef ExactFdm_HPP
#define ExactFdm_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

class ExactFdm : public FdmBase
{
private:
	double S0; // Initial Condition
	double sig; // Vol
	double mu; // Drift

public:
	ExactFdm(); // Default Constructor
	ExactFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions,
		double S0, double vol, double drift); // Argument Constructor
	virtual ~ExactFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !ExactFdm_HPP