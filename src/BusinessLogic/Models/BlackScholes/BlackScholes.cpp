#include "BusinessLogic/Models/BlackScholes/BlackScholes.hpp"

namespace {
	constexpr double INV_SQRT_2PI = 0.3989422804014327;  // 1/sqrt(2*pi)
	constexpr double ONE_OVER_SQRT_2 = 0.7071067811865475; // 1/sqrt(2)
}

BlackScholes::BlackScholes(const OptionData& optData) 
	: optionData_(optData), sqrtT_(std::sqrt(optData.T))
{
	if (optData.IC <= 0.0 || optData.K <= 0.0 ||
		optData.T <= 0.0 || optData.sig <= 0.0)
	{
		throw std::invalid_argument("Invalid option parameters");
	}
}

double BlackScholes::computeNormalCDF(double x)
{
	// Abramowitz and Stegun approximation
	if (x < -10.0) return 0.0;
	if (x > 10.0) return 1.0;
	return 0.5 * (1.0 + std::erf(x * ONE_OVER_SQRT_2));
}

double BlackScholes::computePrice() const
{
	const double d1 = computeD1();
	const double d2 = computeD2(d1);
	if (optionData_.type == 1)
	{
		const double term1 = optionData_.IC * computeNormalCDF(d1);
		const double term2 = optionData_.K * std::exp(-optionData_.r * optionData_.T) * computeNormalCDF(d2);
		return term1 - term2;
	}
	else
	{
		const double term1 = optionData_.K * std::exp(-optionData_.r * optionData_.T) * computeNormalCDF(-d2);
		const double term2 = optionData_.IC * computeNormalCDF(-d1);
		return term1 - term2;
	}
}

double BlackScholes::computeD1() const
{
	const double logMoneyness = std::log(optionData_.IC / optionData_.K);
	const double volSquaredHalf = 0.5 * optionData_.sig * optionData_.sig;
	const double numerator = logMoneyness + (optionData_.b + volSquaredHalf) * optionData_.T;
	const double denominator = optionData_.sig * std::sqrt(optionData_.T);
	return numerator / denominator;
}

double BlackScholes::computeD2(double d1) const
{
	return d1 - optionData_.sig * sqrtT_;
}