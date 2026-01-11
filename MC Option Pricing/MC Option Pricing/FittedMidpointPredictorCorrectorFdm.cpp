// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the FittedMidpointPredictorCorrectorFdm FD scheme.

#include "FittedMidpointPredictorCorrectorFdm.hpp"

// Default Constructor
FittedMidpointPredictorCorrectorFdm::FittedMidpointPredictorCorrectorFdm() : FdmBase()
{}

// Argument Constructor
FittedMidpointPredictorCorrectorFdm::FittedMidpointPredictorCorrectorFdm(const std::shared_ptr<Sde>& stochasticEquation,
	int numSubdivisions, double a, double b)
	: FdmBase(stochasticEquation, numSubdivisions), A(a), B(b)
{}

// Destructor
FittedMidpointPredictorCorrectorFdm::~FittedMidpointPredictorCorrectorFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double FittedMidpointPredictorCorrectorFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	// Euler for predictor
	double aFit = (std::exp(0.08 * dt) - 1.0) / dt;
	VMid = xn + aFit * xn * dt + sde_->Diffusion(xn, tn) * dtSqrt * normalVar;

	// Modified Trapezoidal rule
	double driftTerm = (sde_->DriftCorrected(A * VMid + (1.0 - A) * xn, tn + dt / 2, B)) * dt;

	double diffusionTerm = (sde_->Diffusion(B * VMid + (1.0 - B) * xn, tn + dt / 2)) * dtSqrt * normalVar;

	// midpoint adjusted
	return xn + driftTerm + diffusionTerm;
}