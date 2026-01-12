// Author: Ben Kovalick
// Purpose: Header file for the Brownian Bridge option class.

#ifndef BrownianBridgePricer_HPP
#define BrownianBridgePricer_HPP

#include "Pricer.hpp"
#include "BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

class BrownianBridgePricer : public Pricer
{
private:
	double price;
	double sum, sum2;
	double barrier = 170.0; // barrier that knocks the price in or out
	int NSim;
	double dt;
	double m_rand;
	std::shared_ptr<Sde> m_sde;
	int counter = 0;

public:
	BrownianBridgePricer(const Payoff& payoff, const Discount& discounter,
		std::shared_ptr<Sde>& sde, double rand, double step); // Argument Constructor
	virtual ~BrownianBridgePricer(); // Destructor

	void ProcessPath(const std::vector<double>& t); // A path for each simulation draw.
	double DiscountFactor(); // Discounting
	void PostProcess(); // Notify end of simulation
	double Price(); // Option Price
};

#endif // !BrownianBridgePricer_HPP
