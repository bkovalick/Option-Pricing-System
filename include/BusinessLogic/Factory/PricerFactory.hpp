#ifndef PricerFactory_hpp
#define PricerFactory_hpp

#include <iostream>
#include <BusinessLogic/Pricers/Pricer.hpp>
#include <BusinessLogic/Pricers/AsianPricer.hpp>
#include <BusinessLogic/Pricers/BarrierPricer.hpp>
#include <BusinessLogic/Pricers/EuropeanPricer.hpp>
#include <BusinessLogic/Pricers/BrownianBridgePricer.hpp>
#include <Core/Domain/OptionData.hpp>


class PricerFactory {
public:
	static std::shared_ptr<Pricer> createPricer(std::string& pricerType, const OptionData& opt);
};
#endif


