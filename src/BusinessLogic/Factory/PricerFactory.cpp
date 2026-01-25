#include "BusinessLogic/Factory/PricerFactory.hpp"

std::shared_ptr<Pricer> PricerFactory::createPricer(const std::string& pricerType, const OptionData& opt) {
	// Create payoff function - binds to the option's payoff method
	auto payoff = std::bind(&OptionData::myPayOffFunction, opt, std::placeholders::_1);

	// Create discounter - captures option parameters by value
	auto discounter = [r = opt.r, T = opt.T]() {
		return std::exp(-r * T);
	};

	if (pricerType == "EuropeanPricer") {
		return std::make_shared<EuropeanPricer>(payoff, discounter);
	}
	else if (pricerType == "AsianPricer") {
		return std::make_shared<AsianPricer>(payoff, discounter);
	}
	else if (pricerType == "BarrierPricer") {
		return std::make_shared<BarrierPricer>(payoff, discounter);
	}
	// special case needs separate handling
	//else if (pricerType == "BrownianBridgePricer") {
	//	return std::make_shared<BrownianBridgePricer>(opt);
	//}
	else {
		throw std::invalid_argument("Unknown pricer type: " + pricerType);
	}
}