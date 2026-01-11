// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the Heun FD scheme.

#include "Heun.hpp"

// Default Constructor
Heun::Heun() : FdmBase()
{}

// Argument Constructor
Heun::Heun(const std::shared_ptr<Sde>& stochasticEquation,
	int numSubdivisions)
	: FdmBase(stochasticEquation, numSubdivisions)
{}

// Destructor
Heun::~Heun()
{}

// Advance the solution from t[n] to t[n + 1]
double Heun::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	// A and B represent the left slopes
	double a = sde_->Drift(xn, tn);
	double b = sde_->Diffusion(xn, tn);

	// suppValue represents the right slope
	double suppValue = xn + a * dt + b * std::sqrt(dt) * normalVar;

	// Final solution is the ideal slope (0.5 * (Slope::left + Slope::right))
	return xn + 0.5 * (sde_->Drift(suppValue, tn) + a) * dt + 0.5 * 
		(sde_->Diffusion(suppValue, tn) + b) * std::sqrt(dt) * normalVar;
}