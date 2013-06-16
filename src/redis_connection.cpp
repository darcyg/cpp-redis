#include <cstring>
#include "redis_reply.hpp"
#include "redis_connection.hpp"
#include "redis_exception.hpp"

namespace redis {

connection::connection(const string& host, const int port, const int db)
	: host_(host), port_(port), db_(db)
{
	context_ = NULL;
}

connection::~connection()
{
	disconnect();
}

redisContext* connection::get_handle() const
{
	return context_;
}

bool connection::connect()
{
	disconnect();
	
	context_ = redisConnect(host_.c_str(), port_);
	if (context_->err)
	{
		redisFree(context_);
		context_ = NULL;
		return false;
	}

	return select_database(db_);
}

void connection::disconnect()
{
	if (context_)
	{
		redisFree(context_);
		context_ = NULL;
	}
}

bool connection::active()
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

redisReply* connection::send_command(int argc, char** argv, size_t* argvlen)
{
	redisReply* reply = (redisReply*)redisCommandArgv(context_, argc, (const char**)argv, argvlen);
	if (reply == NULL)
		throw command_exception(context_->errstr);
	return reply;
}

bool connection::select_database(const int db)
{
	redisReply* reply = (redisReply*)redisCommand(context_,  "SELECT %d", db);
	if (reply == NULL)
		return false;

	return recv_ok_reply(reply);
}


}
