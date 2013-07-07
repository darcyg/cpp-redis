namespace redis {

template <typename ... Args>
bool Client::mset(const Args & ... args)
{
    string_array params = { args... };
    MakeCmd cmd("MSET");
    cmd << params;
    return execute_and_get_ok_reply(cmd);
}

template <typename ... Args>
int Client::msetnx(const Args & ... args)
{
    string_array params = { args... };
    MakeCmd cmd("MSETNX");
    cmd << params;
    return execute_and_get_int_reply(cmd);
}

template <typename ... Args>
int Client::mget(string_map& kv_map, const Args & ... args)
{
    string_array keys = { args... };
    return mget(keys, kv_map);
}

template <typename ... Args>
int Client::bitop(const BitOp operation, const string& destkey, const Args & ... args)
{
    string_array keys = { args... };
    return bitop(operation, destkey, keys);
}

template <typename ... Args>
int Client::del(const Args & ... args)
{
    string_array keys = { args... };
    return del(keys);
}

template <typename ... Args>
int Client::sadd(const string& key, const Args & ... args)
{
    string_array members = { args... };
    return sadd(key, members);
}

template <typename ... Args>
int Client::sdiff(string_set& s, const Args & ... args)
{
    string_array keys = { args... };
    return sdiff(keys, s);
}

template <typename ... Args>
int Client::sdiffstore(const string& destination, const Args & ... args)
{
    string_array keys = { args... };
    return sdiffstore(destination, keys);
}

template <typename ... Args>
int Client::sinter(string_set& s, const Args & ... args)
{
    string_array keys = { args... };
    return sinter(keys, s);
}

template <typename ... Args>
int Client::sinterstore(const string& destination, const Args & ... args)
{
    string_array keys = { args... };
    return sinterstore(destination, keys);
}

template <typename ... Args>
int Client::sunion(string_set& s, const Args & ... args)
{
    string_array keys = { args... };
    return sunion(keys, s);
}

template <typename ... Args>
int Client::sunionstore(const string& destination, const Args & ... args)
{
    string_array keys = { args... };
    return sunionstore(destination, keys);
}

template <typename ... Args>
int Client::hmget(string_hash_map& h, const string& key, const Args & ... args)
{
    string_array fields = { args... };
    return hmget(key, fields, h);
}

template <typename ... Args>
bool Client::hmset(const string& key, const Args & ... args)
{
    string_array params = { args... };
    MakeCmd cmd("HMSET");
    cmd << key << params;
    return execute_and_get_ok_reply(cmd);
}

template <typename ... Args>
int Client::blpop(const int timeout, string_array& values, const Args & ... args)
{
    string_array keys = { args... };
    return blpop(keys, timeout, values);
}

template <typename ... Args>
int Client::lpush(const string& key, const Args & ... args)
{
    string_array values = { args... };
    return lpush(key, values);
}

template <typename ... Args>
int Client::rpush(const string& key, const Args & ... args)
{
    string_array values = { args... };
    return rpush(key, values);
}

}
