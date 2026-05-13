#include "BusinessLogic/Engines/BinomialTreeEngine.hpp"

BinomialTreeEngine::BinomialTreeEngine(const std::shared_ptr<BinomialTreeModel> binomialTreeModel, int numSteps)
	: PricingEngine("Lattice"), binomialTreeModel_(binomialTreeModel), numSteps_(numSteps)
{
    if (!binomialTreeModel_) {
        throw std::invalid_argument("LatticeEngine: null model");
    }
}

double BinomialTreeEngine::computePrice()
{
    return binomialTreeModel_->getPrice(numSteps_);
}