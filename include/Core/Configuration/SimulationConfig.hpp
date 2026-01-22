#ifndef SimulationConfig_HPP
#define SimulationConfig_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Core/Domain/OptionData.hpp>

// Enums for type safety
enum class MediatorType
{
    MonteCarlo,
    PDE,
    BinomialTree
};

enum class ExecutionMode
{
    SingleThreaded,
    OpenMP,
    PPL
};

struct SimulationConfig
{
    // Mediator configuration
    MediatorType mediatorType;
    ExecutionMode executionMode;
    int numSimulations;
    
    // Option configurations
    std::vector<OptionData> options;
    
    // Component types (as strings from JSON, or use enums)
    std::vector<std::string> pricerTypes;
    std::vector<std::string> sdeTypes;
    std::vector<std::string> fdmTypes;
    std::vector<std::string> rngTypes;
    
    // Optional: Builder choice (if you still need it)
    int builderChoice;
    
    // Factory method to create from JSON
    static SimulationConfig fromJson(const nlohmann::json& json);
    
    // Validation
    bool isValid() const;
};

#endif