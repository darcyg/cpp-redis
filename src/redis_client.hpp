#ifndef REDIS_CLIENT_HPP
#define REDIS_CLIENT_HPP

#include "redis_types.hpp"
#include "redis_command.hpp"
#include "redis_connection_pool.hpp"

using namespace std;

namespace redis {

class client {
public:
	client();
	~client();

	int connect(const string& host, const int port = 6379, const int db = 0,
			const int pool_size = 5, const int max_pool_size = 10);
	int connect_with_url(const string& url);

	void close();

	// string command
	bool set(const string& key, const string& value);
	bool setex(const string& key, const string& value, const int seconds);
	bool psetex(const string& key, const string& value, const int milliseconds);
	int setnx(const string& key, const string& value);

	bool mset(const string_map& kv_map);
	template <typename ... Args>
	bool mset(const Args & ... args);

	int msetnx(const string_map& kv_map);
	template <typename ... Args>
	int msetnx(const Args & ... args);

	int get(const string& key, string& value);
	int mget(const string_array& keys, string_map& kv_map);
	template <typename ... Args>
	int mget(string_map& kv_map, const Args & ... args);

	int getset(const string& key, const string& value, string& original);

	int setrange(const string& key, const int offset, const string& value);
	int getrange(const string& key, const int start, const int end, string& substring);

	int append(const string& key, const string& value);

	int incr(const string& key);
	int incrby(const string& key, const int increment);
	float incrbyfloat(const string& key, const float increment);

	int decr(const string& key);
	int decrby(const string& key, const int increment);

	int strlen(const string& key);

	int setbit(const string& key, const int offset, const int value);
	int getbit(const string& key, const int offset);

	int bitcount(const string& key);
	int bitcount(const string& key, const int start);
	int bitcount(const string& key, const int start, const int end );

	int bitop(const BitOp operation, const string& destkey, const string_array& keys);
	template <typename ... Args>
	int bitop(const BitOp operation, const string& destkey, const Args & ... args);

	// keys command
	int exists(const string& key);

	int expire(const string& key, const unsigned int seconds);
	int expireat(const string& key, const unsigned int timestamp);

	int pexpire(const string& key, const unsigned int milliseconds);
	int pexpireat(const string& key, const unsigned int milliseconds_timestamp);

	int ttl(const string& key);
	int pttl(const string& key);

	int persist(const string& key);

	int keys(const string& pattern, string_array& arr);

	int del(const string_array& keys);
	template <typename ... Args>
	int del(const Args & ... args);

	string type(const string& key);

	// set command
	int scard(const string& key);
	
	int sadd(const string& key, const string_array& members);
	template <typename ... Args>
	int sadd(const string& key, const Args & ... args);

	int sdiff(const string_array& keys, string_set& s);
	template <typename ... Args>
	int sdiff(string_set& s, const Args & ... args);

	int sdiffstore(const string& destination, const string_array& keys);
	template <typename ... Args>
	int sdiffstore(const string& destination, const Args & ... args);

	int sinter(const string_array& keys, string_set& s);
	template <typename ... Args>
	int sinter(string_set& s, const Args & ... args);

	int sinterstore(const string& destination, const string_array& keys);
	template <typename ... Args>
	int sinterstore(const string& destination, const Args & ... args);

	int smembers(const string& key, string_set& members);

	int sismember(const string& key, const string& member);

	int smove(const string& source, const string& destination, const string& member);

	int spop(const string& key, string& member);

	int srandmember(const string& key, const int count, string_array& members);

	int srem(const string& key, const string& member);

	int sunion(const string_array& keys, string_set& s);
	template <typename ... Args>
	int sunion(string_set& s, const Args & ... args);

	int sunionstore(const string& destination, const string_array& keys);
	template <typename ... Args>
	int sunionstore(const string& destination, const Args & ... args);

	// hash command
	int hget(const string& key, const string& field, string& value);

	int hmget(const string& key, const string_array& fields, string_hash_map& h);
	template <typename ... Args>
	int hmget(string_hash_map& h, const string& key, const Args & ... args);

	int hset(const string& key, const string& field, const string& value);

	bool hmset(const string& key, const string_hash_map& h);
	template <typename ... Args>
	bool hmset(const string& key, const Args & ... args);

	int hsetnx(const string& key, const string& field, const string& value);

	int hexists(const string& key, const string& field);

	int hincrby(const string& key, const string& field, const int increment = 1);
	float hincrbyfloat(const string& key, const string& filed, const float increment);

	int hkeys(const string& key, string_array& fields);
	int hvals(const string& key, string_array& values);
	int hgetall(const string& key, string_hash_map& h);

	int hlen(const string& key);

	// list command
	template <typename ... Args>
	int blpop(const int timeout, string_array& values, const Args & ... args);
	int blpop(const string_array& keys, const int timeout, string_array& values);

	int linsert(const string& key, InsertDirection direction, const int pivot, const string& value);

	int lindex(const string& key, const int index, string& value);

	int llen(const string& key);

	int lpop(const string& key, string& value);

	template <typename ... Args>
	int lpush(const string& key, const Args & ... args);
	int lpush(const string& key, const string_array& values);

	int lpushx(const string& key, const string& value);

	int lrange(const string& key, const int start, const int stop, string_array& values);

	int lrem(const string& key, const int count, const string& value);

	bool lset(const string& key, const int count, const string& value);

	bool ltrim(const string& key, const int start, const int stop);

	int rpop(const string& key, string& value);

	int rpoplpush(const string& source, const string& destination, string_array& values);
	int brpoplpush(const string& source, const string& destination, const int timeout, 
		string_array& values);

	template <typename ... Args>
	int rpush(const string& key, const Args & ... args);
	int rpush(const string& key, const string_array& values);

	int rpushx(const string& key, const string& value);


private:
	redisReply* execute(const rediscmd& cmd);

	bool execute_and_get_ok_reply(const rediscmd& cmd);
	string execute_and_get_status_reply(const rediscmd& cmd);
	int execute_and_get_int_reply(const rediscmd& cmd);
	float execute_and_get_float_reply(const rediscmd& cmd);
	int execute_and_get_string_reply(const rediscmd& cmd, string& str);
	int execute_and_get_string_array_reply(const rediscmd& cmd, string_array& arr);
	int execute_and_get_string_set_reply(const rediscmd& cmd, string_set& s);
	int execute_and_get_string_map_reply(const rediscmd& cmd, const string_array& keys, 
		string_map& m);
	int execute_and_get_string_hash_map_reply(const rediscmd& cmd, string_hash_map& h);
	int execute_and_get_string_hash_map_reply(const rediscmd& cmd, const string_array& fileds, 
		string_hash_map& h);

	connection_pool* pool_;
};

}

#include "redis_client.inl"

#endif
