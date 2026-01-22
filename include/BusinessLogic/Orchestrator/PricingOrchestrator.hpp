#ifndef PricingOrchestrator_HPP
#define PricingOrchestrator_HPP

#include <Core/Configuration/SimulationConfig.hpp>
#include <BusinessLogic/Mediators/Mediator.hpp>
#include <BusinessLogic/Pricers/Pricer.hpp>

class PricingOrchestrator
{
private:
	SimulationConfig config_;
	std::unique_ptr<Mediator> mediator_;
	std::vector<std::unique_ptr<Pricer>> pricers_;

	void initializeComponents();
	void setupMediator();
	void setupPricers();
	void connectPricers();

public:
	explicit PricingOrchestrator(const SimulationConfig& config);
	~PricingOrchestrator() = default;

	// Delete copy operations (holds reference)
	PricingOrchestrator(const PricingOrchestrator&) = delete;
	PricingOrchestrator& operator=(const PricingOrchestrator&) = delete;

	// Allow move operations
	PricingOrchestrator(PricingOrchestrator&&) = default;
	PricingOrchestrator& operator=(PricingOrchestrator&&) = default;

	void run();
	void printResults();
};

#endif
