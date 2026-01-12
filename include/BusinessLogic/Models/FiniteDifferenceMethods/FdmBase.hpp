// Author: Ben Kovalick
// Purpose: Header file for FDM base class.

#ifndef FdmBase_HPP
#define FdmBase_HPP

#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FDM.hpp"
#include <vector>
#include <memory>
#include <cmath>

class FdmBase : public FDM
{
protected:
	std::shared_ptr<Sde> sde_;
	double dtSqrt;

public:
	int NT; // Number of subdivisons
	std::vector<double> meshArray; // The mesh array; -> Make this generic
	double k; // Mesh size

	FdmBase() {}; // Default constructor
	FdmBase(const std::shared_ptr<Sde>& sde, int numSubdivisions) : sde_(sde), NT(numSubdivisions)  // Argument constructor
	{
		k = sde_->Expiry() / (double)NT;
		dtSqrt = std::sqrt(k);
		meshArray.resize(NT + 1);

		// Create the mesh array
		meshArray[0] = 0.0;
		for (unsigned int n = 1; n < meshArray.size(); n++)
		{
			meshArray[n] = meshArray[n - 1] + k;
		}
	}

	virtual ~FdmBase() {}; // Destructor

	std::shared_ptr<Sde> StochasticEquation()
	{
		return sde_;
	}

	// Advance solution from level t[n] to level t[n+1]
	virtual double advance(double xn, double tn, double dt, double WienerIncrement, double WienerIncrement2) = 0;
};

#endif