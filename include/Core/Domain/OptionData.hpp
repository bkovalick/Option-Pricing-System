// Author: Ben Kovalick
// Purpose: Basic Struct used to house all data related to an Option.

#ifndef OptionData_HPP
#define OptionData_HPP

#include <algorithm> // for max()
#include <boost/parameter.hpp>

namespace OptionParams
{
	BOOST_PARAMETER_KEYWORD(Tag, strike)
		BOOST_PARAMETER_KEYWORD(Tag, expiration)
		BOOST_PARAMETER_KEYWORD(Tag, interestRate)
		BOOST_PARAMETER_KEYWORD(Tag, volatility)
		BOOST_PARAMETER_KEYWORD(Tag, dividend)
		BOOST_PARAMETER_KEYWORD(Tag, beta)
		BOOST_PARAMETER_KEYWORD(Tag, initialPrice)
		BOOST_PARAMETER_KEYWORD(Tag, optionType)
}


// Encapsulate all data in one place
struct OptionData
{ // Option data + behaviour

	double K;
	double T;
	double r;
	double sig;
	double b;
	double IC;

	// Extra data 
	double D;		// dividend

	int type;		// 1 == call, -1 == put

	explicit constexpr OptionData(double strike, double expiration, double interestRate,
		double volatility, double dividend, double beta, double initialPrice, int PC)
		: K(strike), T(expiration), r(interestRate), sig(volatility), D(dividend), b(beta), IC(initialPrice), type(PC)
	{}

	template <typename ArgPack> OptionData(const ArgPack& args)
	{
		K = args[OptionParams::strike];
		T = args[OptionParams::expiration];
		r = args[OptionParams::interestRate];
		sig = args[OptionParams::volatility];
		D = args[OptionParams::dividend];
		b = args[OptionParams::beta];
		IC = args[OptionParams::initialPrice];
		type = args[OptionParams::optionType];

		std::string put_call;

		if (type == 1)
			put_call = "Call";
		else
			put_call = "Put";

		std::cout << "Option Data: " << std::endl;
		std::cout << "K: " << K << ", T: " << T << ", r: " << r 
			 << ", vol: " << sig << ", div: " << D 
			 << ", beta: " << b << ", IC: " << IC << ", type: " << put_call << std::endl;
		std::cout << std::endl;
	}

	double myPayOffFunction(double S)
	{ // Payoff function

		if (type == 1)
		{ // Call

			return std::max(S - K, 0.0);
		}
		else
		{ // Put

			return std::max(K - S, 0.0);
		}
	}
};


#endif