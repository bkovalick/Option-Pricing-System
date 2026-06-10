// Author: Ben Kovalick
// Purpose: Header file for the Asian option class.

#ifndef AsianPricer_HPP
#define AsianPricer_HPP

#include "Pricer.hpp"

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>

class AsianPricer : public Pricer
{
private:
	double price;
	double sum;
	int NSim;
	double avg = 0.0;
	double max = 0.0;

public:
	AsianPricer(const Payoff& payoff, const Discount& discounter);
	virtual ~AsianPricer();

	double Average(const std::vector<double>& vec);
	double Geometric(const std::vector<double>& vec);
	double Max(const std::vector<double>& vec);

	void ProcessPath(const std::vector<double>& t);
	double DiscountFactor();
	void PostProcess();
	double Price();
};

#endif
