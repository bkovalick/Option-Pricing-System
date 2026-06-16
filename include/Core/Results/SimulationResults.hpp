#ifndef SimulationResults_HPP
#define SimulationResults_HPP

#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <mutex>
#include <Core/Domain/OptionData.hpp>

// Individual pricer result
struct PricerResult
{
    std::string pricerType;
    double price;
    double standardError;
    int numSimulations;
    
    PricerResult(const std::string& type, double p, double se = 0.0, int n = 0)
        : pricerType(type), price(p), standardError(se), numSimulations(n) {}
};

// Complete simulation instance result
struct SimulationResult
{
    // Identification
    int instanceId;
    std::string optionName;
    
    // Configuration
    std::string mediatorType;
    std::string methodType;
    std::string algorithmDetail;
    std::string executionMode;
    
    // Computational details
    int numSimulations;
    int numTimesteps;
    double computationTime;  // seconds
    double price;
    std::string pricerType;
    
    SimulationResult() 
        : instanceId(0), numSimulations(0), numTimesteps(0), computationTime(0.0), price(0.0), pricerType("European")
        {}
};

#endif