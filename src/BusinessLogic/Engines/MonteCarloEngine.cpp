#include <BusinessLogic/Engines/MonteCarloEngine.hpp>

using namespace Concurrency;

MonteCarloEngine::MonteCarloEngine(
    std::shared_ptr<Sde> sde, 
    std::shared_ptr<FdmBase> fdm, 
    std::shared_ptr<Rng> rng,
    std::shared_ptr<Pricer> pricer,
	int numSimulations, 
    ExecutionMode executionMode)
    :   PricingEngine("MonteCarloEngine"), 
        sde_(sde), 
        fdm_(fdm), 
        rng_(rng), 
        pricer_(pricer), 
        numSimulations_(numSimulations), 
        executionMode_(executionMode)
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

    if (executionMode_ == ExecutionMode::SingleThreaded) {
        return computePriceSequential();
    }
    else if (executionMode_ == ExecutionMode::OpenMP) {
        return computePriceOpenMP();
    }
    else if (executionMode_ == ExecutionMode::PPL) {
        return computePricePPL();
    }
    return 0.0;
}

double MonteCarloEngine::computePriceSequential()
{
    if (!pricer_) {
        throw std::runtime_error("MonteCarloEngine: pricer not created");
    }

    for (int i = 0; i < numSimulations_; i++) {
        if ((i / 5000) * 5000 == i) {
            std::cout << "Simulation " << i << std::endl;
        }

        generatePath();
        pricer_->ProcessPath(res);
    }

    pricer_->PostProcess();
    return pricer_->Price();
}

double MonteCarloEngine::computePriceOpenMP()
{
    if (!pricer_) {
        throw std::runtime_error("MonteCarloEngine: pricer not created");
    }

    #pragma omp parallel
    {
        std::vector<double> localPath(fdm_->NT + 1);

        #pragma omp for schedule(static)
        for (int i = 0; i < numSimulations_; i++) {
            if ((i / 5000) * 5000 == i) {
                std::cout << "Simulation " << i << std::endl;
            }

            double VOld = sde_->InitialCondition();
            localPath[0] = VOld;
            for (size_t n = 1; n < localPath.size(); n++)
            {
                double VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
                localPath[n] = VNew;
                VOld = VNew;
            }

            #pragma omp critical
            {
                pricer_->ProcessPath(localPath);
            }
        }
    }

    pricer_->PostProcess();
    return pricer_->Price();
}

double MonteCarloEngine::computePricePPL()
{
    concurrency::parallel_for(0, numSimulations_, [&](int i)
    {
        if ((i / 5000) * 5000 == i) {
            std::cout << i << std::endl;
        }

        std::vector<double> localPath(res.size());

        double VOld = sde_->InitialCondition();
        localPath[0] = VOld;
        for (size_t n = 1; n < localPath.size(); n++)
        {
            double VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
            localPath[n] = VNew;
            VOld = VNew;
        }

        concurrency::critical_section::scoped_lock lock(pricerMutex_);
        pricer_->ProcessPath(localPath);
    });

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