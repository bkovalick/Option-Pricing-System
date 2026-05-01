#ifndef MarketDataGateway_HPP
#define MarketDataGateway_HPP

#include <string>
#include <memory>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <mysql/jdbc.h> 

template<typename T>
class GatewayBase
{
private:
	std::string connString_;
	std::unique_ptr<sql::Connection> connection_; // replace the string with a Connection type

protected:
	explicit GatewayBase(const std::string& connString) : connString_(connString)
	{
		connect();
	}

public:
	virtual ~GatewayBase() = default;

	void connect()
	{
		std::cout << "Connect to database using connection string." << std::endl;
		try {
			sql::Driver* driver = sql::mysql::get_driver_instance();
			// Connect and store the handle
			connection.reset(driver->connect(url, user, pass));
		}
		catch (sql::SQLException& e) {
			throw std::runtime_error("Connection failed: " + std::string(e.what()));
		}
	}

	bool isConnected() const;
	void addToDatabase(const T& t);
	T getFromDatabase(std::string sqlQuery);
};

struct MarketData {
	std::string runDate;
	std::string ticker;
	float priceOpen;
	float priceClose;
	float askPrice;
	float bidPrice;
	float midPrice;
};

class MarketDataGateway : public GatewayBase<MarketData>
{
private:
	std::string baseSql_ = "";
	std::vector<MarketData> fullUniverse_;

public:
	explicit MarketDataGateway(const std::string& connString);
	~MarketDataGateway() = default;

	MarketDataGateway(const MarketDataGateway&) = delete;
	MarketDataGateway& operator=(const MarketDataGateway&) = delete;
	MarketDataGateway(MarketDataGateway&&) = default;
	MarketDataGateway& operator=(MarketDataGateway&&) = default;

	std::vector<MarketData> getByTicker(std::string ticker);
	std::vector<MarketData> getFullUniverseByDate(std::string runDate);
};

#endif 