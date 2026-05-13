#ifndef BlackScholesEngine_HPP
#define BlackScholesEngine_HPP

#include <iostream>
#include <memory>
#include <BusinessLogic/Pricers/Pricer.hpp>
#include "BusinessLogic/Engines/PricingEngine.hpp"
#include "BusinessLogic/Models/BlackScholes/BlackScholes.hpp"

class BlackScholesEngine : public PricingEngine
{
private:
	std::shared_ptr<BlackScholes> blackScholesModel_;

public:
	BlackScholesEngine(const std::shared_ptr<BlackScholes>& blackScholesModel);
	virtual ~BlackScholesEngine() = default;
	
	double computePrice() override;
};

#endif
