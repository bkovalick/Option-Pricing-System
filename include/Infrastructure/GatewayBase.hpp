#ifndef GatewayBase_HPP
#define GatewayBase_HPP

#include <mysqlx/xdevapi.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <iostream>

template<typename T>
class GatewayBase
{
private:
	std::string connString_;
	std::unique_ptr<mysqlx::Session> session_;
	std::unique_ptr<mysqlx::Schema> schema_;
	void connect(const std::string& schemaName);
	void ensureConnected() const {
		if (session_ == nullptr) {
			throw std::runtime_error("Database connection is not established");
		}
	}

protected:
	explicit GatewayBase(const std::string& connString, const std::string& schemaName);
	mysqlx::RowResult executeQuery(const std::string& sqlQuery, const std::vector<mysqlx::Value>& params = {});
	

public:
	virtual ~GatewayBase() = default;

	bool addToDatabase(const std::vector<T>& objects);
	bool saveAll(const T& object);
	
	std::string getConnectionString() const;
	bool isConnected() const;

	virtual std::string toInsertStatement(const T& data) const = 0;
};

template<typename T>
GatewayBase<T>::GatewayBase(const std::string& connString, const std::string& schemaName) 
	: connString_(connString)
{
	connect(schemaName);
}

template<typename T>
void GatewayBase<T>::connect(const std::string& schemaName)
{
	try {
		session_ = std::make_unique<mysqlx::Session>(connString_);
		schema_ = std::make_unique<mysqlx::Schema>(session_->getSchema(schemaName));
		std::cout << "Connected to database: " << schemaName << std::endl;
	}
	catch (const mysqlx::Error& err) {
		throw std::runtime_error("Connection failed: " + std::string(err.what()));
	}
}

template<typename T>
mysqlx::RowResult GatewayBase<T>::executeQuery(const std::string& sqlQuery, const std::vector<mysqlx::Value>& params = {})
{
	ensureConnected();
	try {
		auto stmt = session_->sql(sqlQuery);
		for (const auto& param : params) {
			stmt.bind(param);
		}

		return stmt.execute();
	}
	catch (const mysqlx::Error& err) {
		throw std::runtime_error("Sql Database Execution error: " + std::string(err.what()));
	}
}

template<typename T>
bool GatewayBase<T>::addToDatabase(const std::vector<T>& objects)
{
	if (objects.empty()) return true;

	ensureConnected();
	try {
		session_->startTransaction();

		for (const auto& obj : objects) {
			std::string query = toInsertStatement(obj);
			auto stmt = session_->sql(query);
			stmt.execute();
		}

		session_->commit();
		return true;
	}
	catch (const mysqlx::Error& err) {
		session_->rollback();
		std::cerr << "Sql Database Insertion error: " + std::string(err.what());
		return false;
	}
}

template<typename T>
bool GatewayBase<T>::saveAll(const T& object)
{
	ensureConnected();
	try {
		std::string query = toInsertStatement(object);
		auto stmt = session_->sql(query);
		auto result = stmt.execute();
		return result.getAffectedItemsCount() > 0;
	}
	catch (const mysqlx::Error& err) {
		std::cerr << "Sql Database Insertion error: " + std::string(err.what());
		return false;
	}
}

template<typename T>
std::string GatewayBase<T>::getConnectionString() const
{
	return connString_;
}

template<typename T>
bool GatewayBase<T>::isConnected() const
{
	return session_ != nullptr;
}
#endif
