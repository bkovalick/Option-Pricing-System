#include "BusinessLogic/Orchestrator/SimulationOrchestrator.hpp"
#include <BusinessLogic/Factory/MediatorFactory.hpp>
#include <BusinessLogic/Factory/PricerFactory.hpp>
#include <BusinessLogic/Mediators/MonteCarloMediator.hpp>
#include <BusinessLogic/Factory/SdeFactory.hpp>
#include <BusinessLogic/Factory/FdmFactory.hpp>
#include <BusinessLogic/Factory/RngFactory.hpp>

SimulationOrchestrator::SimulationOrchestrator(const SimulationConfig& config)
    : config_(config)
{
    initializeOptions();
    createComponentConfigs();
    buildMediators();
}

void SimulationOrchestrator::initializeOptions()
{
    std::cout << "=== Initializing Simulation Orchestrator ===" << std::endl;
    std::cout << "Mediator Type: " << static_cast<int>(config_.mediatorType) << std::endl;
    std::cout << "Execution Mode: " << static_cast<int>(config_.executionMode) << std::endl;
    std::cout << "Number of Simulations per Mediator: " << config_.numSimulations << std::endl;
    
    options_.clear();
    options_.reserve(config_.options.size());
    
    for (const auto& opt : config_.options) {
        std::cout << "  - Option: " << opt.OptionName << std::endl;
        options_.push_back(opt);
    }
}

void SimulationOrchestrator::createComponentConfigs()
{
    std::cout << "\n=== Creating all possible Monte Carlo Component Configurations ===" << std::endl;
	componentConfigs_.clear();
    componentConfigs_.reserve(
        config_.sdeTypes.size() *
        config_.fdmTypes.size() *
        config_.rngTypes.size());

    for (const auto& sdeType : config_.sdeTypes) {
        for (const auto& fdmType : config_.fdmTypes) {
            for (const auto& rngType : config_.rngTypes) {
                ComponentConfig compConfig{ sdeType, fdmType, rngType };
				componentConfigs_.push_back(compConfig);
            }
        }
    }
}
SimulationInstance SimulationOrchestrator::createSimulationInstance(
    int instanceId,
    const OptionData& option,
    const ComponentConfig& components) const
{
    std::cout << "\n=== Building singular simulation instance ===" << std::endl;
    SimulationInstance simulationInstance(
        instanceId, components.sdeType, components.fdmType, components.rngType, option.OptionName
    );

    auto sde = SdeFactory::createSde(components.sdeType, option);
    auto fdm = FdmFactory::createFdm(components.fdmType, option, sde, config_.numTimesteps);
    auto rng = RngFactory::createRng(components.rngType);

    if (!sde || !fdm || !rng) {
        throw std::runtime_error("Failed to create components for instance " +
            std::to_string(instanceId));
    }

    auto parts = std::make_tuple(sde, fdm, rng);
    simulationInstance.mediator_ = std::make_unique<MonteCarloMediator>(parts, config_.numSimulations);

    return simulationInstance;
}

void SimulationOrchestrator::attachPricers(SimulationInstance& simulationInstance, 
    const OptionData& option)
{
    std::cout << "\n=== Attaching Pricers to Options ===" << std::endl;
    int slot = 0;
    for (const auto& pricerType : config_.pricerTypes) {
        auto pricer = PricerFactory::createPricer(pricerType, option);
        if (pricer) {
            simulationInstance.pricers_[pricerType] = pricer;
            simulationInstance.mediator_->connectPricers(pricer, slot);
            slot++;
        }
    }
}

void SimulationOrchestrator::buildMediators()
{
    std::cout << "\n=== Building Simulation Instances ===" << std::endl;

    int instanceId = 0;
    for (const auto& option : options_) {
        for (const auto& componentConfig : componentConfigs_) {
            try
            {
                auto simulationInstance = createSimulationInstance(instanceId, option, componentConfig);

                attachPricers(simulationInstance, option);

                simulationInstances_.push_back(std::move(simulationInstance));
                instanceId++;
            }
            catch (const std::exception& e) {
                std::cerr << "  Error creating instance " << instanceId
                    << ": " << e.what() << std::endl;
            }
        }
    }
}

