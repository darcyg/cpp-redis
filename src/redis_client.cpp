#include "redis_client.hpp"
#include "command.hpp"
#include "reply.hpp"
#include "redis_exception.hpp"
#include "uri.hpp"

namespace redis {

array<string, BitOp_Num> bitop_strs = {"AND", "OR", "XOR", "NOT"};
array<string, InsertDirection_Num> insert_direction_str = {"BEFORE", "AFTER"};

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

int client::connect_with_url(const string& url)
{
	Uri uri;
	if (Uri::parse(url, uri) == false)
		return -1;

	return this->connect(uri.host, uri.port, uri.db);
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

	return conn->send_command(cmd);
}

bool client::execute_and_get_ok_reply(const rediscmd& cmd)
{
	redisReply* reply = execute(cmd);
	return recv_ok_reply(reply);
}

string client::execute_and_get_status_reply(const rediscmd& cmd)
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
	string_map& m)
{
	redisReply* reply = execute(cmd);
	return recv_string_map_reply(reply, keys, m);
}

int client::execute_and_get_string_hash_map_reply(const rediscmd& cmd, string_hash_map& h)
{
	redisReply* reply = execute(cmd);
	return recv_string_hash_map_reply(reply, h);
}

int client::execute_and_get_string_hash_map_reply(const rediscmd& cmd, const string_array& fields, 
	string_hash_map& h)
{
	redisReply* reply = execute(cmd);
	return recv_string_hash_map_reply(reply, fields, h);
}

bool client::set(const string& key, const string& value)
{
	makecmd cmd("SET");
	cmd << key << value;
	return execute_and_get_ok_reply(cmd);
}

bool client::setex(const string& key, const string& value, const int seconds)
{
	if (seconds <= 0)
		return false;

	makecmd cmd("SETEX");
	cmd << key << seconds << value;
	return execute_and_get_ok_reply(cmd);
}

bool client::psetex(const string& key, const string& value, const int milliseconds)
{
	if (milliseconds <= 0)
		return false;

	makecmd cmd("SETEX");
	cmd << key << milliseconds << value;
	return execute_and_get_ok_reply(cmd);
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
	return execute_and_get_ok_reply(cmd);
}

