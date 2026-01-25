#include "BusinessLogic/Factory/FdmFactory.hpp"


std::shared_ptr<FdmBase> FdmFactory::createFdm(const std::string& fdmType, const OptionData& opt,
	std::shared_ptr<Sde> sde, int NT) {

	if (fdmType == "EulerFdm")
	{
		return std::make_shared<EulerFdm>(sde, NT);
	}
	else if (fdmType == "ExactFdm")
	{
		return std::make_shared<ExactFdm>(sde, NT, opt.IC, opt.r, opt.sig);
	}
	else if (fdmType == "MilsteinFdm")
	{
		return std::make_shared<MilsteinFdm>(sde, NT);
	}
	else if (fdmType == "DiscreteMilstein")
	{
		return std::make_shared<DiscreteMilstein>(sde, NT);
	}
	else if (fdmType == "PredictorCorrectorFdm")
	{
		int a = 0.5;
		int b = 0.5;
		return std::make_shared<PredictorCorrectorFdm>(sde, NT, a, b);
	}
	else if (fdmType == "Heun")
	{
		return std::make_shared<Heun>(sde, NT);
	}
	else if (fdmType == "DerivativeFree")
	{
		return std::make_shared<DerivativeFree>(sde, NT);
	}
	else if (fdmType == "ModifiedPredictorCorrectorFdm")
	{
		int a = 0.5;
		int b = 0.5;
		return std::make_shared<ModifiedPredictorCorrectorFdm>(sde, NT, a, b);
	}
	else if (fdmType == "MidpointPredictorCorrectorFdm")
	{
		int a = 0.5;
		int b = 0.5;
		return std::make_shared<MidpointPredictorCorrectorFdm>(sde, NT, a, b);
	}
	else if (fdmType == "FittedMidpointPredictorCorrectorFdm")
	{
		int a = 0.5;
		int b = 0.5;
		return std::make_shared<FittedMidpointPredictorCorrectorFdm>(sde, NT, a, b);
	}
	else {
		throw std::invalid_argument("Unknown sde type: " + fdmType);
	}
}