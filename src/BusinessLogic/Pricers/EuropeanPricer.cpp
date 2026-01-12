// Author: Ben Kovalick
// Purpose: Source file for European option class

#include "EuropeanPricer.hpp"

// Argument Constructor
EuropeanPricer::EuropeanPricer(const Payoff& payoff, const Discount& discounter) 
	: Pricer(payoff, discounter)
{
	price = sum = 0.0;
	NSim = 0;
}

// Destructor
EuropeanPricer::~EuropeanPricer()
{}

// A path for each simulation draw.
inline void EuropeanPricer::ProcessPath(const std::vector<double>& t)
{
	sum += m_payoff(t[t.size() - 1]);
	NSim++;
}

// Discounting
inline double EuropeanPricer::DiscountFactor()
{
	return m_discounter();
}

// Responsible for post simulation calculations
inline void EuropeanPricer::PostProcess()
{
	std::cout << "Computing European Option Price: " << std::endl;
	price = DiscountFactor() * sum / NSim;
	std::cout << "Price: {0}, {1} " << price << ", " << NSim << std::endl;
}

// Returns the option price
inline double EuropeanPricer::Price()
{
	return price;
}
