#include <BusinessLogic/Factory/SdeFactory.hpp>

std::shared_ptr<Sde> SdeFactory::createSde(const std::string& sdeType, const OptionData& opt) {

	if (sdeType == "GBM") {
		return std::make_shared<GBM>(opt.r, opt.sig, opt.D, opt.IC, opt.T);
	}
	else if (sdeType == "CEV") {
		return std::make_shared<CEV>(opt.r, opt.sig, opt.D, opt.IC, opt.T, opt.b);
	}
	else {
		throw std::invalid_argument("Unknown sde type: " + sdeType);
	}
}