// Author: Ben Kovalick
// Purpose: Header file for the CEV model.

#ifndef CEV_HPP
#define CEV_HPP

#include "Sde.hpp"
#include <iostream>

// CEV will implement the SDE
class CEV : public Sde
{
private:
	double mu; // Drift
	double vol; // Constant volatility
	double d; // Constant dividend yield
	double ic; // Initial Condition
	double exp; // Expiry
	double b; // Beta

public:
	CEV(); // Default Constructor
	CEV(double driftCoefficient, double diffusionCoefficient, double dividendYield,
		double initialCondition, double expiry, double beta); // Argument Constructor
	virtual ~CEV(); // Destructor

	// Implement virtual functions
	double Drift(double x, double t);
	double Diffusion(double x, double t);
	double DriftCorrected(double x, double t, double B);
	double DiffusionDerivative(double x, double t);
	double InitialCondition();
	double Expiry();
};

#endif // !1