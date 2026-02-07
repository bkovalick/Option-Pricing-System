#include "BusinessLogic/Mediators/BlackScholesMediator.hpp"

BlackScholesMediator::BlackScholesMediator(const std::shared_ptr<BlackScholes>& blackScholesModel)
	: blackScholesModel_(blackScholesModel)
{
}

void BlackScholesMediator::connectPricers(const std::shared_ptr<Pricer>& pricer, int slot)
{
	(void)pricer;
	(void)slot;
}

void BlackScholesMediator::start()
{
	double price = blackScholesModel_->computePrice();
	std::cout << "Black-Scholes Price: " << price << std::endl;
}

void BlackScholesMediator::startPPL()
{
	start();
}

void BlackScholesMediator::startOpenMP()
{
	start();
}

void BlackScholesMediator::disconnect()
{

}
