// Author: Ben Kovalick
// Purpose: Header file for Abstract FDM class.

// FDM implementation (Used to find approximate solutions of SDEs) -> Several schemes to do this
// 1. Explicit Euler
// 2. Semi-Implicit Euler
// 3. Heun
// 4. Milstein
// 5. Derivative-free

#ifndef FDM_HPP
#define FDM_HPP

class FDM
{// Abstract Base class for one-step FDM methods for SDEs

public:
	// Advance solution from level t[n] to level t[n+1]
	virtual double advance(double xn, double tn, double dt, double WienerIncrement, double WienerIncrement2) = 0;
};

#endif // !FDM_HPP