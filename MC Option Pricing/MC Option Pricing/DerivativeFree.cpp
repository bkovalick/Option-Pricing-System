// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the DerivativeFree FD scheme.

#include "DerivativeFree.hpp"

// Default Constructor
DerivativeFree::DerivativeFree() : FdmBase()
{}

// Argument Constructor
DerivativeFree::DerivativeFree(const std::shared_ptr<Sde>& stochasticEquation,
	int numSubdivisions)
	: FdmBase(stochasticEquation, numSubdivisions)
{}

// Destructor
DerivativeFree::~DerivativeFree()
{}

// Advance the solution from t[n] to t[n + 1]
double DerivativeFree::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	double dt1 = dt;
	sqrk = std::sqrt(dt1);
	Wincr = sqrk * normalVar;

	F1 = sde_->Drift(xn, tn);
	G1 = sde_->Diffusion(xn, tn);

	G2 = sde_->Diffusion(xn + G1 * sqrk, tn);
	addedVal = 0.5 * (G2 - G1) * (Wincr * Wincr - dt1) / sqrk;

	return xn + (F1 * dt1 + G1 * Wincr + addedVal);
}