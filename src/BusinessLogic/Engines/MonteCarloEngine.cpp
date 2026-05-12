#include <BusinessLogic/Engines/MonteCarloEngine.hpp>

MonteCarloEngine::MonteCarloEngine(
    std::shared_ptr<Sde> sde, 
    std::shared_ptr<FdmBase> fdm, 
    std::shared_ptr<Rng> rng,
    std::shared_ptr<Pricer> pricer,
	int numSimulations)
    : PricingEngine("MonteCarloEngine"), sde_(sde), fdm_(fdm), rng_(rng), pricer_(pricer), numSimulations_(numSimulations)
{
    if (!sde_ || !fdm_ || !rng_ || !pricer_) {
        throw std::invalid_argument("MonteCarloEngine: null components");
    }
    res.resize(fdm_->NT + 1);
}

double MonteCarloEngine::computePrice()
{
    if (!pricer_) {
        throw std::runtime_error("MonteCarloEngine: pricer not created");
    }

    for (int i = 0; i <= numSimulations_; i++) {
        if ((i / 5000) * 5000 == i) {
            std::cout << "Simulation " << i << std::endl;
        }

        generatePath();
        pricer_->ProcessPath(res);
    }

    pricer_->PostProcess();
    return pricer_->Price();
}

void MonteCarloEngine::generatePath() 
{
    double VOld, VNew;
    VOld = sde_->InitialCondition(); res[0] = VOld;
    for (size_t n = 1; n < res.size(); n++)
    {
        VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
        res[n] = VNew; 
        VOld = VNew;
    }
}