#ifndef BlackScholes_HPP
#define BlackScholes_HPP

#include <Core/Domain/OptionData.hpp>

class BlackScholes
{
private:
	const OptionData optionData_;
	const double sqrtT_;

	static double computeNormalCDF(double x);
	double computeD1() const;
	double computeD2(double d1) const;

public:
	explicit BlackScholes(const OptionData& optData);
	virtual ~BlackScholes() = default;

	// Delete copy operations (holds reference)
	BlackScholes(const BlackScholes&) = delete;
	BlackScholes& operator=(const BlackScholes&) = delete;

	// Allow move operations
	BlackScholes(BlackScholes&&) = default;
	BlackScholes& operator=(BlackScholes&&) = default;

	double computePrice() const;
};

#endif // !1
