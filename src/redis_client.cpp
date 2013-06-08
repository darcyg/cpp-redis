#include "redis_client.hpp"
#include "redis_command.hpp"
#include "redis_reply.hpp"
#include "redis_exception.hpp"

namespace redis {

client::client()
{
}

client::~client()
{
	delete pool_;
}

int client::connect(const string& host, const int port, const int db,
		const int pool_size, const int max_pool_size)
{
	pool_ = new connection_pool(host, port, db, pool_size, max_pool_size);
	return pool_->initialize();
}

void client::close()
{
	delete pool_;
	pool_ = NULL;
}

redisReply* client::execute(const rediscmd& cmd)
{
	connection_guard guard = connection_guard(pool_);
	connection* conn = guard.get_connection();
	if (conn == NULL)
		throw connection_exception("no available redis connection");

	return conn->send_command(cmd.argc, cmd.argv, cmd.argvlen);
}

bool client::execute_and_get_status_reply(const rediscmd& cmd)
{
	redisReply* reply = execute(cmd);
	return recv_status_reply(reply);
}

int client::execute_and_get_int_reply(const rediscmd& cmd)
{
	redisReply* reply = execute(cmd);
	return recv_int_reply(reply);
}

int client::execute_and_get_string_reply(const rediscmd& cmd, string& value)
{
	redisReply* reply = execute(cmd);
	return recv_string_reply(reply, value);
}

int client::execute_and_get_string_array_reply(const rediscmd& cmd, string_array& arr)
{
	redisReply* reply = execute(cmd);
	return recv_string_array_reply(reply, arr);
}

int client::execute_and_get_string_pair_array_reply(const rediscmd& cmd, const string_array& keys, 
	string_pair_vector& kv_pair_array)
{
	redisReply* reply = execute(cmd);
	return recv_string_pair_array_reply(reply, keys, kv_pair_array);
}

bool client::set(const string& key, const string& value)
{
	makecmd cmd("SET");
	cmd << key << value;
	return execute_and_get_status_reply(cmd);
}

bool client::setex(const string& key, const string& value, const int seconds)
{
	if (seconds <= 0)
		return false;

	makecmd cmd("SETEX");
	cmd << key << seconds << value;
	return execute_and_get_status_reply(cmd);
}

bool client::psetex(const string& key, const string& value, const int milliseconds)
{
	if (milliseconds <= 0)
		return false;

	makecmd cmd("SETEX");
	cmd << key << milliseconds << value;
	return execute_and_get_status_reply(cmd);
}

int client::setnx(const string& key, const string& value)
{
	makecmd cmd("SETNX");
	cmd << key << value;
	return execute_and_get_int_reply(cmd);
}

bool client::mset(const int key_numbers, ...)
{
	makecmd m("MSET");
	int num = key_numbers * 2;
	va_list ap;
	va_start(ap, key_numbers);
	for (int i = 0; i < num; ++i)
		m << va_arg(ap, char*);
	va_end(ap);
	const rediscmd& cmd = m;
	return execute_and_get_status_reply(cmd);
}

bool client::mset(const string_pair_vector& kv_pairs)
{
	makecmd cmd("MSET");
	for (string_pair_vector::const_iterator it = kv_pairs.begin(); it != kv_pairs.end(); ++it)
		cmd << it->first << it->second;
	return execute_and_get_status_reply(cmd);
}

int client::get(const std::string& key, string& value)
{
	makecmd cmd("GET");
	cmd << key;
	return execute_and_get_string_reply(cmd, value);
}

int client::mget(const string_array& keys, string_pair_vector& kv_pairs)
{
	makecmd cmd("MGET");
	for (string_array::const_iterator it = keys.begin(); it != keys.end(); ++it)
		cmd << *it;
	return execute_and_get_string_pair_array_reply(cmd, keys, kv_pairs);
}

int client::mget(string_pair_vector& kv_pairs, const int key_numbers, ...)
{
	makecmd cmd("MGET");
	string_array keys;

	va_list ap;
	va_start(ap, key_numbers);
	for (int i = 0; i < key_numbers; ++i)
	{
		string key = va_arg(ap, char*);
		cmd << key;
		keys.push_back(key);
	}
	va_end(ap);

	return execute_and_get_string_pair_array_reply(cmd, keys, kv_pairs);
}

int client::exists(const std::string& key)
{
	makecmd cmd("EXISTS");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::expire(const std::string& key, const unsigned int secs)
{
	makecmd m("EXPIRE");
	const rediscmd& cmd = (m << key << secs);
	return execute_and_get_int_reply(cmd);
}

int client::incr(const string& key)
{
	makecmd cmd("INCR");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::incrby(const string& key, int amount)
{
	makecmd cmd("INCRBY");
	cmd << key << amount;
	return execute_and_get_int_reply(cmd);
}

int client::decr(const string& key)
{
	makecmd cmd("DECR");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::decrby(const std::string& key, int amount)
{
	makecmd cmd("DECRBY");
	cmd << key << amount;
	return execute_and_get_int_reply(cmd);
}

int client::keys(const std::string& pattern, string_array& arr)
{
	makecmd cmd("KEYS");
	cmd << pattern;
	return execute_and_get_string_array_reply(cmd, arr);
}

int client::del(const string& key)
{
	makecmd cmd("DEL");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::del(const string_array& keys)
{
	makecmd cmd("DEL");
	for (string_array::const_iterator it = keys.begin(); it != keys.end(); ++it)
		cmd<< *it;
	return execute_and_get_int_reply(cmd);
}

int client::scard(const std::string& key)
{
	makecmd cmd("SCARD");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::sadd(const std::string& key, const std::string& member)
{
	makecmd cmd("SADD");
	cmd << key << member;
	return execute_and_get_int_reply(cmd);
}

int client::smembers(const string& key, string_array& arr)
{
	makecmd cmd("SMEMBERS");
	cmd << key;
	return execute_and_get_string_array_reply(cmd, arr);
}

int client::sismember(const std::string& key, const std::string& member)
{
	makecmd cmd("SISMEMBER");
	cmd << key << member;
	return execute_and_get_int_reply(cmd);
}

int client::srem(const std::string& key, const std::string& member)
{
	makecmd cmd("SREM");
	cmd << key << member;
	return execute_and_get_int_reply(cmd);
}

int client::hincrby(const std::string& key, const std::string& field, const int amount)
{
	makecmd cmd("HINCRBY");
	cmd << key << field << amount;
	return execute_and_get_int_reply(cmd);
}

}
