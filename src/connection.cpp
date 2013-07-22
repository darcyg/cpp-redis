#include <cstring>
#include "reply.hpp"
#include "connection.hpp"
#include "redis_exception.hpp"
#include "uri.hpp"

namespace redis {

Connection::Connection()
    : context_(NULL)
{
}

Connection::~Connection()
{
    disconnect();
}

bool Connection::connect(const string& host, const int port)
{
    context_ = redisConnect(host.c_str(), port);
    if (context_->err)
    {
        redisFree(context_);
        context_ = NULL;
        return false;
    }

    return true;
}

bool Connection::connect(const string& url)
{
    URI uri;
    if (!URI::parse(url, uri))
        return false;
    return connect(uri.host, uri.port);
}

void Connection::disconnect()
{
    if (context_)
    {
        quit();
        redisFree(context_);
        context_ = NULL;
    }
}

bool Connection::ping()
{
    redisReply* reply = (redisReply*)redisCommand(context_,  "PING");
    if (reply == NULL)
        return false;
        
    bool ret = false;
    if (strncmp(reply->str, REDIS_STATUS_REPLY_PONG, reply->len) == 0)
        ret = true;
    freeReplyObject(reply);
    return ret;
}

bool Connection::auth(const string& password)
{
    return true;
}

bool Connection::select(const int db)
{
    redisReply* reply = (redisReply*)redisCommand(context_,  "SELECT %d", db);
    if (reply == NULL)
        return false;

    return recv_ok_reply(reply);
}

bool Connection::quit()
{
    redisReply* reply = (redisReply*)redisCommand(context_,  "QUIT");
    if (reply != NULL)
        recv_ok_reply(reply);
    return true;
}

redisReply* Connection::send_command(const RedisCmd& cmd)
{
    redisReply* reply = (redisReply*)redisCommandArgv(context_,
        cmd.argc, (const char**)cmd.argv, cmd.argvlen);
    if (reply == NULL)
        throw CommandException(context_->errstr);
    return reply;
}

}
