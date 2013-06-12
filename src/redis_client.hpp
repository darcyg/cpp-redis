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
	void close();

	// string command
	bool set(const string& key, const string& value);
	bool setex(const string& key, const string& value, const int seconds);
	bool psetex(const string& key, const string& value, const int milliseconds);
	int setnx(const string& key, const string& value);

	// the variable argument mset function is not binary safe.
	bool mset(const int pair_num, ...);
	bool mset(const string_map& kv_map);

	int get(const string& key, string& value);
	int mget(const string_array& keys, string_map& kv_map);
	int mget(string_map& kv_map, const int num, ...);

	int exists(const string& key);

	int expire(const string& key, const unsigned int secs);

	int incr(const string& key);

	int incrby(const string& key, int amount);

	int decr(const string& key);

	int decrby(const string& key, int amount);

	// keys command
	int keys(const string& pattern, string_array& arr);

	int del(const string& key);
	int del(const int num, ...);
	int del(const string_array& keys);

	// set command
	int scard(const string& key);
	
	int sadd(const string& key, const string& member);
	int sadd(const string& key, const int num, ...);
	int sadd(const string& key, const string_array& members);

	int spop(const string& key, string& member);

	int smembers(const string& key, string_array& arr);

	int sismember(const string& key, const string& member);

	int srem(const string& key, const string& member);

	// hash command
	int hincrby(const string& key, const string& field, const int amount = 1);

private:
	redisReply* execute(const rediscmd& cmd);

	bool execute_and_get_status_reply(const rediscmd& cmd);
	int execute_and_get_int_reply(const rediscmd& cmd);
	int execute_and_get_string_reply(const rediscmd& cmd, string& str);
	int execute_and_get_string_array_reply(const rediscmd& cmd, string_array& arr);
	int execute_and_get_string_map_reply(const rediscmd& cmd, const string_array& keys, 
		string_map& kv_map);

	connection_pool* pool_;
};

}

#endif
