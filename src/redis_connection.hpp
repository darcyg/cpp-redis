#ifndef REDIS_CONNECTION_HPP
#define REDIS_CONNECTION_HPP

#include <hiredis/hiredis.h>
#include "redis_types.hpp"
using namespace std;

namespace redis {

class connection {
public:
	connection(const string& host, const int port, const int db);
	virtual ~connection();

	// hope this function will be never used.
	redisContext* get_handle() const;

	bool connect();
	void disconnect();
	bool ping();

	redisReply* send_command(int argc, char** argv, size_t* argvlen);

private:
	bool auth(const string& password);

	bool select(const int db);

	bool quit();

private:
	redisContext* context_;
	string host_;
	int port_;
	int db_;
};

}

#endif
