#ifndef PricingEngine_HPP
#define PricingEngine_HPP

#include <iostream>

class PricingEngine
{
protected:
	std::string engineType_;

	PricingEngine(const std::string& engineType) : engineType_(engineType) {}

public:
	virtual ~PricingEngine() = default;

	PricingEngine(const PricingEngine&) = delete;
	PricingEngine& operator=(const PricingEngine&) = delete;
	PricingEngine(PricingEngine&&) = default;
	PricingEngine& operator=(PricingEngine&&) = default;

	virtual double computePrice() = 0;

	std::string getEngineType() const { return engineType_; }
};

#endif