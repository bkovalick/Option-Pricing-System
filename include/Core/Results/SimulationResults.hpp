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

    // Results from all pricers
    std::vector<PricerResult> pricerResults;
    
    SimulationResult() 
        : instanceId(0), numSimulations(0), numTimesteps(0), computationTime(0.0), price(0.0), pricerType("European")
        {}
    
    // Add a pricer result
    void addPricerResult(const std::string& type, double price, double stdError = 0.0) {
        pricerResults.emplace_back(type, price, stdError, numSimulations);
    }
    
    // Get price for specific pricer type
    double getPriceForPricer(const std::string& type) const {
        for (const auto& result : pricerResults) {
            if (result.pricerType == type) return result.price;
        }
        return 0.0;
    }
};

// Container for all simulation results
class SimulationResultsContainer
{
private:
    std::vector<SimulationResult> results_;
    double totalComputationTime_;
    mutable std::mutex mutex_;

public:
    SimulationResultsContainer() : totalComputationTime_(0.0) {}
    
    void addResult(const SimulationResult& result) {
        std::lock_guard<std::mutex> lock(mutex_);
        results_.push_back(result);
        totalComputationTime_ += result.computationTime;
    }
    
    const std::vector<SimulationResult>& getResults() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return results_; 
    }
    
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return results_.size(); 
    }
    
    double getTotalTime() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return totalComputationTime_; 
    }
    
    double getAverageTime() const { 
        std::lock_guard<std::mutex> lock(mutex_);
        return results_.empty() ? 0.0 : totalComputationTime_ / results_.size(); 
    }
    
    // Get all results for a specific option
    std::vector<const SimulationResult*> getResultsForOption(const std::string& optionName) const {
        std::lock_guard<std::mutex> lock(mutex_);
        std::vector<const SimulationResult*> filtered;
        for (const auto& result : results_) {
            if (result.optionName == optionName) {
                filtered.push_back(&result);
            }
        }
        return filtered;
    }
    
    // Get baseline result (first instance)
    const SimulationResult* getBaseline() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return results_.empty() ? nullptr : &results_[0];
    }
    
    // Clear all results
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        results_.clear();
        totalComputationTime_ = 0.0;
    }
};

#endif