int client::msetnx(const string_map& kv_map)
{
	makecmd cmd("MSETNX");
	cmd << kv_map;
	return execute_and_get_int_reply(cmd);
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

int client::bitop(const BitOp operation, const string& destkey, const string_array& keys)
{
	if (keys.empty())
		return -1;

	makecmd cmd("BITOP");
	cmd << bitop_strs[operation] << destkey << keys;
	return execute_and_get_int_reply(cmd);
}

int client::exists(const std::string& key)
{
	makecmd cmd("EXISTS");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::expire(const string& key, const unsigned int seconds)
{
	makecmd cmd("EXPIRE");
	cmd << key << seconds;
	return execute_and_get_int_reply(cmd);
}

int client::expireat(const string& key, const unsigned int timestamp)
{
	makecmd cmd("EXPIREAT");
	cmd << key << timestamp;
	return execute_and_get_int_reply(cmd);
}

int client::pexpire(const string& key, const unsigned int milliseconds)
{
	makecmd cmd("PEXPIRE");
	cmd << key << milliseconds;
	return execute_and_get_int_reply(cmd);
}

int client::pexpireat(const string& key, const unsigned int milliseconds_timestamp)
{
	makecmd cmd("PEXPIREAT");
	cmd << key << milliseconds_timestamp;
	return execute_and_get_int_reply(cmd);
}

int client::ttl(const string& key)
{
	makecmd cmd("TTL");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::pttl(const string& key)
{
	makecmd cmd("PTTL");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::persist(const string& key)
{
	makecmd cmd("PERSIST");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::keys(const std::string& pattern, string_array& arr)
{
	makecmd cmd("KEYS");
	cmd << pattern;
	return execute_and_get_string_array_reply(cmd, arr);
}

int client::del(const string_array& keys)
{
	makecmd cmd("DEL");
	cmd << keys;
	return execute_and_get_int_reply(cmd);
}

string client::type(const string& key)
{
	makecmd cmd("TYPE");
	cmd << key;
	return execute_and_get_status_reply(cmd);
}

int client::scard(const std::string& key)
{
	makecmd cmd("SCARD");
	cmd << key;
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

int client::sdiff(const string_array& keys, string_set& s)
{
	s.clear();
	if (keys.empty())
		return 0;

	makecmd cmd("SDIFF");
	cmd << keys;
	return execute_and_get_string_set_reply(cmd, s);
}

int client::sdiffstore(const string& destination, const string_array& keys)
{
	makecmd cmd("SDIFFSTORE");
	cmd << destination << keys;
	return execute_and_get_int_reply(cmd);
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

int client::sinterstore(const string& destination, const string_array& keys)
{
	makecmd cmd("SINTERSTORE");
	cmd << destination << keys;
	return execute_and_get_int_reply(cmd);
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

int client::sunionstore(const string& destination, const string_array& keys)
{
	makecmd cmd("SUNIONSTORE");
	cmd << destination << keys;
	return execute_and_get_int_reply(cmd);
}

int client::hget(const string& key, const string& field, string& value)
{
	makecmd cmd("HGET");
	cmd << key << field;
	return execute_and_get_string_reply(cmd, value);
}

int client::hmget(const string& key, const string_array& fields, string_hash_map& h)
{
	makecmd cmd("HMGET");
	cmd << key << fields;
	return execute_and_get_string_hash_map_reply(cmd, fields, h);
}

int client::hset(const string& key, const string& field, const string& value)
{
	makecmd cmd("HSET");
	cmd << key << field << value;
	return execute_and_get_int_reply(cmd);
}

bool client::hmset(const string& key, const string_hash_map& h)
{
	makecmd cmd("HMSET");
	cmd << key << h;
	return execute_and_get_ok_reply(cmd);
}

int client::hsetnx(const string& key, const string& field, const string& value)
{
	makecmd cmd("HSETNX");
	cmd << key << field << value;
	return execute_and_get_int_reply(cmd);
}

int client::hexists(const string& key, const string& field)
{
	makecmd cmd("HEXISTS");
	cmd << key << field;
	return execute_and_get_int_reply(cmd);
}

int client::hincrby(const string& key, const string& field, const int increment)
{
	makecmd cmd("HINCRBY");
	cmd << key << field << increment;
	return execute_and_get_int_reply(cmd);
}

float client::hincrbyfloat(const string& key, const string& filed, const float increment)
{
	makecmd cmd("HINCRBYFLOAT");
	cmd << key << filed << increment;
	return execute_and_get_float_reply(cmd);
}

int client::hkeys(const string& key, string_array& fields)
{
	makecmd cmd("HKEYS");
	cmd << key;
	return execute_and_get_string_array_reply(cmd, fields);
}

int client::hvals(const string& key, string_array& values)
{
	makecmd cmd("HVALS");
	cmd << key;
	return execute_and_get_string_array_reply(cmd, values);
}

int client::hgetall(const string& key, string_hash_map& h)
{
	makecmd cmd("HGETALL");
	cmd << key;
	return execute_and_get_string_hash_map_reply(cmd, h);
}

int client::hlen(const string& key)
{
	makecmd cmd("HLEN");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::blpop(const string_array& keys, const int timeout, string_array& values)
{
	makecmd cmd("BLPOP");
	cmd << keys << timeout;
	return execute_and_get_string_array_reply(cmd, values);
}

int client::linsert(const string& key, InsertDirection direction, 
	const int pivot, const string& value)
{
	makecmd cmd("LINSERT");
	cmd << key << insert_direction_str[direction] << pivot << value;
	return execute_and_get_int_reply(cmd);
}


int client::lindex(const string& key, const int index, string& value)
{
	makecmd cmd("LINDEX");
	cmd << key << index;
	return execute_and_get_string_reply(cmd, value);
}

int client::llen(const string& key)
{
	makecmd cmd("LLEN");
	cmd << key;
	return execute_and_get_int_reply(cmd);
}

int client::lpop(const string& key, string& value)
{
	makecmd cmd("LPOP");
	cmd << key;
	return execute_and_get_string_reply(cmd, value);
}

int client::lpush(const string& key, const string_array& values)
{
	makecmd cmd("LPUSH");
	cmd << key << values;
	return execute_and_get_int_reply(cmd);
}

int client::lpushx(const string& key, const string& value)
{
	makecmd cmd("LPUSHX");
	cmd << key << value;
	return execute_and_get_int_reply(cmd);
}

int client::lrange(const string& key, const int start, const int stop, string_array& values)
{
	makecmd cmd("LRANGE");
	cmd << key << start << stop;
	return execute_and_get_string_array_reply(cmd, values);
}

int client::lrem(const string& key, const int count, const string& value)
{
	makecmd cmd("LREM");
	cmd << key << count << value;
	return execute_and_get_int_reply(cmd);
}

bool client::lset(const string& key, const int count, const string& value)
{
	makecmd cmd("LSET");
	cmd << key << count << value;
	return execute_and_get_ok_reply(cmd);
}

bool client::ltrim(const string& key, const int start, const int stop)
{
	makecmd cmd("LTRIM");
	cmd << key << start << stop;
	return execute_and_get_ok_reply(cmd);
}

int client::rpop(const string& key, string& value)
{
	makecmd cmd("RPOP");
	cmd << key;
	return execute_and_get_string_reply(cmd, value);
}

int client::rpoplpush(const string& source, const string& destination, string_array& values)
{
	makecmd cmd("RPOPLPUSH");
	cmd << source << destination;
	return execute_and_get_string_array_reply(cmd, values);
}

int client::brpoplpush(const string& source, const string& destination, const int timeout, 
	string_array& values)
{
	makecmd cmd("BRPOPLPUSH");
	cmd << source << destination << timeout;
	return execute_and_get_string_array_reply(cmd, values);
}

int client::rpush(const string& key, const string_array& values)
{
	makecmd cmd("RPUSH");
	cmd << key << values;
	return execute_and_get_int_reply(cmd);
}

int client::rpushx(const string& key, const string& value)
{
	makecmd cmd("RPUSHX");
	cmd << key << value;
	return execute_and_get_int_reply(cmd);
}


}
