// Author: Ben Kovalick
// Purpose: Header file for Geometric Brownian Motion Class, derived from Sde.

#ifndef GBM_HPP
#define GBM_HPP

#include "Sde.hpp"
#include <iostream>

// GBM will implement the SDE
// Wt = W(t)(one-dimensional Brownian motion)
class GBM : public Sde
{
private:
	double mu; // Drift
	double vol; // Constant volatility
	double d; // Constant dividend yield
	double ic; // Initial Condition
	double exp; // Expiry

public:
	GBM(); // Default Constructor
	GBM(double driftCoefficient, double diffusionCoefficient, double dividendYield,
		double initialCondition, double expiry); // Argument Constructor
	virtual ~GBM(); // Destructor

	// Implement virtual functions
	double Drift(double x, double t);
	double Diffusion(double x, double t);
	double DriftCorrected(double x, double t, double B);
	double DiffusionDerivative(double x, double t);
	double InitialCondition();
	double Expiry();
};

#endif // !1