void SimulationOrchestrator::run()
{
    std::cout << "\n=== Running All Simulations ===" << std::endl;
    
    results_.clear();  // Clear previous results
    auto overallStart = std::chrono::high_resolution_clock::now();
    
    for (const auto& simulation : simulationInstances_) {
        std::cout << "\n--- Instance " << simulation.instanceId_ << " ---" << std::endl;
        std::cout << "Config: " << simulation.sde_ << " × " 
                  << simulation.fdm_ << " × " << simulation.rng_ << std::endl;

        auto start = std::chrono::high_resolution_clock::now();

        // Run simulation
        switch (config_.executionMode) {
        case ExecutionMode::SingleThreaded:
            simulation.mediator_->start();
            break;
        case ExecutionMode::OpenMP:
            simulation.mediator_->startOpenMP();
            break;
        case ExecutionMode::PPL:
            simulation.mediator_->startPPL();
            break;
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        // Capture results
        SimulationResult result;
        result.instanceId = simulation.instanceId_;
        result.optionName = simulation.optionName_;
        result.mediatorType = toString(config_.mediatorType);
        result.sdeType = simulation.sde_;
        result.fdmType = simulation.fdm_;
        result.rngType = simulation.rng_;
        result.numSimulations = config_.numSimulations;
        result.numTimesteps = config_.numTimesteps;
        result.computationTime = elapsed.count();
        
        // Extract pricer results
        for (const auto& pricer : simulation.pricers_) {
			const std::string& pricerType = pricer.first;
            double price = pricer.second->Price();
            result.addPricerResult(pricerType, price);
        }
        
        results_.addResult(result);

        std::cout << "Completed in " << elapsed.count() << " seconds" << std::endl;
    }
    
    auto overallEnd = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> totalElapsed = overallEnd - overallStart;
    
    std::cout << "\n=== All Simulations Complete ===" << std::endl;
    std::cout << "Total time: " << totalElapsed.count() << " seconds" << std::endl;
    std::cout << "Average per instance: " << results_.getAverageTime() << " seconds" << std::endl;
}

void SimulationOrchestrator::printResults() const
{
    std::cout << "\n" << std::endl;
    std::cout << "SIMULATION RESULTS SUMMARY" << std::endl;
    const auto& allResults = results_.getResults();
    
    for (const auto& result : allResults) {
        std::cout << "\n━━━ Instance " << result.instanceId << " ━━━" << std::endl;
        std::cout << "Option: " << result.optionName << std::endl;
        std::cout << "Config: " << result.sdeType << " × " << result.fdmType << " × " << result.rngType << std::endl;
        std::cout << "Time: " << std::fixed << std::setprecision(4) << result.computationTime << "s" << std::endl;
        std::cout << "\nPrices:" << std::endl;
        
        for (const auto& pricerResult : result.pricerResults) {
            std::cout << "  " << std::setw(20) << std::left << pricerResult.pricerType 
                      << ": " << std::setw(12) << std::right << std::fixed << std::setprecision(6) 
                      << pricerResult.price << std::endl;
        }
    }
}

void SimulationOrchestrator::exportToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing" << std::endl;
        return;
    }
    
    // Header
    file << "InstanceID,OptionName,SDE,FDM,RNG,PricerType,Price,ComputationTime,NumSimulations\n";
    
    // Data
    const auto& allResults = results_.getResults();
    for (const auto& result : allResults) {
        for (const auto& pricerResult : result.pricerResults) {
            file << result.instanceId << ","
                 << result.optionName << ","
                 << result.sdeType << ","
                 << result.fdmType << ","
                 << result.rngType << ","
                 << pricerResult.pricerType << ","
                 << std::fixed << std::setprecision(10) << pricerResult.price << ","
                 << result.computationTime << ","
                 << result.numSimulations << "\n";
        }
    }
    
    file.close();
    std::cout << "Results exported to " << filename << std::endl;
}

void SimulationOrchestrator::printConfiguration() const
{
    std::cout << "\n=== Orchestrator Configuration ===" << std::endl;
    std::cout << "Simulation Instances: " << simulationInstances_.size() << std::endl;
    std::cout << "Options: " << options_.size() << std::endl;
    std::cout << "Simulations per Instance: " << config_.numSimulations << std::endl;
    std::cout << "Timesteps: " << config_.numTimesteps << std::endl;
}