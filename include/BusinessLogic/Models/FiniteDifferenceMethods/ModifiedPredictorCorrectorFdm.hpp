// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the ModifiedPredictorCorrector FDM scheme.

#ifndef ModifiedPredictorCorrectorFdm_HPP
#define ModifiedPredictorCorrectorFdm_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

// PC using adjusted drift, Trapezoidal rule
class ModifiedPredictorCorrectorFdm : public FdmBase
{
private:
	double A, B, VMid;

public:
	ModifiedPredictorCorrectorFdm(); // Default Constructor
	ModifiedPredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions,
		double a, double b); // Argument Constructor
	virtual ~ModifiedPredictorCorrectorFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !ModifiedPredictorCorrectorFdm_HPP