#ifndef EngineOrchestrator_HPP
#define EngineOrchestrator_HPP

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

#include <BusinessLogic/Factory/PricerFactory.hpp>
#include <BusinessLogic/Factory/SdeFactory.hpp>
#include <BusinessLogic/Factory/FdmFactory.hpp>
#include <BusinessLogic/Factory/RngFactory.hpp>

#include <BusinessLogic/Engines/PricingEngine.hpp>
#include <BusinessLogic/Engines/BlackScholesEngine.hpp>
#include <BusinessLogic/Engines/BinomialTreeEngine.hpp>
#include <BusinessLogic/Engines/MonteCarloEngine.hpp>
#include <BusinessLogic/Models/BlackScholes/BlackScholes.hpp>
#include <BusinessLogic/Models/BinomialTreeModel/BinomialTreeModel.hpp>

class EngineOrchestrator
{
private:
    SimulationConfig config_;
    std::vector<ComponentConfig> componentConfigs_;
    std::vector<SimulationInstance> simulationInstances_;
    std::vector<OptionData> options_;
    std::vector<std::shared_ptr<Pricer>> pricers_;
    SimulationResultsContainer results_;

    void initializeOptions();
    void createComponentsConfig();
    void initializeSimulations();

    SimulationInstance createSimulationInstance(int instanceId, const OptionData& option,
        const ComponentConfig& components);
    SimulationInstance createSimulationInstanceMonteCarlo(int instanceId, const OptionData& option,
        const ComponentConfig& components);
    SimulationInstance createSimulationInstanceBlackScholes(int instanceId, const OptionData& option);
    SimulationInstance createSimulationInstancBinomialTree(int instanceId, const OptionData& option);

public:
	explicit EngineOrchestrator(const SimulationConfig& config);
	virtual ~EngineOrchestrator() = default;

	EngineOrchestrator(const EngineOrchestrator& orch) = delete;
	EngineOrchestrator& operator=(const EngineOrchestrator& orch) = delete;
	EngineOrchestrator(EngineOrchestrator&& orch) = default;
	EngineOrchestrator& operator=(EngineOrchestrator&& orch) = default;

	void run();
    void exportToCSV(const std::string& filename) const;
    const SimulationResultsContainer& getResults() const { return results_; }
};

#endif 

