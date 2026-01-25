#include "BusinessLogic/Factory/PricerFactory.hpp"

std::shared_ptr<Pricer> PricerFactory::createPricer(const std::string& pricerType, const OptionData& opt) {
	auto discounter = [&opt]() { return std::exp(-opt.r * opt.T); };
	if (pricerType == "EuropeanPricer") {
		return std::make_shared<EuropeanPricer>((std::bind(&OptionData::myPayOffFunction, 
			opt, std::placeholders::_1), discounter));
	}
	else if (pricerType == "AsianPricer") {
		return std::make_shared<AsianPricer>((std::bind(&OptionData::myPayOffFunction,
			opt, std::placeholders::_1), discounter));
	}
	else if (pricerType == "BarrierPricer") {
		return std::make_shared<BarrierPricer>((std::bind(&OptionData::myPayOffFunction,
			opt, std::placeholders::_1), discounter));
	}
	// special case needs separate handling
	//else if (pricerType == "BrownianBridgePricer") {
	//	return std::make_shared<BrownianBridgePricer>(opt);
	//}
	else {
		throw std::invalid_argument("Unknown pricer type: " + pricerType);
	}
}