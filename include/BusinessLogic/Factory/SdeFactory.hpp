#ifndef SdeFactory_hpp
#define SdeFactory_hpp

#include <memory>
#include <string>
#include <iostream>
#include <Core/Domain/OptionData.hpp>
#include <BusinessLogic/Models/StochasticDifferentialEquations/Sde.hpp>
#include <BusinessLogic/Models/StochasticDifferentialEquations/GBM.hpp>
#include <BusinessLogic/Models/StochasticDifferentialEquations/CEV.hpp>

class SdeFactory {
public:
	static std::shared_ptr<Sde> createSde(const std::string& sdeType, const OptionData& opt);
};

#endif