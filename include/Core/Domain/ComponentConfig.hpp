#ifndef ComponentConfig_HPP
#define ComponentConfig_HPP

#include <string>

struct ComponentConfig
{
    std::string sdeType;
    std::string fdmType;
    std::string rngType;

    ComponentConfig(const std::string& sde, const std::string& fdm, const std::string& rng)
        : sdeType(sde), fdmType(fdm), rngType(rng)
    {}
};

#endif