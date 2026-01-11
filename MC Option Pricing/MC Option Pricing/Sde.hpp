// Author: Ben Kovalick
// Purpose: Header file for Abstract SDE class.

// SDE's must deliver the following:
// 1. The drift function.
// 2. The diffusion function.
// 3. The interval [0,T] on which the SDE is defined.
// 4. The initial value X(0) of the stochastic process.

#ifndef Sde_HPP
#define Sde_HPP

class Sde
{	// Standard one-factor SDE dX = a(X,t)dt + b(X,t)dW, X(0) given dX = mu(X,t)dt + sig(X,t)dW

private:

public:
	virtual double Drift(double x, double t) = 0; // a (mu)
	virtual double Diffusion(double x, double t) = 0; // b (sig)

	// Extra functions 
	virtual double DriftCorrected(double x, double t, double B) = 0;
	virtual double DiffusionDerivative(double x, double t) = 0;
	virtual double InitialCondition() = 0;
	virtual double Expiry() = 0;
};

#endif // !Sde_HPP

