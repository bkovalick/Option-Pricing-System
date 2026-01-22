#ifndef MediatorFactory_HPP
#define MediatorFactory_HPP

#include <iostream>
#include <string>
#include "BusinessLogic/Mediators/Mediator.hpp"
#include "Core/Configuration/SimulationConfig.hpp"

// Add this function declaration to convert MediatorType to string
std::string toString(const MediatorType& mediatorType);

class MediatorFactory {
public:
	static std::unique_ptr<Mediator> createMediator(MediatorType& mediatorType);
};

#endif

