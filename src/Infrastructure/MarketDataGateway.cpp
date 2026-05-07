#include "Infrastructure/MarketDataGateway.hpp"

MarketDataGateway::MarketDataGateway(const std::string& connString, const std::string& schemaName) 
	: GatewayBase<MarketData>(connString, schemaName)
{
	std::cout << "Intializing Market Data Gateway Connection" << std::endl;
}

std::vector<MarketData> MarketDataGateway::parseResultSet(MarketDataRow& result)
{
	std::vector<MarketData> data;
	for (mysqlx::Row row : result) {
		MarketData record;

		// Extract columns by index or name
		record.ticker = row[0].get<std::string>();
		record.date = row[1].get<std::string>();
		record.open = row[2].get<double>();
		record.high = row[3].get<double>();
		record.low = row[4].get<double>(); 
		record.close = row[5].get<double>(); 
		record.volume = row[6].get<long long>();
		data.push_back(record);
	}

	return data;
}

std::string MarketDataGateway::toInsertStatement(const MarketData& data) const
{
	std::ostringstream sql;
	sql << "INSERT INTO market_data (ticker, date, open, high, low, close, volume) VALUES ("
		<< "'" << data.ticker << "', "
		<< "'" << data.date << "', "
		<< std::fixed << std::setprecision(2)
		<< data.open << ", "
		<< data.high << ", "
		<< data.low << ", "
		<< data.close << ", "
		<< data.volume << ")";

	return sql.str();
}

std::vector<MarketData> MarketDataGateway::getByTicker(const std::string& ticker)
{
	std::string query = "SELECT * FROM MarketTable Where Ticker = ?";
	auto results = executeQuery(query, { ticker });
	return parseResultSet(results);
}

std::vector<MarketData> MarketDataGateway::getFullUniverseByDate(const std::string& runDate)
{
	std::string query = "SELECT * FROM MarketTable Where RunDate = ?";
	auto results = executeQuery(query, { runDate });
	return parseResultSet(results);
}
