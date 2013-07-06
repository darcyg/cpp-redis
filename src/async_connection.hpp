#ifndef ASYNC_CONNECTION_HPP
#define ASYNC_CONNECTION_HPP

#include <string>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include "command.hpp"

using namespace std;

namespace redis {

class async_connection {
	friend inline async_connection* get_async_connection(const redisAsyncContext* ac);
public:
	async_connection();
	~async_connection();

	int connect(const string& host, const int port = 6379, const int db = 0);
	void disconnect();

	void on_connect(int status);
	void on_disconnect(int status);

	int send_command(const rediscmd& cmd, redisCallbackFn* fn, void* privatedata);

private:
	redisAsyncContext* async_context_;
	string host_;
	int port_;
	int db_; 
};

inline async_connection* get_async_connection(const redisAsyncContext* ac)
{
	async_connection* conn = NULL;
	return (async_connection*)((char*)(&ac) - (long)(&conn->async_context_));
}

}

#endif