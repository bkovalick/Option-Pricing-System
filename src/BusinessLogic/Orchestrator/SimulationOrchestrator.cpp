#include "BusinessLogic/Orchestrator/SimulationOrchestrator.hpp"

std::string toString(const MediatorType& mediatorType) {
    switch (mediatorType) {
    case MediatorType::MonteCarlo:
        return "MonteCarlo";
    case MediatorType::BlackScholes:
        return "BlackScholes";
    case MediatorType::BinomialTree:
        return "BinomialTree";
    default:
        return "Unknown";
    }
}

SimulationOrchestrator::SimulationOrchestrator(const SimulationConfig& config)
    : config_(config)
{
    initializeOptions();
    createComponentsConfig();
    initializeSimulations();
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

void SimulationOrchestrator::createComponentsConfig()
{
    std::cout << "\n=== Creating Component Configurations ===" << std::endl;
    componentConfigs_.clear();

    // For analytical mediators, create single placeholder config
    if (config_.mediatorType == MediatorType::BlackScholes ||
        config_.mediatorType == MediatorType::BinomialTree) {

        componentConfigs_.emplace_back("Analytical", "Analytical", "Analytical");
        std::cout << "Analytical mediator - using placeholder configuration" << std::endl;
        return;
    }

    // For Monte Carlo, create all combinations
    componentConfigs_.reserve(
        config_.sdeTypes.size() *
        config_.fdmTypes.size() *
        config_.rngTypes.size());

    for (const auto& sdeType : config_.sdeTypes) {
        for (const auto& fdmType : config_.fdmTypes) {
            for (const auto& rngType : config_.rngTypes) {
                componentConfigs_.emplace_back(sdeType, fdmType, rngType);
            }
        }
    }
    std::cout << "Created " << componentConfigs_.size() << " component combinations" << std::endl;
}

void SimulationOrchestrator::initializeSimulations()
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
                std::cerr << "  Error creating instance " << instanceId << ": " << e.what() << std::endl;
            }
        }
    }
}

SimulationInstance SimulationOrchestrator::createSimulationInstance(
    int instanceId,
    const OptionData& option,
    const ComponentConfig& component) const
{
    switch (config_.mediatorType) {
    case MediatorType::MonteCarlo:
        return createSimulationInstanceMonteCarlo(instanceId, option, component);

    case MediatorType::BlackScholes:
        return createSimulationInstanceBlackScholes(instanceId, option);

    default:
        throw std::runtime_error("Unknown mediator type");
    }
}

SimulationInstance SimulationOrchestrator::createSimulationInstanceMonteCarlo(
    int instanceId,
    const OptionData& option,
    const ComponentConfig& component) const
{
    std::cout << "\n=== Building singular monte carlo simulation instance ===" << std::endl;
    SimulationInstance simulationInstance(
        instanceId, component.sdeType, component.fdmType, component.rngType, option.OptionName
    );

    auto sde = SdeFactory::createSde(component.sdeType, option);
    auto fdm = FdmFactory::createFdm(component.fdmType, option, sde, config_.numTimesteps);
    auto rng = RngFactory::createRng(component.rngType);

    if (!sde || !fdm || !rng) {
        throw std::runtime_error("Failed to create components for instance " +
            std::to_string(instanceId));
    }

    auto parts = std::make_tuple(sde, fdm, rng);
    simulationInstance.mediator_ = std::make_unique<MonteCarloMediator>(parts, config_.numSimulations);
    return simulationInstance;
}

SimulationInstance SimulationOrchestrator::createSimulationInstanceBlackScholes(
    int instanceId,
    const OptionData& option) const
{
    std::cout << "\n=== Building singular black scholes simulation instance ===" << std::endl;
    SimulationInstance simulationInstance(
        instanceId, "BlackScholes", "Analytical", "Deterministic", option.OptionName
    );

	auto blackScholesModel = std::make_shared<BlackScholes>(option);
    simulationInstance.mediator_ = std::make_unique<BlackScholesMediator>(blackScholesModel);
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

void SimulationOrchestrator::run()
{
    std::cout << "\n=== Running All Simulations ===" << std::endl;
    
    results_.clear();  // Clear previous results
    auto overallStart = std::chrono::high_resolution_clock::now();
    
    for (const auto& simulation : simulationInstances_) {
        std::cout << "\n--- Instance " << simulation.instanceId_ << " ---" << std::endl;
        std::cout << "Config: " << simulation.methodType_ << " × "
                  << simulation.algorithmDetail_ << " × " << simulation.executionMode_ << std::endl;

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
        result.methodType = simulation.methodType_;
        result.algorithmDetail = simulation.algorithmDetail_;
        result.executionMode = simulation.executionMode_;
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
                 << result.methodType << ","
                 << result.algorithmDetail << ","
                 << result.executionMode << ","
                 << pricerResult.pricerType << ","
                 << std::fixed << std::setprecision(10) << pricerResult.price << ","
                 << result.computationTime << ","
                 << result.numSimulations << "\n";
        }
    }
    
    file.close();
    std::cout << "Results exported to " << filename << std::endl;
}