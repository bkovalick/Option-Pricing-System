#ifndef FdmFactory_hpp
#define FdmFactory_hpp

#include <memory>
#include <string>
#include <iostream>
#include <Core/Domain/OptionData.hpp>
#include "BusinessLogic/Models/FiniteDifferenceMethods/FDM.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FdmBase.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/EulerFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/ExactFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/MilsteinFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/DiscreteMilstein.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/PredictorCorrectorFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/Heun.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/DerivativeFree.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/ModifiedPredictorCorrectorFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/MidpointPredictorCorrectorFdm.hpp"
#include "BusinessLogic/Models/FiniteDifferenceMethods/FittedMidpointPredictorCorrectorFdm.hpp"

class FdmFactory {
public:
	static std::shared_ptr<FdmBase> FdmFactory::createFdm(const std::string& fdmType,
		const OptionData& opt, std::shared_ptr<Sde> sde, int NT);
};

#endif