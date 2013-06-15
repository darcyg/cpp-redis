#ifndef REDIS_CONNECYION_POOL_HPP
#define REDIS_CONNECYION_POOL_HPP

#include <queue>
#include "redis_connection.hpp"
using namespace std;

namespace redis {

class connection_pool {
public:
	connection_pool(const string& host, const int port, const int db,
			const int size = 5, const int max_size = 10);
	~connection_pool();

	int initialize();

	connection* acquire();
	void release(connection* conn);

private:
	queue<connection*> connq_;

	string host_;
	int port_;
	int db_;

	int size_;
	int max_size_;

	pthread_mutex_t lock_;
};

class connection_guard {
public:
	connection_guard(connection_pool* pool)
		: pool_(pool)
	{
		conn_ = pool_->acquire();
	}

	~connection_guard()
	{
		pool_->release(conn_);
	}

	connection* get_connection()
	{
		return conn_;
	}

private:
	connection* conn_;
	connection_pool* pool_;
};

}

#endif
