#include "Infrastructure/MarketDataGateway.hpp"

MarketDataGateway::MarketDataGateway(const std::string& connString) 
	: GatewayBase<MarketData>(connString)
{
	std::cout << "Intializing Market Data Gateway Connection" << std::endl;
}

std::vector<MarketData> MarketDataGateway::getByTicker(std::string ticker)
{
	if (!isConnected()) {
		throw std::runtime_error("Market Data Gateway Connection is not open!");
	}
	return fullUniverse_;
}

std::vector<MarketData> MarketDataGateway::getFullUniverseByDate(std::string runDate)
{
	if (!isConnected()) {
		throw std::runtime_error("Market Data Gateway Connection is not open!");
	}
	return fullUniverse_;
}
