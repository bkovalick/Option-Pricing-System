#ifndef SimulationOrchestrator_HPP
#define SimulationOrchestrator_HPP

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <Core/Configuration/SimulationConfig.hpp>
#include <Core/Results/SimulationResults.hpp>
#include <Core/Domain/SimulationInstance.hpp>
#include <Core/Domain/ComponentConfig.hpp>
#include <BusinessLogic/Mediators/Mediator.hpp>
#include <BusinessLogic/Pricers/Pricer.hpp>

class Sde;
class FdmBase;
class Rng;

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
    void exportToCSV(const std::string& filename) const;
    const SimulationResultsContainer& getResults() const { return results_; }
};

#endif
