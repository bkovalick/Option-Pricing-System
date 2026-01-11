// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the MidpointPredictorCorrectorFdm FDM scheme.

#ifndef MidpointPredictorCorrectorFdm_HPP
#define MidpointPredictorCorrectorFdm_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

// PC using adjusted drift, Trapezoidal rule
class MidpointPredictorCorrectorFdm : public FdmBase
{
private:
	double A, B, VMid;

public:
	MidpointPredictorCorrectorFdm(); // Default Constructor
	MidpointPredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions,
		double a, double b); // Argument Constructor
	virtual ~MidpointPredictorCorrectorFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !MidpointPredictorCorrectorFdm_HPP