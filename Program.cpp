// Author: Ben Kovalick
// Purpose: Provides the implementation point for the MC option pricing program.

#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <tuple>
#include <chrono>
#include <fstream>

// Pricer classes
#include "include/BusinessLogic/Pricers/Pricer.hpp"
#include "include/BusinessLogic/Pricers/EuropeanPricer.hpp"
#include "include/BusinessLogic/Pricers/AsianPricer.hpp"
#include "include/BusinessLogic/Pricers/BarrierPricer.hpp"
#include "include/BusinessLogic/Pricers/BrownianBridgePricer.hpp"

// Builder/Mediator Classes
#include "include/BusinessLogic/Builders/MonteCarloBuilder.hpp"
#include "include/BusinessLogic/Mediators/MonteCarloMediator.hpp"
#include "include/BusinessLogic/Factory/MediatorFactory.hpp"

// Option Data Struct
#include "include/Core/Domain/OptionData.hpp"
#include <Core/Configuration/SimulationConfig.hpp>
#include <BusinessLogic/Orchestrator/SimulationOrchestrator.hpp>

// Eventually will use the orchestrator to run the program.
int main()
{
	try {
		// Load configuration from JSON
		std::ifstream configFile("config/option_example.json");
		if (!configFile.is_open()) {
			std::cerr << "Error: Could not open config.json" << std::endl;
			return 1;
		}

		nlohmann::json configJson;
		configFile >> configJson;
		configFile.close();

		// Create configuration from JSON
		auto config = SimulationConfig::fromJson(configJson);

		// Create orchestrator
		SimulationOrchestrator orchestrator(config);

		// Print configuration
		orchestrator.printConfiguration();

		// Run simulation
		orchestrator.run();
		
		// Print results
		orchestrator.printResults();
		
		// Export to CSV
		orchestrator.exportToCSV("results.csv");
	}
	catch (const std::exception& ex) {
		std::cerr << "Error: " << ex.what() << std::endl;
		return 1;
	}

	system("pause");
	return 0;
}


