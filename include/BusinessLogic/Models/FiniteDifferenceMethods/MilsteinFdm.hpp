// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the Milstein FD scheme.

#ifndef MilsteinFdm_HPP
#define MilsteinFdm_HPP

#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include <memory>

class MilsteinFdm : public FdmBase
{
public:
	MilsteinFdm(); // Default Constructor
	MilsteinFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions); // Argument Constructor
	virtual ~MilsteinFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !MilsteinFdm_HPP