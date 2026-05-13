#define _SCL_SECURE_NO_WARNINGS

#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <tuple>
#include <chrono>
#include <fstream>

#include "include/BusinessLogic/Pricers/Pricer.hpp"
#include "include/BusinessLogic/Pricers/EuropeanPricer.hpp"
#include "include/BusinessLogic/Pricers/AsianPricer.hpp"
#include "include/BusinessLogic/Pricers/BarrierPricer.hpp"
#include "include/BusinessLogic/Pricers/BrownianBridgePricer.hpp"

#include "include/BusinessLogic/Builders/MonteCarloBuilder.hpp"
#include "include/BusinessLogic/Mediators/MonteCarloMediator.hpp"

#include "include/Core/Domain/OptionData.hpp"
#include "Core/Configuration/SimulationConfig.hpp"
#include "BusinessLogic/Orchestrator/SimulationOrchestrator.hpp"
#include "BusinessLogic/Orchestrator/EngineOrchestrator.hpp"

int main()
{
	try {
		// Load configuration from JSON
		/*std::ifstream configFile("config/monte_carlo_mediator.json");*/
		/*std::ifstream configFile("config/black_scholes_mediator.json");*/
		std::ifstream configFile("config/black_scholes_engine.json");
		//std::ifstream configFile("config/monte_carlo_engine.json");
		if (!configFile.is_open()) {
			std::cerr << "Error: Could not open config.json" << std::endl;
			return 1;
		}

		nlohmann::json configJson;
		configFile >> configJson;
		configFile.close();

		// Create configuration from JSON
		/*auto config = SimulationConfig::fromJson(configJson);*/
		auto config = SimulationConfig::fromJsonEngine(configJson);

		// Create orchestrator
		//SimulationOrchestrator orchestrator(config);
		EngineOrchestrator orchestrator(config);

		// Run simulation
		orchestrator.run();
		
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