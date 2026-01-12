// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the PredictorCorrectorFdm FD scheme.

#include "include/BusinessLogic/Models/FiniteDifferenceMethods/PredictorCorrectorFdm.hpp"

// Default Constructor
PredictorCorrectorFdm::PredictorCorrectorFdm() : FdmBase()
{}

// Argument Constructor
PredictorCorrectorFdm::PredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation,
	int numSubdivisions, double a, double b)
	: FdmBase(stochasticEquation, numSubdivisions), A(a), B(b)
{}

// Destructor
PredictorCorrectorFdm::~PredictorCorrectorFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double PredictorCorrectorFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	// Euler for predictor
	VMid = xn + sde_->Drift(xn, tn) * dt + sde_->Diffusion(xn, tn) * dtSqrt * normalVar;

	// Modified double rapezoidal rule
	double driftdoubleTerm = (A * sde_->Drift(VMid, tn + dt) + ((1.0 - A) 
		* sde_->Drift(xn, tn))) * dt;
	double diffusiondoubleTerm = (B * sde_->Diffusion(VMid, tn + dt) + ((1.0 - B) 
		* sde_->Diffusion(xn, tn))) * dtSqrt * normalVar;
	return xn + driftdoubleTerm + diffusiondoubleTerm;
}