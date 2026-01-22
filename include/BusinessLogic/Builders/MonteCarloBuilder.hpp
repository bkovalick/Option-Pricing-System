// Author: Ben Kovalick
// Purpose: Header file for implementing the Builder design pattern.

#ifndef MCBuilder_HPP
#define MCBuilder_HPP

// Option Data
#include "Core/Domain/OptionData.hpp"

// Finite Difference Scheme Classes
#include "BusinessLogic/Models/FiniteDifferenceMethods/FDM.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/EulerFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/ExactFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/MilsteinFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/DiscreteMilstein.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/PredictorCorrectorFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/Heun.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/DerivativeFree.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/ModifiedPredictorCorrectorFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/MidpointPredictorCorrectorFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FittedMidpointPredictorCorrectorFdm.hpp"

// Stochastic Differential Equation Classes
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/GBM.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/CEV.hpp"

// Random Number Generation Classes
#include "BusinessLogic/Models/RandomNumberGenerators/Rng.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/MersenneTwister.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/BoxMuller.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/LinearCongEngine.hpp"

#include <iostream>
#include <memory>
#include <tuple>

// Perhaps we make a builder that makes a list of all the necessary parts
class MonteCarloBuilder
{
private:
	double r;
	double v;
	double d;
	double IC;
	double T;
	double beta;

public:
	MonteCarloBuilder(); // Default Constructor
	MonteCarloBuilder(const OptionData& optData); // Argument Constructor
	virtual ~MonteCarloBuilder(); // Destructor

	// Parts initialised from the outside
	std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FDM>, std::shared_ptr<Rng>>
		Parts(const std::shared_ptr<Sde>& _sde, const std::shared_ptr<FDM>& _fdm,
		const std::shared_ptr<Rng>& _rng);

	// Parts initialised from the inside
	std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> Parts();

	// Returns a pointer to an Sde
	std::shared_ptr<Sde> getSde();

	// Returns a pointer to an Fdm
	std::shared_ptr<FdmBase> getFdm(const std::shared_ptr<Sde>& sde);

	// Returns a pointer to a Rng
	std::shared_ptr<Rng> getRng();

	// Static method that implements the Builder pattern.
	static std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> 
		ChooseBuilder(int n, const OptionData& optData);
};

// Default Builder class which constructs a generic MC implementation
class DefaultBuilder
{
private:
	double r;
	double v;
	double d;
	double IC;
	double T;
	double beta;

public:
	DefaultBuilder(const OptionData& optData)
	{
		int choice;
		std::cout << "Use default parameters? 1 = Yes, 0 = No "; std::cin >> choice;

		if (choice == 1)
		{
			r = 0.08;
			v = 0.30;
			d = 0.0;
			IC = 60.0;
			T = 0.25;
			beta = 1.0;
		}
		else
		{
			r = optData.r;
			v = optData.sig;
			d = optData.D;
			IC = optData.IC;
			T = optData.T;
			beta = optData.b;
		}
	}

	virtual ~DefaultBuilder() {} // Destructor

	// Parts initialised from the outside
	std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FDM>, std::shared_ptr<Rng>>
		Parts(const std::shared_ptr<Sde>& _sde, const std::shared_ptr<FDM>& _fdm,
			const std::shared_ptr<Rng>& _rng)
	{
		return std::make_tuple(_sde, _fdm, _rng);
	}

	// Parts initialised from the inside
	std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> Parts()
	{
		std::shared_ptr<Sde> sde = getSde(); // Return a concrete SDE
		std::shared_ptr<Rng> rng = getRng(); // Return a concrete RNG
		std::shared_ptr<FdmBase> fdm = getFdm(sde); // Return a concrete FDM

		return std::make_tuple(sde, fdm, rng);
	}

	// Returns a pointer to an Sde
	std::shared_ptr<Sde> getSde()
	{
		// Pointer to base class
		std::shared_ptr<Sde> sde;

		sde = std::make_shared<GBM>(r, v, d, IC, T);

		return sde;
	}

	// Returns a pointer to an Fdm
	std::shared_ptr<FdmBase> getFdm(const std::shared_ptr<Sde>& sde)
	{
		// Pointer to base class
		std::shared_ptr<FdmBase> fdm;

		// Have user select # of timesteps
		int NT = 500;
		std::cout << "Please select a number of timesteps: "; std::cin >> NT;

		fdm = std::make_shared<EulerFdm>(sde, NT);

		return fdm;
	}

	// Returns a pointer to a Rng
	std::shared_ptr<Rng> getRng()
	{
		// Pointer to base class
		std::shared_ptr<Rng> rng;
		rng = std::make_shared<MersenneTwister>();

		return rng;
	}

};

#endif // !MCBuilder_HPP