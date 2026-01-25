#ifndef Mediator_HPP
#define Mediator_HPP

#include <memory>

class Mediator {
public:
	virtual ~Mediator() = default;
	virtual void connectPricers(std::shared_ptr<Pricer>& pricer, int slot) = 0;
};

#endif