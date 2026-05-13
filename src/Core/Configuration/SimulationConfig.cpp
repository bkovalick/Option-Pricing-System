#include "Core/Configuration/SimulationConfig.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>

using json = nlohmann::json;

SimulationConfig SimulationConfig::fromJsonEngine(const json& j)
{
    SimulationConfig config;

    try {
        // Parse mediator settings
        if (!j.contains("engine")) {
            throw std::runtime_error("Missing 'engine' section in JSON");
        }

        const auto& engine = j["engine"];

        // Required fields
        if (!engine.contains("type")) {
            throw std::runtime_error("Missing 'engine.type' in JSON");
        }

        std::string engineStr = engine["type"].get<std::string>();
        if (engineStr == "MonteCarlo") {
            config.engineType = EngineType::MonteCarlo;
        }
        else if (engineStr == "BlackScholes") {
            config.engineType = EngineType::BlackScholes;
        }
        else if (engineStr == "BinomialTree") {
            config.engineType = EngineType::BinomialTree;
        }
        else {
            throw std::runtime_error("Unknown engine type: " + engineStr);
        }

        // Execution mode with default
        std::string modeStr = engine.value("executionMode", "single-threaded");
        if (modeStr == "single-threaded") {
            config.executionMode = ExecutionMode::SingleThreaded;
        }
        else if (modeStr == "openmp") {
            config.executionMode = ExecutionMode::OpenMP;
        }
        else if (modeStr == "ppl") {
            config.executionMode = ExecutionMode::PPL;
        }

        // Required numeric fields
        config.numSimulations = engine.value("numSimulations", 10000);
        config.numTimesteps = engine.value("timesteps", 500);
        config.builderChoice = engine.value("builderChoice", 1);

        // Parse options
        if (!j.contains("options")) {
            throw std::runtime_error("Missing 'options' array in JSON");
        }

        for (const auto& opt : j["options"]) {
            config.options.emplace_back(
                opt.value("name", "Unnamed"),
                opt.value("strike", 100.0),
                opt.value("expiration", 1.0),
                opt.value("volatility", 0.2),
                opt.value("dividend", 0.0),
                opt.value("optionType", 1),
                opt.value("interestRate", 0.05),
                opt.value("initialPrice", 100.0)
            );
        }

        // Parse pricers
        if (j.contains("pricers")) {
            for (const auto& pricer : j["pricers"]) {
                config.pricerTypes.push_back(pricer.get<std::string>());
            }
        }
        else {
            std::cerr << "Warning: No pricers specified, using default" << std::endl;
            config.pricerTypes.push_back("EuropeanPricer");
        }

        // Parse SDEs (optional for Monte Carlo)
        if (j.contains("sdes")) {
            for (const auto& sde : j["sdes"]) {
                config.sdeTypes.push_back(sde.get<std::string>());
            }
        }

        // Parse FDMs (optional for Monte Carlo)
        if (j.contains("fdms")) {
            for (const auto& fdm : j["fdms"]) {
                config.fdmTypes.push_back(fdm.get<std::string>());
            }
        }

        // Parse RNGs (optional for Monte Carlo)
        if (j.contains("rngs")) {
            for (const auto& rng : j["rngs"]) {
                config.rngTypes.push_back(rng.get<std::string>());
            }
        }

        // Set defaults for Monte Carlo if not provided
        if (config.mediatorType == MediatorType::MonteCarlo) {
            if (config.sdeTypes.empty()) {
                std::cerr << "Warning: No SDEs specified, using GBM" << std::endl;
                config.sdeTypes.push_back("GBM");
            }
            if (config.fdmTypes.empty()) {
                std::cerr << "Warning: No FDMs specified, using Euler" << std::endl;
                config.fdmTypes.push_back("Euler");
            }
            if (config.rngTypes.empty()) {
                std::cerr << "Warning: No RNGs specified, using MersenneTwister" << std::endl;
                config.rngTypes.push_back("MersenneTwister");
            }
        }
    }
    catch (const json::exception& e) {
        throw std::runtime_error(std::string("JSON parsing error: ") + e.what());
    }

    return config;
}

