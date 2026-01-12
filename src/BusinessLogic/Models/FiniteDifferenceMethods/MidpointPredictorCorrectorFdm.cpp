// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the MidpointPredictorCorrectorFdm FD scheme.

#include "BusinessLogic/Models/FiniteDifferenceMethods/MidpointPredictorCorrectorFdm.hpp"

// Default Constructor
MidpointPredictorCorrectorFdm::MidpointPredictorCorrectorFdm() : FdmBase()
{}

// Argument Constructor
MidpointPredictorCorrectorFdm::MidpointPredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation,
	int numSubdivisions, double a, double b)
	: FdmBase(stochasticEquation, numSubdivisions), A(a), B(b)
{}

// Destructor
MidpointPredictorCorrectorFdm::~MidpointPredictorCorrectorFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double MidpointPredictorCorrectorFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	// Euler for predictor
	VMid = xn + sde_->Drift(xn, tn) * dt + sde_->Diffusion(xn, tn) * dtSqrt * normalVar;

	// Modified Trapezoidal rule
	double driftTerm = (sde_->DriftCorrected(A * VMid + (1.0 - A) * xn, tn + dt / 2, B)) * dt;
 
	double diffusionTerm = (sde_->Diffusion(B * VMid + (1.0 - B) * xn, tn + dt / 2)) * dtSqrt * normalVar;

	// midpoint adjusted
	return xn + driftTerm + diffusionTerm;
}