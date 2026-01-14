
#include "BusinessLogic/Factory/MediatorFactory.hpp"
#include "BusinessLogic/Mediators/MonteCarloMediator.hpp"
#include <memory>

std::unique_ptr<Mediator> MediatorFactory::createMediator(const std::string& mediatorType) {
	if (mediatorType == "MonteCarlo") {
		return std::make_unique<MonteCarloMediator>(); // all mediators should be default and have a calc or start method that accepts a json request.
	}  
	else {
		throw std::invalid_argument("Unknown pricing engine type: " + mediatorType);
	}

	throw std::runtime_error("Unknown model name");
}