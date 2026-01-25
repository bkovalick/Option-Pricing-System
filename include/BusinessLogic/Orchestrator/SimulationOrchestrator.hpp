#ifndef SimulationOrchestrator_HPP
#define SimulationOrchestrator_HPP

#include <Core/Configuration/SimulationConfig.hpp>
#include <BusinessLogic/Mediators/Mediator.hpp>
#include <BusinessLogic/Pricers/Pricer.hpp>

// Forward declarations for Monte Carlo components
class Sde;
class FdmBase;
class Rng;

struct SimulationInstance
{
	int instanceId_;
	std::unique_ptr<Mediator> mediator_;
	std::vector<std::shared_ptr<Pricer>> pricers;
	std::shared_ptr<Sde> sde_;
	std::shared_ptr<FdmBase> fdm_;
	std::shared_ptr<Rng> rng_;

	SimulationInstance(int instanceId, const std::shared_ptr<Sde>& sde,
		const std::shared_ptr<FdmBase>& fdm, const std::shared_ptr<Rng>& rng)
		: instanceId_(instanceId), sde_(sde), fdm_(fdm), rng_(rng) {
	}
};

class SimulationOrchestrator
{
private:
	SimulationConfig config_;
	std::unique_ptr<Mediator> mediator_;
	std::vector<std::shared_ptr<Pricer>> pricers_;
	std::vector<std::shared_ptr<Pricer>> pricers_;
	std::vector<OptionData> options_;

	void initializeOptions();
	void buildMediators();
	void setupPricers();
	void connectPricers();

public:
	explicit SimulationOrchestrator(const SimulationConfig& config);
	~SimulationOrchestrator() = default;

	// Delete copy operations (holds reference)
	SimulationOrchestrator(const SimulationOrchestrator&) = delete;
	SimulationOrchestrator& operator=(const SimulationOrchestrator&) = delete;

	// Allow move operations
	SimulationOrchestrator(SimulationOrchestrator&&) = default;
	SimulationOrchestrator& operator=(SimulationOrchestrator&&) = default;

	void run();
	void printResults();
};

#endif
