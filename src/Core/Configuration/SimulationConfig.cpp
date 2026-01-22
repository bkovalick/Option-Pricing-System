#include "Core/Configuration/SimulationConfig.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>

using json = nlohmann::json;

SimulationConfig SimulationConfig::fromJson(const json& json)
{
    SimulationConfig config;

	// This is where we create concrete structures from the JSON input
    
    // Parse mediator settings
    std::string mediatorStr = json["mediator"]["type"].get<std::string>();
    if (mediatorStr == "MonteCarlo") {
        config.mediatorType = MediatorType::MonteCarlo;
    } else if (mediatorStr == "PDE") {
        config.mediatorType = MediatorType::PDE;
    }
    
    std::string modeStr = json["mediator"]["executionMode"];
    if (modeStr == "single-threaded") {
        config.executionMode = ExecutionMode::SingleThreaded;
    } else if (modeStr == "openmp") {
        config.executionMode = ExecutionMode::OpenMP;
    } else if (modeStr == "ppl") {
        config.executionMode = ExecutionMode::PPL;
    }
    
    config.numSimulations = json["mediator"]["numSimulations"].get<int>();
    config.builderChoice = json["mediator"]["builderChoice"].get<int>();
    
    // Parse options
    for (const auto& opt : json["options"]) {
        config.options.emplace_back(
            opt["name"].get<std::string>(),
            opt["strike"].get<double>(),
            opt["expiration"].get<double>(),
            opt["volatility"].get<double>(),
            opt["dividend"].get<double>(),
            opt["optionType"].get<int>(),
            opt["interestRate"].get<double>(),
            opt["beta"].get<double>(),
            opt["initialPrice"].get<double>()
        );
    }
    
    // Parse component types
    for (const auto& pricer : json["pricers"]) {
        config.pricerTypes.push_back(pricer.get<std::string>());
    }
    
    if (json.contains("sdes")) {
        for (const auto& sde : json["sdes"]) {
            config.sdeTypes.push_back(sde.get<std::string>());
        }
    }
    
    if (json.contains("fdms")) {
        for (const auto& fdm : json["fdms"]) {
            config.fdmTypes.push_back(fdm.get<std::string>());
        }
    }
    
    if (json.contains("rngs")) {
        for (const auto& rng : json["rngs"]) {
            config.rngTypes.push_back(rng.get<std::string>());
        }
    }
    
    return config;
}

bool SimulationConfig::isValid() const
{
    if (options.empty()) return false;
    if (pricerTypes.empty()) return false;
    if (numSimulations <= 0) return false;
    
    // Validate Monte Carlo specific requirements
    if (mediatorType == MediatorType::MonteCarlo) {
        if (sdeTypes.empty() || fdmTypes.empty() || rngTypes.empty()) {
            return false;
        }
    }
    
    return true;
}