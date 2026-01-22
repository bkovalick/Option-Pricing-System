// Author: Ben Kovalick
// Purpose: Implementation of the mediator pattern.

#include "BusinessLogic/Mediators/MonteCarloMediator.hpp"
#include <BusinessLogic/Models/StochasticDifferentialEquations/GBM.hpp>
#include <Core/Configuration/SimulationConfig.hpp>

using namespace Concurrency;

MonteCarloMediator::MonteCarloMediator() : NSim(10000)
{
	// Default constructor
}

// Argument Constructor
MonteCarloMediator::MonteCarloMediator(std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> parts,
	int numberSimulations) : NSim(numberSimulations)
{
	sde_ = std::get<0>(parts);
	fdm_ = std::get<1>(parts);
	rng_ = std::get<2>(parts);
	res.resize(fdm_->NT + 1);
}

MonteCarloMediator::~MonteCarloMediator()
{
}

void MonteCarloMediator::configure(const SimulationConfig& request)
{
	//sde_ = std::make_shared<GBM>();
	//fdm_ = std::make_shared<FdmBase>();
	//rng_ = std::make_shared<>();
	//res.resize(fdm_->NT + 1);
	// Request will be a json or domain model object, perhaps a list of requests?
	// The idea is simple, it will create sde, fdm, rng based on the request
	
	//if (request == "start")
	//{
	//	start(); // Start the path generation
	//}
	//else
	//{
	//	throw std::invalid_argument("Unknown request: " + request);
	//}
}


// Path generation function
void MonteCarloMediator::start()
{
	double VOld, VNew;
	std::shared_future<double> VNew2;

	for (long i = 1; i <= NSim; ++i)
	{	// Calc a path at each iteration

		if ((i / 5000) * 5000 == i)
		{ // Give status after a given numbers of iterations
			std::cout << i << std::endl;
		}

		// Single-Threaded Solution
		VOld = sde_->InitialCondition(); res[0] = VOld;
		for (unsigned int n = 1; n < res.size(); n++)
		{ // Compute the solution at level n+l
			VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
			res[n] = VNew; VOld = VNew;
		}
		// Send path data to the pricers
		path(res);
	}
	
	finish(); // Signal to pricers to finish up.
	disconnect(); // Disconnect signals from their slots.
}

// MC simulation using OpenMP for parallelization 
void MonteCarloMediator::startOpenMP()
{
	double VOld, VNew;
	int tid = 0;

	#pragma omp parallel for
	for (long i = 1; i <= NSim; ++i)
	{	// Calc a path at each iteration

		tid = omp_get_thread_num();

		if ((i / 5000) * 5000 == i)
		{ // Give status after a given numbers of iterations
			std::cout << i << std::endl;
		}

		VOld = sde_->InitialCondition(); res[0] = VOld;
		for (unsigned int n = 1; n < res.size(); n++)
		{ // Compute the solution at level n+l
			VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
			res[n] = VNew; VOld = VNew;
		}
		// Send path data to the pricers
		path(res);
	}

	// In case you want to see the number of threads used.
	std::cout << "NumThreads: " << tid << std::endl;

	finish(); // Signal to pricers to finish up.
	disconnect(); // Disconnect signals from their slots.
}

// MC simulation using the PPL library for parallelization
void MonteCarloMediator::startPPL()
{
	double VOld, VNew;

	concurrency::parallel_for(0, NSim, [&](int i)
	{
		if ((i / 5000) * 5000 == i)
		{ // Give status after a given numbers of iterations
			std::cout << i << std::endl;
		}

		VOld = sde_->InitialCondition(); res[0] = VOld;
		std::size_t size = res.size();
		concurrency::parallel_for(std::size_t(0), size, [&](std::size_t n) {
			VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
			res[n] = VNew; VOld = VNew;
		});

		path(res);
	});

	finish();
	disconnect();
}

// Disconnect from all slots.
void MonteCarloMediator::disconnect()
{
	path.disconnect_all_slots();
	finish.disconnect_all_slots();
}