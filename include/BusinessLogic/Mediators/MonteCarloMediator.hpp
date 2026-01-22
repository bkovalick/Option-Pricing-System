// Author: Ben Kovalick
// Purpose: Mediator header file, the main responsibility of this class is to coordinate 
// the other components in the system.

#ifndef MonteCarloMediator_HPP
#define MonteCarloMediator_HPP

#include <functional>
#include <algorithm>
#include <iostream>
#include <random>
#include <future>
#include <chrono>
#include <thread>

#include "BusinessLogic/Models/FiniteDifferenceMethods/FDM.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/Rng.hpp" 
#include "BusinessLogic/Mediators/Mediator.hpp"

#include <boost/signals2.hpp>

// Used for PPL Solution
#include <ppl.h> 

// Used for Open MP Solution
#include <omp.h>

template <typename T>
using PathEvent = boost::signals2::signal<void(const T& t)>; // Send a path array

using EndOfSimulation = boost::signals2::signal<void()>; // No more paths

class MonteCarloMediator : public Mediator
{
private:
	std::shared_ptr<Sde> sde_;
	std::shared_ptr<FdmBase> fdm_;
	std::shared_ptr<Rng> rng_;

	int NSim; 
	std::vector<double> res;

public:
	MonteCarloMediator();
	MonteCarloMediator(std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> parts,
		int numberSimulations);
	virtual ~MonteCarloMediator();

	void configure(const SimulationConfig& request) override;
	void start(); 
	void startPPL();
	void startOpenMP();
	void disconnect();
	PathEvent<std::vector<double>> path;
	EndOfSimulation finish;
};

#endif