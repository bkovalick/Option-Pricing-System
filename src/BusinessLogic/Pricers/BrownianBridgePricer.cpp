// Author: Ben Kovalick
// Purpose: Source file for the Brownian Bridge option class.

#include "BrownianBridgePricer.hpp"

// Argument Constructor
BrownianBridgePricer::BrownianBridgePricer(const Payoff& payoff, const Discount& discounter,
	std::shared_ptr<Sde>& sde, double rand, double step)
	: Pricer(payoff, discounter), m_sde(sde), m_rand(rand), dt(step)
{
	// Stock must fall below this amount to be activated.
	std::cout << "Please select a barrier amount: "; std::cin >> barrier;

	price = sum = sum2 = 0.0;
	NSim = 0;
}

// Destructor
BrownianBridgePricer::~BrownianBridgePricer()
{}

// A path for each simulation draw.
inline void BrownianBridgePricer::ProcessPath(const std::vector<double>& t)
{
	// Amount returned to the purchaser
	double rebate = 0.0;

	double P, tmp;

	bool crossed = false;
	for (unsigned int i = 1; i < t.size(); i++)
	{
		// Calculate exit probability
		tmp = m_sde->Diffusion(t[i - 1], (i - 1) * dt);
		P = std::exp(-2.0 * (barrier - t[i - 1]) * (barrier - t[i]) / (tmp * tmp * dt));
		
		if (P >= m_rand)
		{ 
			counter++;
		}
		if (t[i] >= barrier || P >= m_rand)
		{
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
inline double BrownianBridgePricer::DiscountFactor()
{
	return m_discounter();
}

// Responsible for post simulation calculations
inline void BrownianBridgePricer::PostProcess()
{
	std::cout << "Computing Brownian Bridge Option Price: " << std::endl;
	price = DiscountFactor() * sum / NSim;
	std::cout << "Price: {0}, {1} " << price << ", " << NSim << std::endl;
}

// Returns the option price
inline double BrownianBridgePricer::Price()
{
	return price;
}
