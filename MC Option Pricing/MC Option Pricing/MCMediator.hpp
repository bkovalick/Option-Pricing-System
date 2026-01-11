// Author: Ben Kovalick
// Purpose: Mediator header file, the main responsibility of this class is to coordinate 
// the other components in the system.

#ifndef MCMediator_HPP
#define MCMediator_HPP

#include <functional>
#include <algorithm>
#include <iostream>
#include <random>
#include <future>
#include <chrono>
#include <thread>

#include "FDM.hpp"
#include "FdmBase.hpp"
#include "Sde.hpp"
#include "Rng.hpp" 

#include <boost/signals2.hpp>

// Used for PPL Solution
#include <ppl.h> 

// Used for Open MP Solution
#include <omp.h>

template <typename T>
using PathEvent = boost::signals2::signal<void(const T& t)>; // Send a path array

using EndOfSimulation = boost::signals2::signal<void()>; // No more paths

class MCMediator
{
private:
	// Main components
	std::shared_ptr<Sde> sde_;
	std::shared_ptr<FdmBase> fdm_;
	std::shared_ptr<Rng> rng_;

	// Other MC-related data
	int NSim; // # of simulations
	std::vector<double> res; // Generated path per simulation

public:
	// Argument constructor
	MCMediator(std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> parts,
		int numberSimulations);

	// Destructor
	virtual ~MCMediator();

	// Responsible for path generation
	void start(); 
	void startPPL();
	void startOpenMP();

	void disconnect(); 

	// Event notification
	PathEvent<std::vector<double>> path; // Signal to the pricers
	EndOfSimulation finish; // All paths are complete
};

#endif // !MCMediator_HPP