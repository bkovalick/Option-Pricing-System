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
#include <boost/signals2.hpp>
#include <ppl.h> 
#include <omp.h>

#include "BusinessLogic/Models/FiniteDifferenceMethods/FDM.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/Rng.hpp" 
#include "BusinessLogic/Mediators/Mediator.hpp"
#include <BusinessLogic/Pricers/Pricer.hpp>

template <typename T>
using PathEvent = boost::signals2::signal<void(const T& t)>;

using EndOfSimulation = boost::signals2::signal<void()>;

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

	void connectPricers(const std::shared_ptr<Pricer>& pricer, int slot) override;
	void start() override;
	void startPPL() override;
	void startOpenMP() override;
	void disconnect();
	PathEvent<std::vector<double>> path;
	EndOfSimulation finish;
};

#endif