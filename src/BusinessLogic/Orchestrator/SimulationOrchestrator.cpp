#include "BusinessLogic/Orchestrator/SimulationOrchestrator.hpp"
#include <BusinessLogic/Factory/MediatorFactory.hpp>
#include <BusinessLogic/Factory/PricerFactory.hpp>
#include <BusinessLogic/Mediators/MonteCarloMediator.hpp>
#include <BusinessLogic/Factory/SdeFactory.hpp>
#include <BusinessLogic/Factory/FdmFactory.hpp>
#include <BusinessLogic/Factory/RngFactory.hpp>

SimulationOrchestrator::SimulationOrchestrator(const SimulationConfig& config)
	: config_(config)
{
	
	initializeOptions();
	buildMediators();
	//setupPricers();
	//connectPricers();
}


void SimulationOrchestrator::initializeOptions()
{
	std::cout << "=== Initializing Simulation Orchestrator ===" << std::endl;
	std::cout << "Mediator Type: " << static_cast<int>(config_.mediatorType) << std::endl;
	std::cout << "Execution Mode: " << static_cast<int>(config_.executionMode) << std::endl;
	std::cout << "Number of Simulations per Mediator: " << config_.numSimulations << std::endl;
	size_t numOptions = config_.options.size();
	options_.clear();
	options_.reserve(numOptions);
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
	int instanceId = 0;
	auto numSimulationInst = options_.size() * config_.pricerTypes.size() * 
		config_.sdeTypes.size() * config_.fdmTypes.size() * config_.rngTypes.size();
	simulationInstances_.clear();
	simulationInstances_.reserve(numSimulationInst);
	if (mediatorType == MediatorType::MonteCarlo)
	{
		for (const auto& option : options_) {
			for (const auto& sdeType : config_.sdeTypes) {
				for (const auto& fdmType : config_.fdmTypes) {
					for (const auto& rngType : config_.rngTypes) {
						for (auto& pricerType : config_.pricerTypes) {
							SimulationInstance simulationInstance = SimulationInstance(instanceId, sdeType, fdmType, rngType);
							auto pricer = PricerFactory::createPricer(pricerType, option);
							auto sde = SdeFactory::createSde(sdeType, option);
							auto fdm = FdmFactory::createFdm(fdmType, option, sde, nTimesteps);
							auto rng = RngFactory::createRng(rngType);
							auto parts = std::make_tuple(sde, fdm, rng);
							mediator_ = std::make_unique<MonteCarloMediator>(parts, nSim);
							instanceId++;
							simulationInstances_.push_back(std::move(simulationInstance));
						}
					}
				}
			}
		}
	}
}

void SimulationOrchestrator::run()
{
	for (const auto& simulation : simulationInstances_) {
		std::cout << "\n--- Running Simulation Instance " << simulation.instanceId_ << " ---" << std::endl;
		std::cout << "Configuration: SDE=" << simulation.sde_
			<< ", FDM=" << simulation.fdm_
			<< ", RNG=" << simulation.rng_ << std::endl;

		auto start = std::chrono::high_resolution_clock::now();

		switch (config_.executionMode) {
		case ExecutionMode::SingleThreaded:

			simulation.mediator_->start();
			break;
		case ExecutionMode::OpenMP:
			simulation.mediator_->startOpenMP();
			break;
		case ExecutionMode::PPL:
			simulation.mediator_->startPPL();
			break;
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;

		std::cout << "Instance " << simulation.instanceId_ << " completed in "
			<< elapsed.count() << " seconds" << std::endl;
	}
}

void SimulationOrchestrator::runWithMode(ExecutionMode mode)
{
	for (const auto& simulation : simulationInstances_) {
		std::cout << "\n--- Running Simulation Instance " << simulation.instanceId_ << " ---" << std::endl;
		std::cout << "Configuration: SDE=" << simulation.sde_
			<< ", FDM=" << simulation.fdm_
			<< ", RNG=" << simulation.rng_ << std::endl;

		auto start = std::chrono::high_resolution_clock::now();
		switch (mode) {
		case ExecutionMode::SingleThreaded:
			simulation.mediator_->start();
			break;
		case ExecutionMode::OpenMP:
			simulation.mediator_->startOpenMP();
			break;
		case ExecutionMode::PPL:
			simulation.mediator_->startPPL();
			break;
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = end - start;

		std::cout << "Instance " << simulation.instanceId_ << " completed in "
			<< elapsed.count() << " seconds" << std::endl;
	}
}

void SimulationOrchestrator::printConfiguration() const
{
	std::cout << "\n=== Orchestrator Configuration ===" << std::endl;
	std::cout << "Number of Simulation Instances: " << simulationInstances_.size() << std::endl;
	std::cout << "Number of Options: " << options_.size() << std::endl;
	std::cout << "Simulations per Mediator: " << config_.numSimulations << std::endl;
}
