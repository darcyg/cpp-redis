#include <iostream>
#include "uri.hpp"
#include "async_connection.hpp"
#include "async_service.hpp"

using namespace std;

namespace redis {

static void connect_callback(const redisAsyncContext *ac, int status)
{
    AsyncConnection* conn = (AsyncConnection*)ac->data;
    conn->on_connect(status);
}

static void disconnect_callback(const redisAsyncContext *ac, int status)
{
    AsyncConnection* conn = (AsyncConnection*)ac->data;
    conn->on_disconnect(status);
}

AsyncConnection::AsyncConnection()
    : async_context_(NULL)
{
}

AsyncConnection::~AsyncConnection()
{
}

int AsyncConnection::connect(const string& host, const int port, const int db)
{
    async_context_ = redisAsyncConnect(host.c_str(), port);
    async_context_->data = this;
    if (async_context_->err)
    {
        cout << "error: " << async_context_->errstr << endl;
        return -1;
    }

    AsyncService::instance().attach(async_context_);

    redisAsyncSetConnectCallback(async_context_, connect_callback);
    redisAsyncSetDisconnectCallback(async_context_, disconnect_callback);

    return 0;
}

int AsyncConnection::connect(const string& url)
{
    URI uri;
    if (!URI::parse(url, uri)) 
    {
        // TODO log error
        return -1;
    }

    return connect(uri.host, uri.port);
}

void AsyncConnection::disconnect()
{
    redisAsyncDisconnect(async_context_);
}

int AsyncConnection::send_command(const RedisCmd& cmd, redisCallbackFn* fn, void* privatedata)
{
    return redisAsyncCommandArgv(async_context_, fn, privatedata,
        cmd.argc, (const char**)cmd.argv, cmd.argvlen);
}

void AsyncConnection::on_connect(int status)
{
    if (status != REDIS_OK)
    {
        cout << "Error: " << async_context_->errstr << endl;
        return;
    }
    cout << "Connected...\n";
}

void AsyncConnection::on_disconnect(int status)
{
    if (status != REDIS_OK) 
    {
        cout << "Error: " << async_context_->errstr << endl;
        return;
    }
    cout << "Disconnected...\n";
}

}
