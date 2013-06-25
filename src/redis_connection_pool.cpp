#include "redis_connection_pool.hpp"

namespace redis {

connection_pool::connection_pool(const std::string& host, const int port, const int db,
		const int size, const int max_size)
	: host_(host), port_(port), db_(db), size_(size), max_size_(max_size)
{
}

connection_pool::~connection_pool()
{
	while (!connq_.empty())
	{
		connection* conn = connq_.front();
		connq_.pop();
		delete conn;
	}
}

int connection_pool::initialize()
{
	int ret = 0;
	for (int i = 0; i < size_; i++)
	{
		connection* conn = new connection(host_, port_, db_);
		if (conn->connect())
			connq_.push(conn);
		else
		{
			ret = -1;
			break;
		}
	}

	return ret;
}

connection* connection_pool::acquire()
{
	lock_guard<mutex> lock(mtx_);
	if (connq_.empty())
	{
		if (size_ < max_size_)
		{
			connection* conn = new connection(host_, port_, db_);
			if (conn->connect())
			{
				connq_.push(conn);
				++size_;
			}
			else
				return NULL;
		}
		else
			return NULL;
	}

	connection* conn = connq_.front();
	connq_.pop();
	if (conn->active() == false)
	{
		--size_;
		delete conn;
		return NULL;
	}

	return conn;
}

void connection_pool::release(connection* conn)
{
	lock_guard<mutex> lock(mtx_);
	connq_.push(conn);
}

}
