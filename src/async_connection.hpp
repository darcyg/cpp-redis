#ifndef ASYNC_CONNECTION_HPP
#define ASYNC_CONNECTION_HPP

#include <string>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include "command.hpp"

using namespace std;

namespace redis {

class AsyncConnection {
public:
    AsyncConnection();
    ~AsyncConnection();

    int connect(const string& host, const int port, const int db = 0);
    int connect(const string& url);
    void disconnect();

    void on_connect(int status);
    void on_disconnect(int status);

    int send_command(const RedisCmd& cmd, redisCallbackFn* fn, void* privatedata);

private:
    redisAsyncContext* async_context_;
    string host_;
    int port_;
    int db_; 
};

}

#endif
