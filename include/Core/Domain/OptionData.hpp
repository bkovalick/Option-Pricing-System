#ifndef OptionData_HPP
#define OptionData_HPP

#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>

struct OptionData
{
	double K;
	double T;
	double sig;
	double D;
	double r;
	double b;
	double IC; 
	int type;
	std::string OptionName;

	OptionData(std::string optionName, double strike, double expiration, double volatility, 
	           double dividend, int optionType, double interestRate, double initialPrice)
		: OptionName(optionName), K(strike), T(expiration), r(interestRate), sig(volatility),
		  D(dividend), IC(initialPrice), type(optionType)
	{
		std::string put_call = (type == 1) ? "Call" : "Put";
		b = r - D;

		std::cout << "Option Data: " << std::endl;
		std::cout << "OptionName: " << optionName << ", K: " << K << ", T: " << T << ", r: " 
			<< r << ", vol: " << sig << ", div: " << D  << ", beta: " << b << ", IC: " << IC << ", type: " << put_call << std::endl;
		std::cout << std::endl;
	}

	double myPayOffFunction(double S)
	{
		return (type == 1) ? std::max(S - K, 0.0) : std::max(K - S, 0.0);
	}
};

#endif