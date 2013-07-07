#include <cstring>
#include "reply.hpp"
#include "connection.hpp"
#include "redis_exception.hpp"

namespace redis {

Connection::Connection(const string& host, const int port, const int db)
    : host_(host), port_(port), db_(db)
{
    context_ = NULL;
}

Connection::~Connection()
{
    disconnect();
}

redisContext* Connection::get_handle() const
{
    return context_;
}

bool Connection::connect()
{
    disconnect();
    
    context_ = redisConnect(host_.c_str(), port_);
    if (context_->err)
    {
        redisFree(context_);
        context_ = NULL;
        return false;
    }

    return select(db_);
}

void Connection::disconnect()
{
    quit();

    if (context_)
    {
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

redisReply* Connection::send_command(const RedisCmd& cmd)
{
    redisReply* reply = (redisReply*)redisCommandArgv(context_,
        cmd.argc, (const char**)cmd.argv, cmd.argvlen);
    if (reply == NULL)
        throw CommandException(context_->errstr);
    return reply;
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
    return true;
}

}