//int main()
//{
//	// Load configuration from JSON file
//	std::ifstream configFile("config.json");
//	if (!configFile.is_open())
//	{
//		std::cerr << "Error: Could not open config.json" << std::endl;
//		return 1;
//	}
//
//	nlohmann::json configJson;
//	configFile >> configJson;
//	configFile.close();
//
//
//	std::vector<OptionData> options;
//	std::vector<std::shared_ptr<Pricer>> pricers;
//	std::vector<std::shared_ptr<Sde>> sdes;
//	std::vector<std::shared_ptr<FdmBase>> fdms;
//	std::vector<std::shared_ptr<Rng>> rngs;
//
//	// Create configuration from JSON
//	auto config = SimulationConfig::fromJson(configJson);
//	auto mediator = MediatorFactory::createMediator(config.mediatorType);
//	//mediator->configure(config);
//	//
//	//// Is this what the mediator class will do?
//	//for (const auto& opt: config.options) {
//	//	std::cout << "Configuring option: " << opt.OptionName << std::endl;
//	//	options.push_back(opt);
//	//}
//
//	//for (const auto& pricer : config.pricerTypes) {
//	//	std::cout << "Configuring option: " << pricer << std::endl;
//	//	auto tempPricer = std::make_shared<Pricer>(pricer);
//	//	pricers.push_back(std::move(tempPricer));
//	//	//pricers.push_back(std::make_shared<pricer>);
//	//}
//
//
//	//for (const auto& mediator : request.mediatorType) {
//	//	auto mediator = MediatorFactory::createMediator("MonteCarlo");
//	//	mediator->processRequest("start");
//	//}
//	
//
//
//	
//	// 1. Initialise the option data
//	// Batch 1
//	// Constructor: (strike, expiration, volatility, dividend, optionType, interestRate, beta, initialPrice)
//	OptionData myOption("Batch1_Call", 65.0, 0.25, 0.30, 0.0, 1, 0.08, 1.0, 60.0);
//
//	//// Put
//	OptionData myOption2("Batch1_Put", 65.0, 0.25, 0.30, 0.0, -1, 0.08, 1.0, 60.0);
//
//	// Batch 2
//	//OptionData myOption("Batch2_Call", 100.0, 1.0, 0.20, 0.0, 1, 0.0, 1.0, 100.0);
//
//	// Put
//	//OptionData myOption2("Batch2_Put", 100.0, 1.0, 0.20, 0.0, -1, 0.0, 1.0, 100.0);
//
//	// Batch 3
//	//OptionData myOption("Batch3_Call", 10.0, 1.0, 0.50, 0.0, 1, 0.12, 1.0, 5.0);
//
//	// Put
//	//OptionData myOption2("Batch3_Put", 10.0, 1.0, 0.50, 0.0, -1, 0.12, 1.0, 5.0);
//
//	// Batch 4
//	//OptionData myOption("Batch4_Call", 100.0, 30.0, 0.30, 0.0, 1, 0.08, 1.0, 100.0);
//
//	//// Put
//	//OptionData myOption2("Batch4_Put", 100.0, 30.0, 0.30, 0.0, -1, 0.08, 1.0, 100.0);
//
//	// 2. Choose the builder that creates the necessary components
//	int choice = 1;
//	auto parts = MonteCarloBuilder::ChooseBuilder(choice, myOption);
//
//	// 3. Create the mediator (SUD)
//	int NSim = 1000000;
//	std::cout << "How many simulations? "; std::cin >> NSim;
//	MonteCarloMediator mcp(parts, NSim);
//
//	// 4. Create the payoff functions -> Can use this payoff function or the one inherent in the OptionData struct
//	auto payoff = [&myOption](double x) {
//		if (myOption.type == 1)
//		{
//			return std::max(0.0, x - myOption.K);
//		}
//		else
//		{
//			return std::max(0.0, myOption.K - x);
//		}
//	};
//
//	auto discounter = [&myOption]() {return std::exp(-myOption.r * myOption.T); };
//
//	// 5. Create prices and link them to the mediator.
//	std::shared_ptr<Pricer> op = 
//		std::make_shared<EuropeanPricer>(std::bind(&OptionData::myPayOffFunction, myOption, std::placeholders::_1), discounter);
//	std::shared_ptr<Pricer> op2 = 
//		std::make_shared<EuropeanPricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), discounter);
//	std::shared_ptr<Pricer> op3 = 
//		std::make_shared<AsianPricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), discounter);
//	std::shared_ptr<Pricer> op4 = 
//		std::make_shared<BarrierPricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), discounter);
//	
//	//// Need SDE process from the builder for the Brownian bridge pricer
//	//auto sdeProc = std::get<0>(parts);
//
//	//// Need FDM process as well to get the step size.
//	//auto fdmProc = std::get<1>(parts);
//	//double dt = fdmProc->k;
//
//	//// Need RNG process as well to get a random number.
//	//auto rngProc = std::get<2>(parts);
//	//double gen = rngProc->gen();
//
//	//// Brownian Bridge Pricer
//	//std::shared_ptr<Pricer> op5 = 
//	//	std::make_shared<BrownianBridgePricer>(std::bind(&OptionData::myPayOffFunction, myOption2, std::placeholders::_1), 
//	//		discounter, sdeProc, gen, dt);
//
//	// Define slots for path information.
//	mcp.path.connect(0, std::bind(&Pricer::ProcessPath, op, std::placeholders::_1));
//	mcp.path.connect(1, std::bind(&Pricer::ProcessPath, op2, std::placeholders::_1));
//	mcp.path.connect(2, std::bind(&Pricer::ProcessPath, op3, std::placeholders::_1));
//	mcp.path.connect(3, std::bind(&Pricer::ProcessPath, op4, std::placeholders::_1));
//	//mcp.path.connect(4, std::bind(&Pricer::ProcessPath, op5, std::placeholders::_1));
//
//	// Signal end of simulation.
//	mcp.finish.connect(0, std::bind(&Pricer::PostProcess, op));
//	mcp.finish.connect(1, std::bind(&Pricer::PostProcess, op2));
//	mcp.finish.connect(2, std::bind(&Pricer::PostProcess, op3));
//	mcp.finish.connect(3, std::bind(&Pricer::PostProcess, op4));
//	//mcp.finish.connect(4, std::bind(&Pricer::PostProcess, op5));
//
//	// 6. Run the program and examine the output.
//	auto start = std::chrono::system_clock::now();
//	mcp.start(); // Uncomment to run single-threaded
//	//mcp.startPPL(); // Uncomment to run multi-threaded using PPL.
//	//mcp.startOpenMP(); // Uncomment to run multi-threaded using OpenMP.
//	auto end = std::chrono::system_clock::now();
//	std::chrono::duration<double> elapsed_seconds = end - start;
//	std::cout << "Total Computation time: " << elapsed_seconds.count() << std::endl;
//
//	system("pause");
//	return 0;
//}