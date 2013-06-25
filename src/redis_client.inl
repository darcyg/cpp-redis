namespace redis {

template <typename ... Args>
bool client::mset(const Args & ... args)
{
	string_array params = { args... };
	makecmd cmd("MSET");
	cmd << params;
	return execute_and_get_ok_reply(cmd);
}

template <typename ... Args>
int client::msetnx(const Args & ... args)
{
	string_array params = { args... };
	makecmd cmd("MSETNX");
	cmd << params;
	return execute_and_get_int_reply(cmd);
}

template <typename ... Args>
int client::mget(string_map& kv_map, const Args & ... args)
{
	string_array keys = { args... };
	return mget(keys, kv_map);
}

template <typename ... Args>
int client::bitop(const BitOp operation, const string& destkey, const Args & ... args)
{
	string_array keys = { args... };
	return bitop(operation, destkey, keys);
}

template <typename ... Args>
int client::del(const Args & ... args)
{
	string_array keys = { args... };
	return del(keys);
}

template <typename ... Args>
int client::sadd(const string& key, const Args & ... args)
{
	string_array members = { args... };
	return sadd(key, members);
}

template <typename ... Args>
int client::sdiff(string_set& s, const Args & ... args)
{
	string_array keys = { args... };
	return sdiff(keys, s);
}

template <typename ... Args>
int client::sdiffstore(const string& destination, const Args & ... args)
{
	string_array keys = { args... };
	return sdiffstore(destination, keys);
}

template <typename ... Args>
int client::sinter(string_set& s, const Args & ... args)
{
	string_array keys = { args... };
	return sinter(keys, s);
}

template <typename ... Args>
int client::sinterstore(const string& destination, const Args & ... args)
{
	string_array keys = { args... };
	return sinterstore(destination, keys);
}

template <typename ... Args>
int client::sunion(string_set& s, const Args & ... args)
{
	string_array keys = { args... };
	return sunion(keys, s);
}

template <typename ... Args>
int client::sunionstore(const string& destination, const Args & ... args)
{
	string_array keys = { args... };
	return sunionstore(destination, keys);
}

template <typename ... Args>
int client::hmget(string_hash_map& h, const string& key, const Args & ... args)
{
	string_array fields = { args... };
	return hmget(key, fields, h);
}

template <typename ... Args>
bool client::hmset(const string& key, const Args & ... args)
{
	string_array params = { args... };
	makecmd cmd("HMSET");
	cmd << key << params;
	return execute_and_get_ok_reply(cmd);
}

template <typename ... Args>
int client::blpop(const int timeout, string_array& values, const Args & ... args)
{
	string_array keys = { args... };
	return blpop(keys, timeout, values);
}

template <typename ... Args>
int client::lpush(const string& key, const Args & ... args)
{
	string_array values = { args... };
	return lpush(key, values);
}

template <typename ... Args>
int client::rpush(const string& key, const Args & ... args)
{
	string_array values = { args... };
	return rpush(key, values);
}

}