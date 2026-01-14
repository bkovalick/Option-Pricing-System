#ifndef Mediator_HPP
#define Mediator_HPP

#include <memory>
#include <string>

class Mediator {
public:
	virtual ~Mediator() = default;

	virtual void processRequest(const std::string& request) = 0;
};

#endif