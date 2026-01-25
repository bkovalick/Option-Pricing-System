#ifndef SimulationOrchestrator_HPP
#define SimulationOrchestrator_HPP

#include <Core/Configuration/SimulationConfig.hpp>
#include <BusinessLogic/Mediators/Mediator.hpp>
#include <BusinessLogic/Pricers/Pricer.hpp>

class Sde;
class FdmBase;
class Rng;

struct SimulationInstance
{
	int instanceId_;
	std::unique_ptr<Mediator> mediator_;
	std::vector<std::shared_ptr<Pricer>> pricers;
	std::string sde_;
	std::string fdm_;
	std::string rng_;

	SimulationInstance(int instanceId, const std::string sde,
		const std::string fdm, const std::string rng)
		: instanceId_(instanceId), sde_(sde), fdm_(fdm), rng_(rng) {
	}
};

class SimulationOrchestrator
{
private:
	SimulationConfig config_;
	std::vector<SimulationInstance> simulationInstances_;
	std::unique_ptr<Mediator> mediator_;

	void initializeOptions();
	void buildMediators();

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
	void runWithMode(ExecutionMode mode);
	void printConfiguration() const;
};

#endif
