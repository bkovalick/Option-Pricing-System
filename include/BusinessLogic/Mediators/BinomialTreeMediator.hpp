#ifndef BinomialTreeMediator_HPP
#define BinomialTreeMediator_HPP

#include <algorithm>
#include <vector>
#include <iostream>
#include <memory>
#include "BusinessLogic/Mediators/Mediator.hpp"
#include "BusinessLogic/Models/BinomialTreeModel/BinomialTreeModel.hpp"

class BinomialTreeMediator : Mediator
{
private:
	std::shared_ptr<BinomialTreeModel> binomialTreeModel_;
	int numSteps_;

public:
	BinomialTreeMediator(const std::shared_ptr<BinomialTreeModel> binomialTreeModel, int numSteps);
	virtual ~BinomialTreeMediator() = default;
};

BinomialTreeMediator::BinomialTreeMediator(const std::shared_ptr<BinomialTreeModel> binomialTreeModel, int numSteps) 
	: binomialTreeModel_(binomialTreeModel), numSteps_(numSteps)
{

}

#endif
