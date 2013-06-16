#include "redis_client.hpp"
#include "redis_command.hpp"
#include "redis_reply.hpp"
#include "redis_exception.hpp"
#include "redis_url.hpp"

namespace redis {

const char* BITOP_ARR[] = {"AND", "OR", "XOR", "NOT"};
const int BITOP_NUM = sizeof(BITOP_ARR) / sizeof(const char*);

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

int client::connect_with_url(const string& uri)
{
	url redis_url;
	if (parse_url(uri, redis_url) == false)
		return -1;

	return this->connect(redis_url.host, redis_url.port, redis_url.db);
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

float client::execute_and_get_float_reply(const rediscmd& cmd)
{
	redisReply* reply = execute(cmd);
	return recv_float_reply(reply);
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

int client::execute_and_get_string_set_reply(const rediscmd& cmd, string_set& s)
{
	redisReply* reply = execute(cmd);
	return recv_string_set_reply(reply, s);
}

int client::execute_and_get_string_map_reply(const rediscmd& cmd, const string_array& keys, 
	string_map& kv_map)
{
	redisReply* reply = execute(cmd);
	return recv_string_map_reply(reply, keys, kv_map);
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

bool client::mset(const string_map& kv_map)
{
	makecmd cmd("MSET");
	cmd << kv_map;
	return execute_and_get_status_reply(cmd);
}

bool client::mset(const int pair_num, ...)
{
	if (pair_num < 1)
		return false;

	string_map kv_pairs;
	pair_arguments_to_string_map(pair_num, kv_pairs);
	return mset(kv_pairs);
}

int client::msetnx(const string_map& kv_map)
{
	makecmd cmd("MSETNX");
	cmd << kv_map;
	return execute_and_get_int_reply(cmd);
}

int client::msetnx(const int pair_num, ...)
{
	if (pair_num < 1)
		return false;

	string_map kv_pairs;
	pair_arguments_to_string_map(pair_num, kv_pairs);
	return msetnx(kv_pairs);
}

int client::get(const std::string& key, string& value)
{
	makecmd cmd("GET");
	cmd << key;
	return execute_and_get_string_reply(cmd, value);
}

int client::mget(const string_array& keys, string_map& kv_map)
{
	makecmd cmd("MGET");
	cmd << keys;
	return execute_and_get_string_map_reply(cmd, keys, kv_map);
}

int client::mget(string_map& kv_map, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return mget(keys, kv_map);
}

int client::getset(const string& key, const string& value, string& original)
{
	makecmd cmd("GETSET");
	cmd << key << value;
	return execute_and_get_string_reply(cmd, original);
}

int client::setrange(const string& key, const int offset, const string& value)
{
	makecmd cmd("SETRANGE");
	cmd << key << offset << value;
	return execute_and_get_int_reply(cmd);
}

int client::getrange(const string& key, const int start, const int end, string& substring)
{
	makecmd cmd("GETRANGE");
	cmd << key << start << end;
	return execute_and_get_string_reply(cmd, substring);
}

int client::append(const string& key, const string& value)
{
	makecmd cmd("APPEND");
	cmd << key << value;
	return execute_and_get_int_reply(cmd);
}

int client::exists(const std::string& key)
{
	makecmd cmd("EXISTS");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::expire(const std::string& key, const unsigned int secs)
{
	makecmd cmd("EXPIRE");
	cmd << key << secs;
	return execute_and_get_int_reply(cmd);
}

int client::incr(const string& key)
{
	makecmd cmd("INCR");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::incrby(const string& key, const int increment)
{
	makecmd cmd("INCRBY");
	cmd << key << increment;
	return execute_and_get_int_reply(cmd);
}

float client::incrbyfloat(const string& key, const float increment)
{
	makecmd cmd("INCRBYFLOAT");
	cmd << key << increment;
	return execute_and_get_float_reply(cmd);
}

int client::decr(const string& key)
{
	makecmd cmd("DECR");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::decrby(const string& key, const int increment)
{
	makecmd cmd("DECRBY");
	cmd << key << increment;
	return execute_and_get_int_reply(cmd);
}

int client::strlen(const string& key)
{
	makecmd cmd("STRLEN");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::setbit(const string& key, const int offset, const int value)
{
	makecmd cmd("SETBIT");
	cmd << key << offset << value;
	return execute_and_get_int_reply(cmd);
}

int client::getbit(const string& key, const int offset)
{
	makecmd cmd("GETBIT");
	cmd << key << offset;
	return execute_and_get_int_reply(cmd);
}

int client::bitcount(const string& key)
{
	makecmd cmd("BITCOUNT");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::bitcount(const string& key, const int start)
{
	makecmd cmd("BITCOUNT");
	cmd << key << start;
	return execute_and_get_int_reply(cmd);
}

int client::bitcount(const string& key, const int start, const int end)
{
	makecmd cmd("BITCOUNT");
	cmd << key << start << end;
	return execute_and_get_int_reply(cmd);
}

int client::bitop(const string& operation, const string& destkey, const string_array& keys)
{
	// someday, this code will be replaced with vector find function.
	int i = 0;
	for ( ; i < BITOP_NUM; ++i)
	{
		if (BITOP_ARR[i] == operation)
			break;
	}
	if (BITOP_NUM == i)
		return -1;

	if (keys.empty())
		return -1;

	makecmd cmd("BITOP");
	cmd << operation << destkey << keys;
	return execute_and_get_int_reply(cmd);
}

int client::bitop(const string& operation, const string& destkey, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return bitop(operation, destkey, keys);
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
	cmd << keys;
	return execute_and_get_int_reply(cmd);
}

int client::del(const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return del(keys);
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

int client::sadd(const string& key, const string_array& members)
{
	if (members.empty())
		return 0;

	makecmd cmd("SADD");
	cmd << key << members;
	return execute_and_get_int_reply(cmd);
}

int client::sadd(const string& key, const int num, ...)
{
	string_array members;
	arguments_to_string_array(num, members);
	return sadd(key, members);
}

int client::sdiff(const string_array& keys, string_set& s)
{
	s.clear();
	if (keys.empty())
		return 0;

	makecmd cmd("SDIFF");
	cmd << keys;
	return execute_and_get_string_set_reply(cmd, s);
}

int client::sdiff(string_set& s, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return sdiff(keys, s);
}

int client::sdiffstore(const string& destination, const string_array& keys)
{
	makecmd cmd("SDIFFSTORE");
	cmd << destination << keys;
	return execute_and_get_int_reply(cmd);
}

int client::sdiffstore(const string& destination, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return sdiffstore(destination, keys);
}

int client::sinter(const string_array& keys, string_set& s)
{
	s.clear();
	if (keys.empty())
		return 0;

	makecmd cmd("SINTER");
	cmd << keys;
	return execute_and_get_string_set_reply(cmd, s);
}

int client::sinter(string_set& s, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return sinter(keys, s);
}

int client::sinterstore(const string& destination, const string_array& keys)
{
	makecmd cmd("SINTERSTORE");
	cmd << destination << keys;
	return execute_and_get_int_reply(cmd);
}

int client::sinterstore(const string& destination, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return sinterstore(destination, keys);
}

int client::smembers(const string& key, string_set& members)
{
	makecmd cmd("SMEMBERS");
	cmd << key;
	return execute_and_get_string_set_reply(cmd, members);
}

int client::sismember(const std::string& key, const std::string& member)
{
	makecmd cmd("SISMEMBER");
	cmd << key << member;
	return execute_and_get_int_reply(cmd);
}

int client::smove(const string& source, const string& destination, const string& member)
{
	makecmd cmd("SMOVE");
	cmd << source << destination << member;
	return execute_and_get_int_reply(cmd);
}

int client::spop(const string& key, string& member)
{
	makecmd cmd("SPOP");
	cmd << key;
	return execute_and_get_string_reply(cmd, member);
}

int client::srandmember(const string& key, const int count, string_array& members)
{
	makecmd cmd("SRANDMEMBER");
	cmd << key << count;
	return execute_and_get_string_array_reply(cmd, members);
}

int client::srem(const std::string& key, const std::string& member)
{
	makecmd cmd("SREM");
	cmd << key << member;
	return execute_and_get_int_reply(cmd);
}

int client::sunion(const string_array& keys, string_set& s)
{
	s.clear();
	if (keys.empty())
		return 0;

	makecmd cmd("SUNION");
	cmd << keys;
	return execute_and_get_string_set_reply(cmd, s);
}

int client::sunion(string_set& s, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return sdiff(keys, s);
}

int client::sunionstore(const string& destination, const string_array& keys)
{
	makecmd cmd("SUNIONSTORE");
	cmd << destination << keys;
	return execute_and_get_int_reply(cmd);
}

int client::sunionstore(const string& destination, const int num, ...)
{
	string_array keys;
	arguments_to_string_array(num, keys);
	return sunionstore(destination, keys);
}

int client::hincrby(const std::string& key, const std::string& field, const int amount)
{
	makecmd cmd("HINCRBY");
	cmd << key << field << amount;
	return execute_and_get_int_reply(cmd);
}

}
