// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the Milstein FD scheme.

#include "include/BusinessLogic/Models/FiniteDifferenceMethods/MilsteinFdm.hpp"

// Default Constructor
MilsteinFdm::MilsteinFdm() : FdmBase()
{}

// Argument Constructor
MilsteinFdm::MilsteinFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions)
	: FdmBase(stochasticEquation, numSubdivisions)
{}

// Destructor
MilsteinFdm::~MilsteinFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double MilsteinFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	return xn + sde_->Drift(xn, tn) * dt + sde_->Diffusion(xn, tn) * std::sqrt(dt) * normalVar
		+ 0.5 * dt * sde_->Diffusion(xn, tn) * sde_->DiffusionDerivative(xn, tn) 
		* (normalVar * normalVar - 1.0);
}