// Author: Ben Kovalick
// Purpose: Header file for European option class

#ifndef EuropeanPricer_HPP
#define EuropeanPricer_HPP

#include "Pricer.hpp"
#include <vector>

class EuropeanPricer : public Pricer
{
private:
	double price;
	double sum;
	int NSim;

public:
	EuropeanPricer(const Payoff& payoff, const Discount& discounter); // Argument Constructor
	virtual ~EuropeanPricer(); // Destructor
	
	void ProcessPath(const std::vector<double>& t); // A path for each simulation draw.
	double DiscountFactor(); // Discounting
	void PostProcess(); // Notify end of simulation
	double Price(); // Option Price
};

#endif // !EuropeanPricer_HPP
