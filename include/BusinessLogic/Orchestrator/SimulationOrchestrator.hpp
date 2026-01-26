#ifndef SimulationOrchestrator_HPP
#define SimulationOrchestrator_HPP

#include <Core/Configuration/SimulationConfig.hpp>
#include <Core/Results/SimulationResults.hpp>
#include <BusinessLogic/Mediators/Mediator.hpp>
#include <BusinessLogic/Pricers/Pricer.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>

class Sde;
class FdmBase;
class Rng;

struct ComponentConfig
{
    std::string sdeType;
    std::string fdmType;
	std::string rngType;
};

struct SimulationInstance
{
    int instanceId_;
    std::unique_ptr<Mediator> mediator_;
    std::unordered_map<std::string, std::shared_ptr<Pricer>> pricers_;
    std::string sde_;
    std::string fdm_;
    std::string rng_;
    std::string optionName_;

    SimulationInstance(int instanceId, const std::string& sde,
        const std::string& fdm, const std::string& rng, const std::string& optionName)
        : instanceId_(instanceId), sde_(sde), fdm_(fdm), rng_(rng), optionName_(optionName) {}
    
    SimulationInstance(SimulationInstance&&) = default;
    SimulationInstance& operator=(SimulationInstance&&) = default;
    
    SimulationInstance(const SimulationInstance&) = delete;
    SimulationInstance& operator=(const SimulationInstance&) = delete;
};

class SimulationOrchestrator
{
private:
    SimulationConfig config_;
    std::vector<ComponentConfig> componentConfigs_;
    std::vector<SimulationInstance> simulationInstances_;
    std::vector<OptionData> options_;
    SimulationResultsContainer results_;

    void initializeOptions();
	void createComponentConfigs();
    void buildMediators();
    SimulationInstance createSimulationInstance(int instanceId, const OptionData& option,
        const ComponentConfig& components) const;
    void attachPricers(SimulationInstance& simulationInstance, const OptionData& option);

public:
    explicit SimulationOrchestrator(const SimulationConfig& config);
    ~SimulationOrchestrator() = default;

    SimulationOrchestrator(const SimulationOrchestrator&) = delete;
    SimulationOrchestrator& operator=(const SimulationOrchestrator&) = delete;

    SimulationOrchestrator(SimulationOrchestrator&&) = default;
    SimulationOrchestrator& operator=(SimulationOrchestrator&&) = default;

    void run();
    
    void printConfiguration() const;
    void printResults() const;
    
    // Export results
    void exportToCSV(const std::string& filename) const;
    
    // Access results
    const SimulationResultsContainer& getResults() const { return results_; }
};

#endif
