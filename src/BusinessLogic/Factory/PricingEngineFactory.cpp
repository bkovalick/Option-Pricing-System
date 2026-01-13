

#include "BusinessLogic/Factory/PricingEngineFactory.hpp"
//#include "BusinessLogic//Models/MonteCarlo/"


std::unique_ptr<PricingEngine> PricingEngineFactory::createPricingEngine(std::string engineType) {
	//if (engineType == "European") {
	//	return std::make_unique<EuropeanPricer>();
	//}  
	//else if (engineType == "BrownianBridge") {
	//	return std::make_unique<BrownianBridgePricer>();
	//} 
	//else {
	//	throw std::invalid_argument("Unknown pricing engine type: " + engineType);
	//}
}