#include "BusinessLogic/Factory/PricerFactory.hpp"

std::shared_ptr<Pricer> PricerFactory::createPricer(std::string& pricerType, const OptionData& opt) {
	if (pricerType == "EuropeanPricer") {
		return std::make_shared<EuropeanPricer>(opt);
	}
	else if (pricerType == "AsianPricer") {
		return std::make_shared<AsianPricer>(opt);
	}
	else if (pricerType == "BarrierPricer") {
		return std::make_shared<BarrierPricer>(opt);
	}
	else if (pricerType == "BrownianBridgePricer") {
		return std::make_shared<BrownianBridgePricer>(opt);
	}
	else {
		throw std::invalid_argument("Unknown pricer type: " + pricerType);
	}
}