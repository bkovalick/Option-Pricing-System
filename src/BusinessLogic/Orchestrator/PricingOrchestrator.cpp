#include "BusinessLogic/Orchestrator/PricingOrchestrator.hpp"
#include <BusinessLogic/Factory/MediatorFactory.hpp>
#include <BusinessLogic/Factory/PricerFactory.hpp>

PricingOrchestrator::PricingOrchestrator(const SimulationConfig& config)
	: config_(config)
{
	config_ = config;
	initializeComponents();
	setupPricers();
	connectPricers();
}


void PricingOrchestrator::initializeComponents()
{
	for (const auto& opt : config_.options) {
		std::cout << "Configuring option: " << opt.OptionName << std::endl;
		options_.push_back(opt);
	}
}

void PricingOrchestrator::setupMediator()
{
    // Pass by value to avoid binding to non-const reference
    auto mediator = MediatorFactory::createMediator(config_.mediatorType);
    //mediator_->configure(config_);
}

void PricingOrchestrator::setupPricers() 
{
	for (const auto& option : config_.options) {
		for (const auto& pricerType : config_.pricerTypes) {
			auto pricer = PricerFactory::createPricer(pricerType, option);
			pricers_.push_back(pricer);
		}
	}
}

void PricingOrchestrator::connectPricers()
{

}