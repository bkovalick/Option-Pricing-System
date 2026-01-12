// Author: Ben Kovalick
// Purpose: Header file for concrete implemention of the PredictorCorrector FDM scheme.

#ifndef PredictorCorrectorFdm_HPP
#define PredictorCorrectorFdm_HPP

#include "FdmBase.hpp"
#include "Sde.hpp"
#include <memory>

class PredictorCorrectorFdm : public FdmBase
{
private:
	double A, B, VMid;

public:
	PredictorCorrectorFdm(); // Default Constructor
	PredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions,
		double a, double b); // Argument Constructor
	virtual ~PredictorCorrectorFdm(); // Destructor

	// Concrete implementation of the advance function
	double advance(double xn, double tn, double dt, double normalVar, double normalVar2);
};

#endif // !PredictorCorrectorFdm_HPP