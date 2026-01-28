#ifndef SimulationInstance_HPP
#define SimulationInstance_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <BusinessLogic/Mediators/Mediator.hpp>

struct SimulationInstance
{
private:
	int instanceId_;
	std::unique_ptr<Mediator> mediator_;
	std::unordered_map<std::string, std::shared_ptr<Pricer>> pricers_;
	std::string sde_;
	std::string fdm_;
	std::string rng_;
	std::string optionName_;

public:
	SimulationInstance(int instanceId, const std::string& sde, 
		const std::string& fdm, const std::string& rng, const std::string& optionName) 
		: instanceId_(instanceId), sde_(sde), fdm_(fdm), rng_(rng), optionName_(optionName)
	{}

	SimulationInstance(SimulationInstance&&) = default;
	SimulationInstance& operator=(SimulationInstance&&) = default;

	SimulationInstance(SimulationInstance&) = delete;
	SimulationInstance& operator=(SimulationInstance&) = delete;

};
#endif

