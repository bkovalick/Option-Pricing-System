#ifndef MediatorFactory_HPP
#define MediatorFactory_HPP

#include <iostream>
#include <string>
#include "BusinessLogic/Mediators/Mediator.hpp"

class MediatorFactory {
public:
	static std::unique_ptr<Mediator> createMediator(const std::string& mediatorType);
};

#endif