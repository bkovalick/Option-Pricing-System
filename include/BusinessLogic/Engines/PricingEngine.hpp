#ifndef PricingEngine_HPP
#define PricingEngine_HPP

#include <iostream>
#include <Core/Domain/Greeks.hpp>

class PricingEngine
{
protected:
	std::string engineType_;
	PricingEngine(const std::string& engineType) : engineType_(engineType) {}

public:
	virtual ~PricingEngine() = default;

	PricingEngine(const PricingEngine& other) = delete;
	PricingEngine& operator=(const PricingEngine& other) = delete;
	PricingEngine(PricingEngine&&) = default;
	PricingEngine& operator=(PricingEngine&&) = default;

	virtual double computePrice() = 0;
	//Greeks computeGreeks();
	//virtual Greeks computeGreeks() = 0;

	std::string getEngineType() const { return engineType_; }
};

#endif