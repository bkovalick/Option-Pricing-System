#ifndef Mediator_HPP
#define Mediator_HPP

#include <memory>

class Pricer;

class Mediator {
public:
	virtual ~Mediator() = default;
	virtual void connectPricers(const std::shared_ptr<Pricer>& pricer, int slot) = 0;
	virtual void start() = 0;
	virtual void startPPL() = 0;
	virtual void startOpenMP() = 0;
};

#endif