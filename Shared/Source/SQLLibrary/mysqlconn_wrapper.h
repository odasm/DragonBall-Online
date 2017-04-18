#pragma once

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <mutex>

using namespace std;

class MySQLConnWrapper
{
	static MySQLConnWrapper DB;
public:
	/* Your MySQL server settings */
	MySQLConnWrapper()
	{
		host = "tcp://127.0.0.1:3306";
		user = "root";
		password = "";
	};
	~MySQLConnWrapper();
	void manageException(sql::SQLException& e);
	bool connect();
	bool switchDb(const string& db_name);
	void prepare(const string& query);
	void setInt(const int& num, const int& data);
	void setString(const int& num, const string& data);
	void execute(const string& query = "");
	bool fetch();
	long double getDouble(const char* index);
	bool getBoolean(const char* index);
	int getInt(const char* index);
	void closeStatm();
	string getString(const char* index);
	string print(const string& field);
	string print(const int& index);
	size_t rowsCount();
	/*
	username , password , host , db
	*/
	void setInfos(string, string, string, string);

	static MySQLConnWrapper *get() noexcept
	{ // pour obtenir le singleton
		return &DB;
	}
private:
	string host;
	string user;
	string password;
	string dbmmoDB;

	sql::Driver* driver;
	sql::Connection* con;
	sql::Statement* stmt;
	sql::PreparedStatement* prep_stmt;
	sql::ResultSet* res;

	std::mutex m_mutex;
};
#define sDB MySQLConnWrapper::get()