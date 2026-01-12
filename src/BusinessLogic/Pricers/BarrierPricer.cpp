// Author: Ben Kovalick
// Purpose: Source file for Barrier option class.

#include "BarrierPricer.hpp"

// Argument Constructor
BarrierPricer::BarrierPricer(const Payoff& payoff, const Discount& discounter)
	: Pricer(payoff, discounter)
{
	// Stock must fall below this amount to be activated.
	std::cout << "Please select a barrier amount: "; std::cin >> barrier;

	price = sum = sum2 = 0.0;
	NSim = 0;
}

// Destructor
BarrierPricer::~BarrierPricer()
{}

// A path for each simulation draw.
inline void BarrierPricer::ProcessPath(const std::vector<double>& t)
{
	// Amount returned to the purchaser
	double rebate = 0.0;

	bool crossed = false;
	for (const auto& elem : t)
	{
		if (elem >= barrier)
		{ // Down and out
			crossed = true;
			break;
		}
	}

	if (crossed == false)
	{
		sum += m_payoff(t[t.size() - 1]);
	}
	else
	{
		sum += rebate;
	}
	
	NSim++;
}

// Discounting
inline double BarrierPricer::DiscountFactor()
{
	return m_discounter();
}

// Responsible for post simulation calculations
inline void BarrierPricer::PostProcess()
{
	std::cout << "Computing Barrier Option Price: " << std::endl;
	price = DiscountFactor() * sum / NSim;
	std::cout << "Price: {0}, {1} " << price << ", " << NSim << std::endl;
}

// Returns the option price
inline double BarrierPricer::Price()
{
	return price;
}
