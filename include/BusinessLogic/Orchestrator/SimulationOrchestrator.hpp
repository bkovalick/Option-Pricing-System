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
#include <BusinessLogic/Pricers/Pricer.hpp>

#include <BusinessLogic/Factory/PricerFactory.hpp>
#include <BusinessLogic/Factory/SdeFactory.hpp>
#include <BusinessLogic/Factory/FdmFactory.hpp>
#include <BusinessLogic/Factory/RngFactory.hpp>

#include <BusinessLogic/Mediators/Mediator.hpp>
#include <BusinessLogic/Mediators/MonteCarloMediator.hpp>
#include <BusinessLogic/Mediators/BlackScholesMediator.hpp>
#include "BusinessLogic/Models/BlackScholes/BlackScholes.hpp"

class Sde;
class FdmBase;
class Rng;

std::string toString(const MediatorType& type);

class SimulationOrchestrator
{
private:
    SimulationConfig config_;
    std::vector<ComponentConfig> componentConfigs_;
    std::vector<SimulationInstance> simulationInstances_;
    std::vector<OptionData> options_;
    SimulationResultsContainer results_;

    void initializeOptions();
	void createComponentsConfig();
    void initializeSimulations();

    SimulationInstance createSimulationInstance(int instanceId, const OptionData& option,
        const ComponentConfig& components) const;
    SimulationInstance createSimulationInstanceMonteCarlo(int instanceId, const OptionData& option,
        const ComponentConfig& components) const;
    SimulationInstance createSimulationInstanceBlackScholes(int instanceId, const OptionData& option) const;
    void attachPricers(SimulationInstance& simulationInstance, const OptionData& option);

public:
    explicit SimulationOrchestrator(const SimulationConfig& config);
    ~SimulationOrchestrator() = default;

    SimulationOrchestrator(const SimulationOrchestrator&) = delete;
    SimulationOrchestrator& operator=(const SimulationOrchestrator&) = delete;

    SimulationOrchestrator(SimulationOrchestrator&&) = default;
    SimulationOrchestrator& operator=(SimulationOrchestrator&&) = default;

    void run();
    void exportToCSV(const std::string& filename) const;
    const SimulationResultsContainer& getResults() const { return results_; }
};

#endif
