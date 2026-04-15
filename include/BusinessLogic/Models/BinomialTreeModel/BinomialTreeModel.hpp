#ifndef BinomialTreeModel_HPP
#define BinomialTreeModel_HPP


#include <vector>

using Matrix = std::vector<std::vector<int>>;

class BinomialTreeModel
{
private:
	int numSteps_;

public:
	explicit BinomialTreeModel(int numSteps);
	~BinomialTreeModel() = default;

	BinomialTreeModel(const BinomialTreeModel& model) = delete;
	BinomialTreeModel& operator=(const BinomialTreeModel& model) = delete;

	BinomialTreeModel(BinomialTreeModel&& model) = default;
	BinomialTreeModel& operator=(BinomialTreeModel&& model) = default;
};

#endif
