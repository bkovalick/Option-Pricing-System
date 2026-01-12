// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the ModifiedPredictorCorrector FD scheme.

#include "include/BusinessLogic/Models/FiniteDifferenceMethods/ModifiedPredictorCorrectorFdm.hpp"

// Default Constructor
ModifiedPredictorCorrectorFdm::ModifiedPredictorCorrectorFdm() : FdmBase()
{}

// Argument Constructor
ModifiedPredictorCorrectorFdm::ModifiedPredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation,
	int numSubdivisions, double a, double b)
	: FdmBase(stochasticEquation, numSubdivisions), A(a), B(b)
{}

// Destructor
ModifiedPredictorCorrectorFdm::~ModifiedPredictorCorrectorFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double ModifiedPredictorCorrectorFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	// Euler for predictor
	VMid = xn + sde_->Drift(xn, tn) * dt + sde_->Diffusion(xn, tn) * dtSqrt * normalVar;

	// Modified Trapezoidal rule
	double driftdoubleTerm = (A * sde_->DriftCorrected(VMid, tn + dt, B) + 
		((1.0 - A) * sde_->DriftCorrected(xn, tn, B))) * dt;

	double diffusiondoubleTerm = (B * sde_->Diffusion(VMid, tn + dt) + ((1.0 - B)
		* sde_->Diffusion(xn, tn))) * dtSqrt * normalVar;

	// midpoint adjusted
	return xn + driftdoubleTerm + diffusiondoubleTerm;
}