#ifndef GreeksCalculator_HPP
#define GreeksCalculator_HPP

#include "Core/Domain/OptionData.hpp"
#include <Core/Domain/Greeks.hpp>
#include <memory>

class GreeksCalculator {
private:
	std::shared_ptr<OptionData> optionData_;

	double calculateDelta() const;
	double calculateGamma() const;
	double calculateVega() const;
	double calculateTheta() const;
	double calculateRho() const;

public:
	explicit GreeksCalculator(const std::shared_ptr<OptionData>& optionData);
	virtual ~GreeksCalculator() = default;

	Greeks getGreeks() const;
};
#endif // !