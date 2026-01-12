// Author: Ben Kovalick
// Purpose: Source file for concrete implemention of the Exact FD scheme.

#include "BusinessLogic/Models/FiniteDifferenceMethods/ExactFdm.hpp"

// Default Constructor
ExactFdm::ExactFdm() : FdmBase()
{}

// Argument Constructor
ExactFdm::ExactFdm(const std::shared_ptr<Sde>& stochasticEquation, int numSubdivisions,
	double IC, double vol, double drift)
	: FdmBase(stochasticEquation, numSubdivisions)
{
	S0 = IC;
	sig = vol;
	mu = drift;
}

// Destructor
ExactFdm::~ExactFdm()
{}

// Advance the solution from t[n] to t[n + 1]
double ExactFdm::advance(double xn, double tn, double dt, double normalVar, double normalVar2)
{
	// Compute exact value at tn + dt
	double alpha = 0.5 * sig * sig;
	return S0 * std::exp((mu - alpha) * (tn + dt) + sig * std::sqrt(tn + dt) * normalVar);
}