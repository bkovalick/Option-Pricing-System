#include "BusinessLogic/Factory/RngFactory.hpp"

std::shared_ptr<Rng> RngFactory::createRng(const std::string& rngType) {
	if (rngType == "MersenneTwister") {
		return std::make_shared<MersenneTwister>();
	}
	else if (rngType == "BoxMuller") {
		return std::make_shared<BoxMuller>();
	}
	else if (rngType == "LinearCongEngine") {
		return std::make_shared<LinearCongEngine>();
	}
	else {
		throw std::invalid_argument("Unknown rng type: " + rngType);
	}
}