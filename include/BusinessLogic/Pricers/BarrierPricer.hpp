// Author: Ben Kovalick
// Purpose: Header file for the Barrier option class

#ifndef BarrierPricer_HPP
#define BarrierPricer_HPP

#include "Pricer.hpp"
#include <vector>

enum class BarrierType {
    DownAndOut,   // Knocked out if price falls below barrier
    UpAndOut      // Knocked out if price rises above barrier
};

class BarrierPricer : public Pricer
{
private:
    double price;
    double sum;
    int NSim;
    double barrier;
    double rebate;
    BarrierType barrierType;

public:
    // Default constructor (uses default barrier)
    BarrierPricer(const Payoff& payoff, const Discount& discounter);
    
    // Parameterized constructor (specify barrier)
    BarrierPricer(const Payoff& payoff, const Discount& discounter,
                  double barrierLevel, BarrierType type = BarrierType::DownAndOut,
                  double rebateAmount = 0.0);
    
    virtual ~BarrierPricer();
    
    void ProcessPath(const std::vector<double>& t) override;
    double DiscountFactor() override;
    void PostProcess() override;
    double Price() override;
    
    // Setters for dynamic configuration
    void setBarrier(double level) { barrier = level; }
    void setBarrierType(BarrierType type) { barrierType = type; }
    void setRebate(double amount) { rebate = amount; }
};

#endif
