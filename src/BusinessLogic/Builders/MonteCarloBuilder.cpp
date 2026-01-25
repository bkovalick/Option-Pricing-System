// Author: Ben Kovalick
// Purpose: Implementation of the builder pattern

#include "BusinessLogic/Builders/MonteCarloBuilder.hpp"

// Default Constructor
MonteCarloBuilder::MonteCarloBuilder()
{}

// Argument Constructor
MonteCarloBuilder::MonteCarloBuilder(const OptionData& optData)
{
	r = optData.r;
	v = optData.sig;
	d = optData.D;
	IC = optData.IC;
	T = optData.T;
	beta = optData.b;
}

//// Destructor
//MonteCarloBuilder::~MonteCarloBuilder()
//{}

// Parts initialised from the outside
std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FDM>, std::shared_ptr<Rng>> 
MonteCarloBuilder::Parts(const std::shared_ptr<Sde>& _sde, const std::shared_ptr<FDM>& _fdm,
	const std::shared_ptr<Rng>& _rng)
{
	return std::make_tuple(_sde, _fdm, _rng);
}

// Parts initialised from the inside
std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>>
MonteCarloBuilder::Parts()
{
	std::shared_ptr<Sde> sde = getSde(); // Return a concrete SDE
	std::shared_ptr<Rng> rng = getRng(); // Return a concrete RNG
	std::shared_ptr<FdmBase> fdm = getFdm(sde); // Return a concrete FDM

	return std::make_tuple(sde, fdm, rng);
}

// Returns a pointer to an Sde
std::shared_ptr<Sde> MonteCarloBuilder::getSde()
{
	int choice;
	std::cout << "1. GBM, 2. CEV "; std::cin >> choice;
	
	// Pointer to base class
	std::shared_ptr<Sde> sde;

	// Assign to a concrete implementation based on user input.
	switch (choice)
	{
	case 1:
		sde = std::make_shared<GBM>(r, v, d, IC, T);
		break;
	default:
		sde = std::make_shared<CEV>(r, v, d, IC, T, beta);
		break;
	}
	
	return sde;
}

// Returns a pointer to an Fdm
std::shared_ptr<FdmBase> MonteCarloBuilder::getFdm(const std::shared_ptr<Sde>& sde)
{
	int NT; // Number of timesteps
	std::cout << "Please select a number of timesteps: ";
	std::cin >> NT;

	// Have user select FDM scheme
	int choice;
	std::cout << std::endl;
	std::cout << "1. Explicit Euler, 2. Exact FDM, 3. Milstein, 4. Discrete Milstein, ";
	std::cout << "5. PredictorCorrector, 6. Heun, 7. DerivativeFree, 8. ModifiedPredictorCorrectorFdm ";
	std::cout << "9. MidpointPredictorCorrectorFdm , 10. FittedMidpointPredictorCorrectorFdm ";
	std::cin >> choice;

	// Pointer to base class
	std::shared_ptr<FdmBase> fdm;

	// Used in certain methods
	double a, b;

	// Assign to a concrete implementation based on user input.
	switch (choice)
	{
	case 1:
		fdm = std::make_shared<EulerFdm>(sde, NT);
		break;
	case 2:
		fdm = std::make_shared<ExactFdm>(sde, NT, IC, r, v);
		break;
	case 3:
		fdm = std::make_shared<MilsteinFdm>(sde, NT);
		break;
	case 4:
		fdm = std::make_shared<DiscreteMilstein>(sde, NT);
		break;
	case 5:
		a = 0.5;
		b = 0.5;
		fdm = std::make_shared<PredictorCorrectorFdm>(sde, NT, a, b);
		break;
	case 6:
		fdm = std::make_shared<Heun>(sde, NT);
		break;
	case 7:
		fdm = std::make_shared<DerivativeFree>(sde, NT);
		break;
	case 8:
		a = 0.5;
		b = 0.5;
		fdm = std::make_shared<ModifiedPredictorCorrectorFdm>(sde, NT, a, b);
		break;
	case 9:
		a = 0.5;
		b = 0.5;
		fdm = std::make_shared<MidpointPredictorCorrectorFdm>(sde, NT, a, b);
		break;
	case 10:
		a = 0.5;
		b = 0.5;
		fdm = std::make_shared<FittedMidpointPredictorCorrectorFdm>(sde, NT, a, b);
		break;
	default:
		fdm = std::make_shared<EulerFdm>(sde, NT);
		break;
	}

	return fdm;
}

// Returns a pointer to a Rng
std::shared_ptr<Rng> MonteCarloBuilder::getRng()
{
	// Have user select random number generator
	int choice;
	std::cout << "1. MersenneTwister, 2. BoxMuller, 3. LinearCongruentialEngine "; 
	std::cin >> choice;

	// Pointer to base class
	std::shared_ptr<Rng> rng;
	switch (choice)
	{
	case 1:
		rng = std::make_shared<MersenneTwister>();
		break;
	case 2:
		rng = std::make_shared<BoxMuller>();
		break;
	case 3:
		rng = std::make_shared<LinearCongEngine>();
		break;
	default:
		rng = std::make_shared<MersenneTwister>();
		break;
	}

	return rng;
}

// Factory method to choose your builder.
std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> 
MonteCarloBuilder::ChooseBuilder(int n, const OptionData& optData)
{ 
	std::shared_ptr<MonteCarloBuilder> builder;
	std::shared_ptr<DefaultBuilder> builder2;

	// Let user choose type of builder.
	std::cout << "1. MCBuilder, 2. Default Builder "; std::cin >> n;
	
	// Return user selected type
	if (n == 1) 
	{
		std::cout << "Chosen 1. MCBuilder " << std::endl;
		builder = std::make_shared<MonteCarloBuilder>(optData);
		return builder->Parts();
	}
	else
	{
		std::cout << "Chosen 2. Default Builder " << std::endl;
		builder2 = std::make_shared<DefaultBuilder>(optData);
		return builder2->Parts();
	}
}