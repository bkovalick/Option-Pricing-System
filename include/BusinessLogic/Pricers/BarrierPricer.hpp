// Author: Ben Kovalick
// Purpose: Header file for the Barrier option class

#ifndef BarrierPricer_HPP
#define BarrierPricer_HPP

#include "Pricer.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

class BarrierPricer : public Pricer
{
private:
	double price;
	double sum, sum2;
	double barrier = 170.0; // barrier that knocks the price in or out
	int NSim;

public:
	BarrierPricer(const Payoff& payoff, const Discount& discounter); // Argument Constructor
	virtual ~BarrierPricer(); // Destructor

	void ProcessPath(const std::vector<double>& t); // A path for each simulation draw.
	double DiscountFactor(); // Discounting
	void PostProcess(); // Notify end of simulation
	double Price(); // Option Price
};

#endif // !BarrierPricer_HPP
