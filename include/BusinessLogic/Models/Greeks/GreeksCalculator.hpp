#ifndef GreeksCalculator_HPP
#define GreeksCalculator_HPP

#include "Core/Domain/OptionData.hpp"

class GreeksCalculator {
private:
	OptionData optionData_;

public:
	GreeksCalculator() = default;
	GreeksCalculator(const OptionData& optData);
	virtual ~GreeksCalculator() = default;

	virtual double calculateDelta() const;
	virtual double calculateGamma() const;
	virtual double calculateVega() const;
	virtual double calculateTheta() const;
	virtual double calculateRho() const;
};
#endif // !
