// Author: Ben Kovalick
// Purpose: Header file for the Asian option class.

#ifndef AsianPricer_HPP
#define AsianPricer_HPP

#include "Pricer.hpp"

#include <vector>
#include <iostream>
#include <algorithm>

class AsianPricer : public Pricer
{
private:
	double price;
	double sum;
	int NSim;
	double avg = 0.0;  // Arithmetic average at the fixing points
	double max = 0.0;

public:
	AsianPricer(const Payoff& payoff, const Discount& discounter); // Argument Constructor
	virtual ~AsianPricer(); // Destructor

	double Average(const std::vector<double>& vec); // compute arithemtic avg.
	double Geometric(const std::vector<double>& vec); // compute geometric avg.
	double Max(const std::vector<double>& vec); // find max

	void ProcessPath(const std::vector<double>& t); // A path for each simulation draw.
	double DiscountFactor(); // Discounting
	void PostProcess(); // Notify end of simulation
	double Price(); // Option Price
};

#endif // !AsianPricer_HPP
