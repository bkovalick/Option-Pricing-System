#ifndef Mediator_HPP
#define Mediator_HPP

#include <memory>
#include <string>

class Mediator {
public:
	virtual ~Mediator() = default;
	virtual void configure(const SimulationConfig& request) = 0;
};

#endif