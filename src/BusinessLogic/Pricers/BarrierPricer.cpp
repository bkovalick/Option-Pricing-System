// Author: Ben Kovalick
// Purpose: Source file for Barrier option class.

#include "BusinessLogic/Pricers/BarrierPricer.hpp"

// Default Constructor - uses default barrier
BarrierPricer::BarrierPricer(const Payoff& payoff, const Discount& discounter)
	: Pricer(payoff, discounter), barrier(80.0), barrierType(BarrierType::DownAndOut), rebate(0.0)
{
	price = sum = 0.0;
	NSim = 0;
}

// Parameterized Constructor - specify barrier
BarrierPricer::BarrierPricer(const Payoff& payoff, const Discount& discounter,
                             double barrierLevel, BarrierType type, double rebateAmount)
	: Pricer(payoff, discounter), barrier(barrierLevel), barrierType(type), rebate(rebateAmount)
{
	price = sum = 0.0;
	NSim = 0;
}

// Destructor
BarrierPricer::~BarrierPricer()
{}

// A path for each simulation draw.
void BarrierPricer::ProcessPath(const std::vector<double>& t)
{
	bool knocked_out = false;

	// Check barrier condition based on type
	switch (barrierType) {
		case BarrierType::DownAndOut:
			// Knocked out if price falls BELOW barrier
			for (const auto& price : t) {
				if (price <= barrier) {
					knocked_out = true;
					break;
				}
			}
			break;

		case BarrierType::UpAndOut:
			// Knocked out if price rises ABOVE barrier
			for (const auto& price : t) {
				if (price >= barrier) {
					knocked_out = true;
					break;
				}
			}
			break;
	}

	if (!knocked_out) {
		sum += m_payoff(t.back());  // Normal payoff
	}
	else {
		sum += rebate;  // Knocked out → receive rebate
	}

	NSim++;
}

// Discounting
double BarrierPricer::DiscountFactor()
{
	return m_discounter();
}

// Responsible for post simulation calculations
void BarrierPricer::PostProcess()
{
	std::cout << "Computing Barrier Option Price:" << std::endl;
	std::cout << "  Barrier Level: " << barrier << std::endl;
	std::cout << "  Type: " << (barrierType == BarrierType::DownAndOut ? "Down-and-Out" : "Up-and-Out") << std::endl;

	price = DiscountFactor() * sum / NSim;
	std::cout << "  Price: " << price << " (from " << NSim << " simulations)" << std::endl;
}

// Returns the option price
double BarrierPricer::Price()
{
	return price;
}
