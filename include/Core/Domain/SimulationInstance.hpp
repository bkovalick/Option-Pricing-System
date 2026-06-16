#ifndef SimulationInstance_HPP
#define SimulationInstance_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <BusinessLogic/Engines/PricingEngine.hpp>
#include <BusinessLogic/Pricers/Pricer.hpp>

struct SimulationInstance
{
public:
	int instanceId_;
	std::unique_ptr<PricingEngine> engine_;
	std::unordered_map<std::string, std::shared_ptr<Pricer>> pricers_;
	
	std::string methodType_;       // "MonteCarlo", "BlackScholes", "BinomialTree"
	std::string algorithmDetail_;  // "GBM+Euler", "Analytical", "CRR"
	std::string executionMode_;    // "Stochastic", "Deterministic", "Lattice"
	std::string optionName_;
	std::string pricerType_;	   // "European", "Asian", "Barrier"

	SimulationInstance(int instanceId, 
					  const std::string& methodType,
					  const std::string& algorithmDetail, 
					  const std::string& executionMode,
					  const std::string& optionName,
					  const std::string& pricerType) 
		: instanceId_(instanceId), 
		  methodType_(methodType),
		  algorithmDetail_(algorithmDetail),
		  executionMode_(executionMode),
		  optionName_(optionName),
		  pricerType_(pricerType) {}
	
	SimulationInstance(SimulationInstance&&) = default;
	SimulationInstance& operator=(SimulationInstance&&) = default;

	SimulationInstance(const SimulationInstance&) = delete;
	SimulationInstance& operator=(const SimulationInstance&) = delete;
};

#endif