// Author: Ben Kovalick
// Purpose: Source file for Asian option class

#include "AsianPricer.hpp"

// Argument Constructor
AsianPricer::AsianPricer(const Payoff& payoff, const Discount& discounter)
	: Pricer(payoff, discounter)
{
	price = sum = 0.0;
	NSim = 0;
}

// Destructor
AsianPricer::~AsianPricer()
{}

// compute arithemtic avg.
inline double AsianPricer::Average(const std::vector<double>& vec)
{
	double avg = vec[0];

	for (const auto& elem : vec)
		avg += elem;

	return avg / vec.size();
}

// compute geometric avg.
inline double AsianPricer::Geometric(const std::vector<double>& vec)
{
	double avg = vec[0];

	for (const auto& elem : vec)
		avg *= elem;

	return (1.0 / std::pow(avg, vec.size()));
}

// find max
inline double AsianPricer::Max(const std::vector<double>& vec)
{
	double max = vec[0];

	for (const auto& elem : vec)
		max = std::max(elem, max);

	return max;
}

// A path for each simulation draw.
inline void AsianPricer::ProcessPath(const std::vector<double>& t)
{
	double avg = Average(t);

	sum += m_payoff(avg);
	NSim++;
}

// Discounting
inline double AsianPricer::DiscountFactor()
{
	return m_discounter();
}

// Responsible for post simulation calculations
inline void AsianPricer::PostProcess()
{
	std::cout << "Computing Asian Option Price: " << std::endl;
	price = DiscountFactor() * sum / NSim;
	std::cout << "Price: {0}, {1} " << price << ", " << NSim << std::endl;
}

// Returns the option price
inline double AsianPricer::Price()
{
	return price;
}
