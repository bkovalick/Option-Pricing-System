// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the DiscreteMilstein FD scheme.

#include "DiscreteMilstein.hpp"

// Default Constructor
DiscreteMilstein::DiscreteMilstein() : FdmBase()
{}

// Argument Constructor
DiscreteMilstein::DiscreteMilstein(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions)
	: FdmBase(stochasticEquation, numSubdivisions)
{}

// Destructor
DiscreteMilstein::~DiscreteMilstein()
{}

// Advance the solution from t[n] to t[n + 1]
double DiscreteMilstein::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	double dt1 = dt; double sqrt = std::sqrt(dt1);
	double a = sde_->Drift(xn, tn);
	double b = sde_->Diffusion(xn, tn);
	double Yn = xn + a * dt1 + b * sqrt;

	return xn + a * dt1 + b * sqrt * normalVar + 0.5 * sqrt * (sde_->Diffusion(Yn, tn) - b)
		* (normalVar * normalVar - 1.0);
}