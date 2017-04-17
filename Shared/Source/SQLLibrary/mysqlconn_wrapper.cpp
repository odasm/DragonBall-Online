#include <mysqlconn_wrapper.h>

using namespace std;

MySQLConnWrapper::~MySQLConnWrapper()
{
	delete res;
	delete prep_stmt;
	delete stmt;
	delete con;
}

void MySQLConnWrapper::manageException(sql::SQLException& e)
{
	if (e.getErrorCode() != 0) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}
}
/*
	username , password , host , db
*/
void MySQLConnWrapper::setInfos(string _username, string _password, string _host, string _dbmmoDB)
{
	user = _username;
	if (_password != "" && _password != " " && _password.length() >= 2)
		password = _password;
	else
		password = "";
	host = _host;
	dbmmoDB = _dbmmoDB;
}
bool MySQLConnWrapper::connect()
{
	try
	{
		driver = get_driver_instance();
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
		return false;
	}
	try
	{
		con = driver->connect(host, user, password);
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
		return false;
	}
	return true;
}

bool MySQLConnWrapper::switchDb(const string& db_name)
{
	try
	{
		con->setSchema(db_name);
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
		return false;
	}
	try
	{
		stmt = con->createStatement();
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
		return false;
	}
	return true;
}

void MySQLConnWrapper::prepare(const string& query)
{
	try
	{
		prep_stmt = con->prepareStatement(query);
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
	}
}

void MySQLConnWrapper::setInt(const int& num, const int& data)
{
	prep_stmt->setInt(num, data);
}

void MySQLConnWrapper::setString(const int& num, const string& data)
{
	prep_stmt->setString(num, data);
}

void MySQLConnWrapper::execute(const string& query)
{
	try 
	{
		res = (query != "") ? stmt->executeQuery(query) : prep_stmt->executeQuery();
	}
	catch (sql::SQLException &e) {
		manageException(e);
	}
}void MySQLConnWrapper::closeStatm()
{
	try
	{
		prep_stmt->close();
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
	}
}

bool MySQLConnWrapper::fetch()
{
	return res->next();
}

long double MySQLConnWrapper::getDouble(const char* index)
{
	return res->getDouble(index);
}

bool MySQLConnWrapper::getBoolean(const char* index)
{
	return res->getBoolean(index);
}

int MySQLConnWrapper::getInt(const char* index)
{
	return res->getInt(index);
}

string MySQLConnWrapper::getString(const char* index)
{
	return res->getString(index);
}

string MySQLConnWrapper::print(const string& field)
{
	return res->getString(field);
}

string MySQLConnWrapper::print(const int& index)
{
	return res->getString(index);
}

size_t MySQLConnWrapper::rowsCount()
{
	return res->rowsCount();
}