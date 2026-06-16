#ifndef MonteCarloEngine_HPP
#define MonteCarloEngine_HPP

#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <ppl.h> 
#include <omp.h>

#include "BusinessLogic/Models/FiniteDifferenceMethods/FDM.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/Rng.hpp" 
#include "BusinessLogic/Pricers/Pricer.hpp"
#include "BusinessLogic/Models/BlackScholes/BlackScholes.hpp"
#include "BusinessLogic/Engines/PricingEngine.hpp"
#include "Core/Configuration/SimulationConfig.hpp"

class MonteCarloEngine : public PricingEngine
{
private:
	std::shared_ptr<Sde> sde_;
	std::shared_ptr<FdmBase> fdm_;
	std::shared_ptr<Rng> rng_;
	std::shared_ptr<Pricer> pricer_;
	int numSimulations_;
	ExecutionMode executionMode_;
	std::vector<double> res;
	mutable concurrency::critical_section pricerMutex_;

	void generatePath();

public:
	MonteCarloEngine(
		std::shared_ptr<Sde> sde, 
		std::shared_ptr<FdmBase> fdm, 
		std::shared_ptr<Rng> rng,
		std::shared_ptr<Pricer> pricer,
		int numSimulations,
		ExecutionMode executionMode);
	virtual ~MonteCarloEngine() = default;

	double computePrice() override;
	double computePriceSequential();
	double computePriceOpenMP();
	double computePricePPL();
};

#endif
