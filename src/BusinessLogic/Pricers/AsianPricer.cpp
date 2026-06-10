// Author: Ben Kovalick
// Purpose: Source file for Asian option class

#include "BusinessLogic/Pricers/AsianPricer.hpp"

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
	if (vec.empty()) return 0.0;
	auto sum = std::accumulate(vec.begin(), vec.end(), 0.0);
	return sum / vec.size();
}

// compute geometric avg.
inline double AsianPricer::Geometric(const std::vector<double>& vec)
{
	if (vec.empty()) return 0.0;
	auto logSum = std::accumulate(vec.begin(), vec.end(), 0.0,
		[](double acc, double val) {
			if (val > 0) {
				return acc + std::log(val);
			}
			return acc;
		});

	return std::exp(logSum / vec.size());
}

// find max
inline double AsianPricer::Max(const std::vector<double>& vec)
{
	if (vec.empty()) return 0.0;
	return *std::max_element(vec.begin(), vec.end());
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