SimulationConfig SimulationConfig::fromJson(const json& j)
{
    SimulationConfig config;

    try {
        // Parse mediator settings
        if (!j.contains("mediator")) {
            throw std::runtime_error("Missing 'mediator' section in JSON");
        }

        const auto& mediator = j["mediator"];
        
        // Required fields
        if (!mediator.contains("type")) {
            throw std::runtime_error("Missing 'mediator.type' in JSON");
        }
        
        std::string mediatorStr = mediator["type"].get<std::string>();
        if (mediatorStr == "MonteCarlo") {
            config.mediatorType = MediatorType::MonteCarlo;
        } else if (mediatorStr == "BlackScholes") {
            config.mediatorType = MediatorType::BlackScholes;
        } else {
            throw std::runtime_error("Unknown mediator type: " + mediatorStr);
        }
        
        // Execution mode with default
        std::string modeStr = mediator.value("executionMode", "single-threaded");
        if (modeStr == "single-threaded") {
            config.executionMode = ExecutionMode::SingleThreaded;
        } else if (modeStr == "openmp") {
            config.executionMode = ExecutionMode::OpenMP;
        } else if (modeStr == "ppl") {
            config.executionMode = ExecutionMode::PPL;
        }
        
        // Required numeric fields
        config.numSimulations = mediator.value("numSimulations", 10000);
        config.numTimesteps = mediator.value("timesteps", 500);
        config.builderChoice = mediator.value("builderChoice", 1);
        
        // Parse options
        if (!j.contains("options")) {
            throw std::runtime_error("Missing 'options' array in JSON");
        }
        
        for (const auto& opt : j["options"]) {
            config.options.emplace_back(
                opt.value("name", "Unnamed"),
                opt.value("strike", 100.0),
                opt.value("expiration", 1.0),
                opt.value("volatility", 0.2),
                opt.value("dividend", 0.0),
                opt.value("optionType", 1),
                opt.value("interestRate", 0.05),
                opt.value("initialPrice", 100.0)
            );
        }
        
        // Parse pricers
        if (j.contains("pricers")) {
            for (const auto& pricer : j["pricers"]) {
                config.pricerTypes.push_back(pricer.get<std::string>());
            }
        } else {
            std::cerr << "Warning: No pricers specified, using default" << std::endl;
            config.pricerTypes.push_back("EuropeanPricer");
        }
        
        // Parse SDEs (optional for Monte Carlo)
        if (j.contains("sdes")) {
            for (const auto& sde : j["sdes"]) {
                config.sdeTypes.push_back(sde.get<std::string>());
            }
        }
        
        // Parse FDMs (optional for Monte Carlo)
        if (j.contains("fdms")) {
            for (const auto& fdm : j["fdms"]) {
                config.fdmTypes.push_back(fdm.get<std::string>());
            }
        }
        
        // Parse RNGs (optional for Monte Carlo)
        if (j.contains("rngs")) {
            for (const auto& rng : j["rngs"]) {
                config.rngTypes.push_back(rng.get<std::string>());
            }
        }
        
        // Set defaults for Monte Carlo if not provided
        if (config.mediatorType == MediatorType::MonteCarlo) {
            if (config.sdeTypes.empty()) {
                std::cerr << "Warning: No SDEs specified, using GBM" << std::endl;
                config.sdeTypes.push_back("GBM");
            }
            if (config.fdmTypes.empty()) {
                std::cerr << "Warning: No FDMs specified, using Euler" << std::endl;
                config.fdmTypes.push_back("Euler");
            }
            if (config.rngTypes.empty()) {
                std::cerr << "Warning: No RNGs specified, using MersenneTwister" << std::endl;
                config.rngTypes.push_back("MersenneTwister");
            }
        }
    }
    catch (const json::exception& e) {
        throw std::runtime_error(std::string("JSON parsing error: ") + e.what());
    }
    
    return config;
}

bool SimulationConfig::isValid() const
{
    if (options.empty()) return false;
    if (pricerTypes.empty()) return false;
    if (numSimulations <= 0) return false;
    if (numTimesteps <= 0) return false;
    
    // Validate Monte Carlo specific requirements
    if (mediatorType == MediatorType::MonteCarlo) {
        if (sdeTypes.empty() || fdmTypes.empty() || rngTypes.empty()) {
            return false;
        }
    }
    
    return true;
}