// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the DiscreteMilstein FD scheme.

#ifndef DiscreteMilstein_HPP
#define DiscreteMilstein_HPP

#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include <memory>

class DiscreteMilstein : public FdmBase
{
public:
	DiscreteMilstein(); // Default Constructor
	DiscreteMilstein(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions); // Argument Constructor
	virtual ~DiscreteMilstein(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !DiscreteMilstein_HPP