#include "BusinessLogic/Engines/BlackScholesEngine.hpp"

BlackScholesEngine::BlackScholesEngine(const std::shared_ptr<BlackScholes>& blackScholesModel)
	: PricingEngine("BlackScholes"), blackScholesModel_(blackScholesModel)
{
    if (!blackScholesModel_) {
        throw std::invalid_argument("BlackScholesEngine: null model");
    }
}

double BlackScholesEngine::computePrice()
{
	return blackScholesModel_->computePrice();
}