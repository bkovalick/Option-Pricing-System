#ifndef RngFactory_hpp
#define RngFactory_hpp

#include <iostream>
#include <BusinessLogic/Models/RandomNumberGenerators/Rng.hpp>
#include "BusinessLogic/Models/RandomNumberGenerators/MersenneTwister.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/BoxMuller.hpp"
#include "BusinessLogic/Models/RandomNumberGenerators/LinearCongEngine.hpp"

class RngFactory {
public:
	static std::shared_ptr<Rng> createRng(const std::string& rngType);
};

#endif
