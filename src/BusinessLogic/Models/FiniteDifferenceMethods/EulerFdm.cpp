// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the Euler FD scheme.

#include "include/BusinessLogic/Models/FiniteDifferenceMethods/EulerFdm.hpp"
#include <iostream>

// Default Constructor
EulerFdm::EulerFdm() : FdmBase()
{}

// Argument Constructor
EulerFdm::EulerFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions)
	: FdmBase(stochasticEquation, numSubdivisions)
{}

// Destructor
EulerFdm::~EulerFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double EulerFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	return xn + sde_->Drift(xn, tn) * dt + sde_->Diffusion(xn, tn) * dtSqrt * normalVar;
}