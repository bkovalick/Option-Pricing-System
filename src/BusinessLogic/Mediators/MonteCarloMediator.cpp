#include "BusinessLogic/Mediators/MonteCarloMediator.hpp"
#include <BusinessLogic/Models/StochasticDifferentialEquations/GBM.hpp>
#include <Core/Configuration/SimulationConfig.hpp>

using namespace Concurrency;

MonteCarloMediator::MonteCarloMediator() : NSim(10000)
{
}

MonteCarloMediator::MonteCarloMediator(std::tuple<std::shared_ptr<Sde>, std::shared_ptr<FdmBase>, std::shared_ptr<Rng>> parts,
	int numberSimulations) : NSim(numberSimulations)
{
	sde_ = std::get<0>(parts);
	fdm_ = std::get<1>(parts);
	rng_ = std::get<2>(parts);
	res.resize(fdm_->NT + 1);
}

MonteCarloMediator::~MonteCarloMediator()
{
}

void MonteCarloMediator::connectPricers(const std::shared_ptr<Pricer>& pricer, int slot)
{
	path.connect(slot, std::bind(&Pricer::ProcessPath, pricer, std::placeholders::_1));
	finish.connect(slot, std::bind(&Pricer::PostProcess, pricer));
}

void MonteCarloMediator::start()
{
	double VOld, VNew;
	for (long i = 1; i <= NSim; ++i)
	{
		if ((i / 5000) * 5000 == i)
		{
			std::cout << i << std::endl;
		}
		
		VOld = sde_->InitialCondition(); res[0] = VOld;
		for (unsigned int n = 1; n < res.size(); n++)
		{
			VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
			res[n] = VNew; VOld = VNew;
		}
		path(res);
	}
	
	finish();
	disconnect();
}

void MonteCarloMediator::startOpenMP()
{
	double VOld, VNew;
	int tid = 0;

	#pragma omp parallel for
	for (long i = 1; i <= NSim; ++i)
	{
		tid = omp_get_thread_num();

		if ((i / 5000) * 5000 == i)
		{
			std::cout << i << std::endl;
		}

		VOld = sde_->InitialCondition(); res[0] = VOld;
		for (unsigned int n = 1; n < res.size(); n++)
		{
			VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
			res[n] = VNew; VOld = VNew;
		}
		path(res);
	}
	std::cout << "NumThreads: " << tid << std::endl;

	finish();
	disconnect();
}

void MonteCarloMediator::startPPL()
{
	double VOld, VNew;
	concurrency::parallel_for(0, NSim, [&](int i)
	{
		if ((i / 5000) * 5000 == i)
		{
			std::cout << i << std::endl;
		}

		VOld = sde_->InitialCondition(); res[0] = VOld;
		std::size_t size = res.size();
		concurrency::parallel_for(std::size_t(0), size, [&](std::size_t n) {
			VNew = fdm_->advance(VOld, fdm_->meshArray[n - 1], fdm_->k, rng_->gen(), rng_->gen());
			res[n] = VNew; VOld = VNew;
		});

		path(res);
	});

	finish();
	disconnect();
}

void MonteCarloMediator::disconnect()
{
	path.disconnect_all_slots();
	finish.disconnect_all_slots();
}