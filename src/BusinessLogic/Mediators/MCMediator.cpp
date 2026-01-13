// Author: Ben Kovalick
// Purpose: Implementation of the mediator pattern.

#include "BusinessLogic/Mediators/MCMediator.hpp"

using namespace Concurrency;

// Argument Constructor
MCMediator::MCMediator(std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> parts,
	int numberSimulations) : NSim(numberSimulations)
{
	sde_ = std::get<0>(parts);
	fdm_ = std::get<1>(parts);
	rng_ = std::get<2>(parts);

	// 1, 2 and 3 should be passed through the constructor
	// 1. Define slots for path information -> Pass in PathEvent Signal

	// 2. Signal end of simulation -> Pass in EndOfSimulation Signal

	// 3. Pass in number of simulations 

	// Ensure size of array is consistent with user defined timesteps.
	res.resize(fdm_->NT + 1);
}

// Destructor
MCMediator::~MCMediator()
{}

// Path generation function
void MCMediator::start()
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
void MCMediator::startOpenMP()
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
void MCMediator::startPPL()
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
void MCMediator::disconnect()
{
	path.disconnect_all_slots();
	finish.disconnect_all_slots();
}