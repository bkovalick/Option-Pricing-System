#ifndef MarketDataGateway_HPP
#define MarketDataGateway_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <Infrastructure/GatewayBase.hpp>

using MarketDataResult = mysqlx::SqlResult;
using MarketDataRow = mysqlx::RowResult;

struct MarketData {
	std::string date;
	std::string ticker;
	double open;
	double high;
	double low;
	double close;
	long long volume;

	MarketData()
		: open(0.0), high(0.0), low(0.0), close(0.0), volume(0) {}
};

class MarketDataGateway : public GatewayBase<MarketData>
{
public:
	explicit MarketDataGateway(const std::string& connString, const std::string& schemaName);
	~MarketDataGateway() = default;

	MarketDataGateway(const MarketDataGateway&) = delete;
	MarketDataGateway& operator=(const MarketDataGateway&) = delete;
	MarketDataGateway(MarketDataGateway&&) = default;
	MarketDataGateway& operator=(MarketDataGateway&&) = default;

	std::vector<MarketData> parseResultSet(MarketDataRow& results);
	std::string toInsertStatement(const MarketData& data) const override;

	std::vector<MarketData> getByTicker(const std::string& ticker);
	std::vector<MarketData> getFullUniverseByDate(const std::string& runDate);
};

#endif 