// Author: Ben Kovalick
// Purpose: Header file for the abstract Pricer class.

#ifndef Pricer_HPP
#define Pricer_HPP

#include <iostream>
#include <functional>
#include <vector>

class Mediator;

// Function wrappers for the option payoff and discounting functions.
using Payoff = std::function<double(double)>;
using Discount = std::function<double()>;

class Pricer
{
protected:
	// Discount and Payoff method need to be used in derived classes.
	Discount m_discounter;
	Payoff m_payoff;

public:
	Pricer(const Payoff& payoff, const Discount& discounter) 
		: m_payoff(payoff), m_discounter(discounter)
	{}
	
	virtual void ProcessPath(const std::vector<double>& t) = 0; // Create a single path
	virtual void PostProcess() = 0; // Notify end of simulation
	virtual double DiscountFactor() = 0; // Discounting
	virtual double Price() = 0; // Option Price
};

#endif // !Pricer_HPP
