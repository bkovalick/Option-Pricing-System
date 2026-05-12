#include <BusinessLogic/Orchestrator/EngineOrchestrator.hpp>

std::string toString(const EngineType& engineType) {
    switch (engineType) {
    case EngineType::MonteCarlo:
        return "MonteCarlo";
    case EngineType::BlackScholes:
        return "BlackScholes";
    case EngineType::BinomialTree:
        return "BinomialTree";
    default:
        return "Unknown";
    }

}
EngineOrchestrator::EngineOrchestrator(const SimulationConfig& config)
    : config_(config)
{
    initializeOptions();
    createComponentsConfig();
    initializeSimulations();
}

void EngineOrchestrator::initializeOptions()
{
    std::cout << "=== Initializing Simulation Orchestrator ===" << std::endl;

    options_.clear();
    options_.reserve(config_.options.size());

    for (const auto& opt : config_.options) {
        std::cout << "  - Option: " << opt.OptionName << std::endl;
        options_.push_back(opt);
    }
}

void EngineOrchestrator::createComponentsConfig()
{
    std::cout << "\n=== Creating Component Configurations ===" << std::endl;
    componentConfigs_.clear();

    // For analytical mediators, create single placeholder config
    if (config_.engineType == EngineType::BlackScholes ||
        config_.engineType == EngineType::BinomialTree) {
        std::cout << "Analytical mediator - using placeholder configuration" << std::endl;
        componentConfigs_.emplace_back("None", "None", "None");
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

void EngineOrchestrator::initializeSimulations()
{
    std::cout << "\n=== Building Simulation Instances ===" << std::endl;
    int instanceId = 0;
    for (const auto& option : options_) {
        for (const auto& componentConfig : componentConfigs_) {
            try {
                auto simulationInstance = createSimulationInstance(instanceId, option, componentConfig);
                simulationInstances_.push_back(std::move(simulationInstance));
                instanceId++;
            }
            catch (const std::exception& e) {
                std::cerr << "  Error creating instance " << instanceId << ": " << e.what() << std::endl;
            }
        }
    }
}

SimulationInstance EngineOrchestrator::createSimulationInstance(
    int instanceId,
    const OptionData& option,
    const ComponentConfig& component)
{
    switch (config_.engineType) {
    case EngineType::MonteCarlo:
        return createSimulationInstanceMonteCarlo(instanceId, option, component);

    case EngineType::BlackScholes:
        return createSimulationInstanceBlackScholes(instanceId, option);

    case EngineType::BinomialTree:
        return createSimulationInstancBinomialTree(instanceId, option);

    default:
        throw std::runtime_error("Unknown mediator type");
    }
}

SimulationInstance EngineOrchestrator::createSimulationInstanceMonteCarlo(
    int instanceId,
    const OptionData& option,
    const ComponentConfig& component)
{
    std::cout << "\n=== Building singular monte carlo simulation instance === " +
        std::to_string(instanceId) << std::endl;
    SimulationInstance simulationInstance(
        instanceId, "MonteCarlo", component.sdeType + component.fdmType, component.rngType, option.OptionName
    );

    auto sde = SdeFactory::createSde(component.sdeType, option);
    auto fdm = FdmFactory::createFdm(component.fdmType, option, sde, config_.numTimesteps);
    auto rng = RngFactory::createRng(component.rngType);

    auto engine = std::make_unique<MonteCarloEngine>(sde, fdm, rng, config_.numSimulations);
    for (const auto& pricerType : config_.pricerTypes) {
        auto pricer = PricerFactory::createPricer(pricerType, option);
        if (pricer) {
            pricers_.push_back(pricer);
        }
    }

    simulationInstance.engine_ = std::move(engine);
    return simulationInstance;
}

SimulationInstance EngineOrchestrator::createSimulationInstanceBlackScholes(
    int instanceId,
    const OptionData& option)
{
    std::cout << "\n=== Building singular black scholes simulation instance === " +
        std::to_string(instanceId) << std::endl;
    SimulationInstance simulationInstance(
        instanceId, "BlackScholes", "Analytical", "Deterministic", option.OptionName
    );

    auto blackScholesModel = std::make_shared<BlackScholes>(option);
    simulationInstance.engine_ = std::make_unique<BlackScholesEngine>(blackScholesModel);
    return simulationInstance;
}

SimulationInstance EngineOrchestrator::createSimulationInstancBinomialTree(int instanceId, const OptionData& option)
{
    std::cout << "\n=== Building binomial tree simulation instance === " +
        std::to_string(instanceId) << std::endl;
    SimulationInstance simulationInstance(
        instanceId, "BinomialTree", "Analytical", "Deterministic", option.OptionName
    );

    return simulationInstance;
}

void EngineOrchestrator::run()
{
    for (const auto& simulation : simulationInstances_) {
        SimulationResult result;

        auto engine = simulation.engine_.get();
        if (simulation.methodType_ == "MonteCarlo") {
            //auto* mcEngine = dynamic_cast<MonteCarloEngine*>(simulation.engine_.get());

            for (auto& pricer : pricers_) {
                if (pricer) {
                    /*mcEngine->addPricer(std::move(pricer));*/
                    engine->addPricer(std::move(pricer));
                    auto start = std::chrono::high_resolution_clock::now();
                    double price = engine->computePrice();
                    auto end = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed = end - start;

                    result.instanceId = simulation.instanceId_;
                    result.optionName = simulation.optionName_;
                    result.methodType = simulation.methodType_;
                    result.algorithmDetail = simulation.algorithmDetail_;
                    result.executionMode = simulation.executionMode_;
                    result.price = price;
                    result.numSimulations = config_.numSimulations;
                    result.numTimesteps = config_.numTimesteps;
                    result.computationTime = elapsed.count();
                    results_.addResult(result);
                }
            }
        }
        else {
            auto start = std::chrono::high_resolution_clock::now();
            double price = engine->computePrice();
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;

            result.instanceId = simulation.instanceId_;
            result.optionName = simulation.optionName_;
            result.methodType = simulation.methodType_;
            result.algorithmDetail = simulation.algorithmDetail_;
            result.executionMode = simulation.executionMode_;
            result.price = price;
            result.computationTime = elapsed.count();
            results_.addResult(result);
        }
    }
}

void EngineOrchestrator::exportToCSV(const std::string& filename) const
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