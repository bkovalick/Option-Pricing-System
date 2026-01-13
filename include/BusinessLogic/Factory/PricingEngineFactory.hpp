#ifndef PricingEngineFactory_HPP
#define PricingEngineFactory_HPP

#include <iostream>
#include <string>
#include <BusinessLogic//Models/PricingModel.hpp>

class PricingEngineFactory {
public:
	static std::unique_ptr<PricingEngine> createPricingEngine(std::string engineType);	
};

#endif