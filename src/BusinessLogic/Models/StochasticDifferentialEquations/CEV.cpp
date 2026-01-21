// Author: Ben Kovalick
// Purpose: Source file for Geometric Brownian Motion Class.

#include "BusinessLogic/Models/StochasticDifferentialEquations/CEV.hpp"

// Default Constructor
CEV::CEV()
	: mu(0.0), vol(0.0), d(0.0), ic(0.0), exp(0.0), b(0.0)
{}

// Argument Constructor
CEV::CEV(double driftCoefficient, double diffusionCoefficient, double dividendYield,
	double initialCondition, double expiry, double beta) : mu(driftCoefficient), 
	d(dividendYield), ic(initialCondition), exp(expiry), b(beta)
{
	vol = diffusionCoefficient * std::pow(ic, 1.0 - b);
}

// Destructor
CEV::~CEV()
{}

// Returns the drift of the Sde
double CEV::Drift(double x, double t)
{
	return (mu - d) * x;
}

// Returns the diffusion of the Sde
double CEV::Diffusion(double x, double t)
{
	return vol * std::pow(x, b);
}

// Returns the correct diffusion of the Sde
double CEV::DriftCorrected(double x, double t, double B)
{
	return Drift(x, t) - B * Diffusion(x, t) * DiffusionDerivative(x, t);
}

// Returns the diffusion derivative of the Sde
double CEV::DiffusionDerivative(double x, double t)
{
	if (b > 1.0)
	{
		return vol * b * std::pow(x, b - 1.0);
	}
	else
	{
		return vol * b / std::pow(x, b - 1.0);
	}
}

// Returns the initial condition of the Sde
double CEV::InitialCondition()
{
	return ic;
}

// Returns the expiry of the Sde
double CEV::Expiry()
{
	return exp;
}