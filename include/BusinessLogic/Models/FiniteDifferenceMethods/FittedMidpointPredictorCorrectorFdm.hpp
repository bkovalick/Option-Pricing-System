// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the FittedMidpointPredictorCorrectorFdm FDM scheme.

#ifndef FittedMidpointPredictorCorrectorFdm_HPP
#define FittedMidpointPredictorCorrectorFdm_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

class FittedMidpointPredictorCorrectorFdm : public FdmBase
{
private:
	double A, B, VMid;

public:
	FittedMidpointPredictorCorrectorFdm(); // Default Constructor
	FittedMidpointPredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions,
		double a, double b); // Argument Constructor
	virtual ~FittedMidpointPredictorCorrectorFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !FittedMidpointPredictorCorrectorFdm_HPP