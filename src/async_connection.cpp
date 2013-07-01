#include <iostream>
#include "async_connection.hpp"
#include "async_service.hpp"

using namespace std;

namespace redis {

static void connect_callback(const redisAsyncContext *ac, int status)
{
    async_connection* conn = get_async_connection(ac);
    conn->on_connect(status);
}

static void disconnect_callback(const redisAsyncContext *ac, int status)
{
    async_connection* conn = get_async_connection(ac);
    conn->on_disconnect(status);
}

async_connection::async_connection()
    : async_context_(NULL)
{
}

async_connection::~async_connection()
{
    disconnect();
}

int async_connection::connect(const string& host, const int port, const int db)
{
	async_context_ = redisAsyncConnect("127.0.0.1", 6379);
    if (async_context_->err)
    {
        cout << "error: " << async_context_->errstr << endl;
        return -1;
    }

    async_service::instance().attach(async_context_);

    redisAsyncSetConnectCallback(async_context_, connect_callback);
    redisAsyncSetDisconnectCallback(async_context_, disconnect_callback);

    return 0;
}

void async_connection::disconnect()
{
	redisAsyncDisconnect(async_context_);
}

void async_connection::on_connect(int status)
{
    if (status != REDIS_OK)
    {
        cout << "Error: " << async_context_->errstr << endl;
        return;
    }
    cout << "Connected...\n";
}

void async_connection::on_disconnect(int status)
{
    if (status != REDIS_OK)
    {
        cout << "Error: " << async_context_->errstr << endl;
        return;
    }
    cout << "Disconnected...\n";
}

}