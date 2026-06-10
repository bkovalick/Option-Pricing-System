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

    // For analytical engine, create single placeholder config
    if (config_.engineType == EngineType::BlackScholes ||
        config_.engineType == EngineType::BinomialTree) {
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
        try {
            switch (config_.engineType) {

            case EngineType::MonteCarlo: 
            {
                for (const auto& component : componentConfigs_) {
                    for (const auto& pricerType : config_.pricerTypes) {
                        auto pricer = PricerFactory::createPricer(pricerType, option);
                        if (pricer) {
                            auto simulationInstance = createSimulationInstanceMonteCarlo(
                                instanceId, option, component, pricer, pricerType);
                            simulationInstances_.push_back(std::move(simulationInstance));
                            instanceId++;
                        }
                    }
                }
                break;
            }
            case EngineType::BlackScholes:
            {
                auto simulationInstance = createSimulationInstanceBlackScholes(instanceId, option);
                simulationInstances_.push_back(std::move(simulationInstance));
                instanceId++;
                break;
            }
            case EngineType::BinomialTree:
            {
                auto simulationInstance = createSimulationInstancBinomialTree(instanceId, option);
                simulationInstances_.push_back(std::move(simulationInstance));
                instanceId++;
                break;
            }
            default:
                throw std::runtime_error("Unknown engine type");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "  Error creating instance " << instanceId << ": " << e.what() << std::endl;
        }
    }
}

SimulationInstance EngineOrchestrator::createSimulationInstanceMonteCarlo(
    int instanceId,
    const OptionData& option,
    const ComponentConfig& component,
    const std::shared_ptr<Pricer>& pricer,
    const std::string& pricerType)
{
    std::cout << "\n=== Building singular monte carlo simulation instance === " +
        std::to_string(instanceId) << std::endl;
    SimulationInstance simulationInstance(
        instanceId, "MonteCarlo", component.sdeType + component.fdmType, component.rngType, option.OptionName, pricerType
    );

    auto sde = SdeFactory::createSde(component.sdeType, option);
    auto fdm = FdmFactory::createFdm(component.fdmType, option, sde, config_.numTimesteps);
    auto rng = RngFactory::createRng(component.rngType);

    auto engine = std::make_unique<MonteCarloEngine>(sde, fdm, rng, pricer, config_.numSimulations, config_.executionMode);
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
        instanceId, "BlackScholes", "Analytical", "Deterministic", option.OptionName, option.OptionName
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
        instanceId, "BinomialTree", "Analytical", "Deterministic", option.OptionName, option.OptionName
    );

    return simulationInstance;
}

void EngineOrchestrator::run()
{
    auto start = std::chrono::high_resolution_clock::now();

    results_.resize(simulationInstances_.size());

    std::transform(std::execution::par,
        simulationInstances_.begin(),
        simulationInstances_.end(),
        results_.begin(),
        [this](const SimulationInstance& simulation) {

            SimulationResult result;

            try {
                auto start = std::chrono::high_resolution_clock::now();
                double price = simulation.engine_->computePrice();
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;

                result.instanceId = simulation.instanceId_;
                result.optionName = simulation.optionName_;
                result.methodType = simulation.methodType_;
                result.algorithmDetail = simulation.algorithmDetail_;
                result.executionMode = simulation.executionMode_;
                result.pricerType = simulation.pricerType_;
                result.price = price;
                result.computationTime = elapsed.count();
                if (simulation.methodType_ == toString(EngineType::MonteCarlo)) {
                    result.numSimulations = config_.numSimulations;
                    result.numTimesteps = config_.numTimesteps;
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Error in instance " << simulation.instanceId_
                          << ": " << e.what() << std::endl;
            }

            return result;
        });

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Parallel Elapsed Time " << elapsed << std::endl;
}

void EngineOrchestrator::exportToCSV(const std::string& filename) const
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << " for writing" << std::endl;
        return;
    }

    // Header
    file << "InstanceID,OptionName,MethodType,Algorithm,ExecutionMode,PricerType,Price,ComputationTime,NumSimulations,NumTimesteps\n";

    // Data
    for (const auto& result : results_) {
        file << result.instanceId << ","
            << result.optionName << ","
            << result.methodType << ","
            << result.algorithmDetail << ","
            << result.executionMode << ","
            << result.pricerType << ","
            << std::fixed << std::setprecision(10) << result.price << ","
            << result.computationTime << ","
            << result.numSimulations << ","
            << result.numTimesteps << "\n";
    }

    file.close();
    std::cout << "Results exported to " << filename << std::endl;
}