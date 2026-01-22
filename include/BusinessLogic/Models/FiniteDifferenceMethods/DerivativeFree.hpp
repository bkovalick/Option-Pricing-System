// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the DerivativeFree FD scheme.

#ifndef DerivativeFree_HPP
#define DerivativeFree_HPP

#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include <memory>



class DerivativeFree : public FdmBase
{
private:
	double F1, G1, G2, addedVal, Wincr;
	double sqrk;
public:
	DerivativeFree(); // Default Constructor
	DerivativeFree(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivision); // Argument Constructor
	virtual ~DerivativeFree(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2) override;
};

#endif // !DerivativeFree_HPP