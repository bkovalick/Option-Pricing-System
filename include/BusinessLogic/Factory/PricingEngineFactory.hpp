#ifndef PricingEngineFactory_HPP
#define PricingEngineFactory_HPP

#include <iostream>
#include <string>
#include <BusinessLogic/Engines/PricingEngine.hpp>

class PricingEngineFactory {
public:
	static std::unique_ptr<PricingEngine> createPricingEngine(std::string engineType);	
};

#endif