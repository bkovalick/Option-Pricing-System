#include "BusinessLogic/Factory/MediatorFactory.hpp"
#include "BusinessLogic/Mediators/MonteCarloMediator.hpp"
#include <memory>
#include <stdexcept>
#include <string>

std::string toString(const MediatorType& mediatorType) {
	switch (mediatorType) {
	case MediatorType::MonteCarlo:
		return "MonteCarlo";
		// Add other cases as needed
	default:
		return "Unknown";
	}
}

std::unique_ptr<Mediator> MediatorFactory::createMediator(MediatorType& mediatorType) {
	if (mediatorType == MediatorType::MonteCarlo) {
		return std::make_unique<MonteCarloMediator>();
	}
	else {
		throw std::invalid_argument("Unknown mediator type: " + toString(mediatorType));
	}

	throw std::runtime_error("Unknown mediator type");
}