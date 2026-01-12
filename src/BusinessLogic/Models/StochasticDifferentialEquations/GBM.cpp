// Author: Ben Kovalick
// Purpose: Source file for Geometric Brownian Motion Class.

#include "BusinessLogic/Models/StochasticDifferentialEquations/GBM.hpp"

// Default Constructor
GBM::GBM()
{}

// Argument Constructor
GBM::GBM(double driftCoefficient, double diffusionCoefficient, double dividendYield,
	double initialCondition, double expiry) : mu(driftCoefficient), vol(diffusionCoefficient),
	d(dividendYield), ic(initialCondition), exp(expiry)
{}

// Destructor
GBM::~GBM()
{}

// Returns the drift of the Sde
double GBM::Drift(double x, double t) 
{ 
	return (mu - d) * x; 
}

// Returns the diffusion of the Sde
double GBM::Diffusion(double x, double t) 
{ 
	return vol * x; 
}

// Returns the correct diffusion of the Sde
double GBM::DriftCorrected(double x, double t, double B) 
{ 
	return Drift(x, t) - B * Diffusion(x, t) * DiffusionDerivative(x, t); 
}

// Returns the diffusion derivative of the Sde
double GBM::DiffusionDerivative(double x, double t) 
{
	return vol; 
}

// Returns the initial condition of the Sde
double GBM::InitialCondition() 
{ 
	return ic; 
}

// Returns the expiry of the Sde
double GBM::Expiry() 
{ 
	return exp; 
}