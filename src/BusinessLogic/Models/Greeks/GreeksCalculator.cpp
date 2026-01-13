#include "BusinessLogic/Models/Greeks/GreeksCalculator.hpp"

GreeksCalculator::GreeksCalculator(const OptionData& optData)
	: optionData_(optData)
{
}

double GreeksCalculator::calculateDelta() const
{
	// Placeholder implementation
	return 0.0;
}

double GreeksCalculator::calculateGamma() const
{
	// Placeholder implementation
	return 0.0;
}

double GreeksCalculator::calculateVega() const
{
	// Placeholder implementation
	return 0.0;
}

double GreeksCalculator::calculateTheta() const
{
	// Placeholder implementation
	return 0.0;
}

double GreeksCalculator::calculateRho() const
{
	// Placeholder implementation
	return 0.0;
}