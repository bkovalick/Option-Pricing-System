#ifndef BinomialTreeEngine_HPP
#define BinomialTreeEngine_HPP

#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>
#include "BusinessLogic/Models/BinomialTreeModel/BinomialTreeModel.hpp"
#include "BusinessLogic/Engines/PricingEngine.hpp"

class BinomialTreeEngine : public PricingEngine
{
private:
	std::shared_ptr<BinomialTreeModel> binomialTreeModel_;
	int numSteps_;

public:
	BinomialTreeEngine(const std::shared_ptr<BinomialTreeModel> binomialTreeModel, int numSteps);
	virtual ~BinomialTreeEngine() = default;

	double computePrice() override;
};

#endif
