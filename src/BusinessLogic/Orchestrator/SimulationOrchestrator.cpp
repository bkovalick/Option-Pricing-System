#include "BusinessLogic/Orchestrator/SimulationOrchestrator.hpp"
#include <BusinessLogic/Factory/MediatorFactory.hpp>
#include <BusinessLogic/Factory/PricerFactory.hpp>
#include <BusinessLogic/Mediators/MonteCarloMediator.hpp>
#include <BusinessLogic/Factory/SdeFactory.hpp>
#include <BusinessLogic/Factory/FdmFactory.hpp>

SimulationOrchestrator::SimulationOrchestrator(const SimulationConfig& config)
	: config_(config)
{
	initializeOptions();
	setupPricers();
	connectPricers();
}


void SimulationOrchestrator::initializeOptions()
{
	options_.clear();
	for (const auto& opt : config_.options) {
		std::cout << "Configuring option: " << opt.OptionName << std::endl;
		options_.push_back(opt);
	}
}

void SimulationOrchestrator::buildMediators()
{
	int nSim = config_.numSimulations;
	int nTimesteps = config_.numTimesteps;
	auto mediatorType = config_.mediatorType;
	if (mediatorType == MediatorType::MonteCarlo)
	{
		for (const auto& sdeType : config_.sdeTypes) {
			for (const auto& fdmType : config_.fdmTypes) {
				for (const auto& rngType : config_.rngTypes) {

					auto sde = SdeFactory::createSde(sdeType, options_[0]);
					auto fdm = FdmFactory::createFdm(fdmType, options_[0], sde, nTimesteps);
					auto rng = std::make_shared<Rng>(rngType);
					auto parts = std::make_tuple(sde, fdm, rng);
					mediator_ = std::make_unique<MonteCarloMediator>(parts, nSim);
				}
			}
		}
	}
}

void SimulationOrchestrator::setupPricers()
{
	for (const auto& option : options_) {
		for (auto& pricerType : config_.pricerTypes) {
			auto pricer = PricerFactory::createPricer(pricerType, option);
			pricers_.push_back(pricer);
		}
	}
}

void SimulationOrchestrator::connectPricers()
{
	if (!mediator_ || !pricers_.empty())
	{
		return;
	}

	int slot = 0;
	for (auto& pricer : pricers_) {
		mediator_->connectPricers(pricer, slot);
		slot++;
	}

}