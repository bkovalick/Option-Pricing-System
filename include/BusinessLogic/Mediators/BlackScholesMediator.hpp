#ifndef BlackScholesMediator_HPP
#define BlackScholesMediator_HPP

#include <iostream>
#include <memory>
#include <BusinessLogic/Pricers/Pricer.hpp>
#include "BusinessLogic/Mediators/Mediator.hpp"
#include "BusinessLogic/Models/BlackScholes/BlackScholes.hpp"

class BlackScholesMediator : public Mediator
{
private:
	std::shared_ptr<BlackScholes> blackScholesModel_;

public:
	BlackScholesMediator(const std::shared_ptr<BlackScholes>& blackScholesModel);
	virtual ~BlackScholesMediator();

	void connectPricers(const std::shared_ptr<Pricer>& pricer, int slot) override;
	void start() override;
	void startPPL() override;
	void startOpenMP() override;
	void disconnect();
};

#endif
