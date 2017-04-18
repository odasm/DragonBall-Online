#include <mysqlconn_wrapper.h>

using namespace std;
MySQLConnWrapper MySQLConnWrapper::DB;
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
	m_mutex.lock();
	user = _username;
	if (_password != "" && _password != " " && _password.length() >= 2)
		password = _password;
	else
		password = "";
	host = _host;
	dbmmoDB = _dbmmoDB;
	m_mutex.unlock();
}
bool MySQLConnWrapper::connect()
{
	try
	{
		m_mutex.lock();
		driver = get_driver_instance();
	}
	catch (sql::SQLException &e)
	{
		m_mutex.unlock();
		manageException(e);
		return false;
	}
	try
	{
		con = driver->connect(host, user, password);
	}
	catch (sql::SQLException &e)
	{
		m_mutex.unlock();
		manageException(e);
		return false;
	}
	m_mutex.unlock();
	return true;
}

bool MySQLConnWrapper::switchDb(const string& db_name)
{
	try
	{
		m_mutex.lock();
		con->setSchema(db_name);
	}
	catch (sql::SQLException &e)
	{
		m_mutex.unlock();
		manageException(e);
		return false;
	}
	try
	{
		stmt = con->createStatement();
	}
	catch (sql::SQLException &e)
	{
		m_mutex.unlock();
		manageException(e);
		return false;
	}
	m_mutex.unlock();
	return true;
}

void MySQLConnWrapper::prepare(const string& query)
{
	try
	{
		m_mutex.lock();
		prep_stmt = con->prepareStatement(query);
	}
	catch (sql::SQLException &e)
	{
		m_mutex.unlock();
		manageException(e);
	}
	m_mutex.unlock();
}

void MySQLConnWrapper::setInt(const int& num, const int& data)
{
	m_mutex.lock();
	prep_stmt->setInt(num, data);
	m_mutex.unlock();
}

void MySQLConnWrapper::setString(const int& num, const string& data)
{
	m_mutex.lock();
	prep_stmt->setString(num, data);
	m_mutex.unlock();
}

void MySQLConnWrapper::execute(const string& query)
{
	try 
	{
		m_mutex.lock();
		res = (query != "") ? stmt->executeQuery(query) : prep_stmt->executeQuery();
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
	}
	m_mutex.unlock();
}
void MySQLConnWrapper::closeStatm()
{
	try
	{
		m_mutex.lock();
		prep_stmt->close();
	}
	catch (sql::SQLException &e)
	{
		manageException(e);
	}
	m_mutex.unlock();
}

bool MySQLConnWrapper::fetch()
{
	m_mutex.lock();
	bool value = res->next();
	m_mutex.unlock();
	return value;
}

long double MySQLConnWrapper::getDouble(const char* index)
{
	m_mutex.lock();
	double value = res->getDouble(index);
	m_mutex.unlock();
	return value;
}

bool MySQLConnWrapper::getBoolean(const char* index)
{
	m_mutex.lock();
	bool value = res->getBoolean(index);
	m_mutex.unlock();
	return value;
}

int MySQLConnWrapper::getInt(const char* index)
{
	m_mutex.lock();
	int value = res->getInt(index);
	m_mutex.unlock();
	return value;
}

string MySQLConnWrapper::getString(const char* index)
{
	m_mutex.lock();
	string value = res->getString(index);
	m_mutex.unlock();
	return value;
}

string MySQLConnWrapper::print(const string& field)
{
	m_mutex.lock();
	string value = res->getString(field);
	m_mutex.unlock();
	return value;
}

string MySQLConnWrapper::print(const int& index)
{
	m_mutex.lock();
	string value = res->getString(index);
	m_mutex.unlock();
	return value;
}

size_t MySQLConnWrapper::rowsCount()
{
	m_mutex.lock();
	size_t value = res->rowsCount();
	m_mutex.unlock();
	return value;
}