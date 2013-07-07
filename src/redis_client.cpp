#include "redis_client.hpp"
#include "command.hpp"
#include "reply.hpp"
#include "redis_exception.hpp"
#include "uri.hpp"

namespace redis {

array<string, BitOp_Num> bitop_strs = {"AND", "OR", "XOR", "NOT"};
array<string, InsertDirection_Num> insert_direction_str = {"BEFORE", "AFTER"};

Client::Client()
{
}

Client::~Client()
{
    delete pool_;
}

int Client::connect(const string& host, const int port, const int db,
        const int pool_size, const int max_pool_size)
{
    pool_ = new ConnectionPool(host, port, db, pool_size, max_pool_size);
    return pool_->initialize();
}

int Client::connect_with_url(const string& url)
{
    Uri uri;
    if (Uri::parse(url, uri) == false)
        return -1;

    return this->connect(uri.host, uri.port, uri.db);
}

void Client::close()
{
    delete pool_;
    pool_ = NULL;
}

redisReply* Client::execute(const RedisCmd& cmd)
{
    ConnectionGuard guard = ConnectionGuard(pool_);
    Connection* conn = guard.get_connection();
    if (conn == NULL)
        throw ConnectionException("no available redis Connection");

    return conn->send_command(cmd);
}

bool Client::execute_and_get_ok_reply(const RedisCmd& cmd)
{
    redisReply* reply = execute(cmd);
    return recv_ok_reply(reply);
}

string Client::execute_and_get_status_reply(const RedisCmd& cmd)
{
    redisReply* reply = execute(cmd);
    return recv_status_reply(reply);
}

int Client::execute_and_get_int_reply(const RedisCmd& cmd)
{
    redisReply* reply = execute(cmd);
    return recv_int_reply(reply);
}

float Client::execute_and_get_float_reply(const RedisCmd& cmd)
{
    redisReply* reply = execute(cmd);
    return recv_float_reply(reply);
}

int Client::execute_and_get_string_reply(const RedisCmd& cmd, string& value)
{
    redisReply* reply = execute(cmd);
    return recv_string_reply(reply, value);
}

int Client::execute_and_get_string_array_reply(const RedisCmd& cmd, string_array& arr)
{
    redisReply* reply = execute(cmd);
    return recv_string_array_reply(reply, arr);
}

int Client::execute_and_get_string_set_reply(const RedisCmd& cmd, string_set& s)
{
    redisReply* reply = execute(cmd);
    return recv_string_set_reply(reply, s);
}

int Client::execute_and_get_string_map_reply(const RedisCmd& cmd, const string_array& keys, 
    string_map& m)
{
    redisReply* reply = execute(cmd);
    return recv_string_map_reply(reply, keys, m);
}

int Client::execute_and_get_string_hash_map_reply(const RedisCmd& cmd, string_hash_map& h)
{
    redisReply* reply = execute(cmd);
    return recv_string_hash_map_reply(reply, h);
}

int Client::execute_and_get_string_hash_map_reply(const RedisCmd& cmd, const string_array& fields, 
    string_hash_map& h)
{
    redisReply* reply = execute(cmd);
    return recv_string_hash_map_reply(reply, fields, h);
}

bool Client::set(const string& key, const string& value)
{
    MakeCmd cmd("SET");
    cmd << key << value;
    return execute_and_get_ok_reply(cmd);
}

bool Client::setex(const string& key, const string& value, const int seconds)
{
    if (seconds <= 0)
        return false;

    MakeCmd cmd("SETEX");
    cmd << key << seconds << value;
    return execute_and_get_ok_reply(cmd);
}

bool Client::psetex(const string& key, const string& value, const int milliseconds)
{
    if (milliseconds <= 0)
        return false;

    MakeCmd cmd("SETEX");
    cmd << key << milliseconds << value;
    return execute_and_get_ok_reply(cmd);
}

int Client::setnx(const string& key, const string& value)
{
    MakeCmd cmd("SETNX");
    cmd << key << value;
    return execute_and_get_int_reply(cmd);
}

bool Client::mset(const string_map& kv_map)
{
    MakeCmd cmd("MSET");
    cmd << kv_map;
    return execute_and_get_ok_reply(cmd);
}

int Client::msetnx(const string_map& kv_map)
{
    MakeCmd cmd("MSETNX");
    cmd << kv_map;
    return execute_and_get_int_reply(cmd);
}

int Client::get(const std::string& key, string& value)
{
    MakeCmd cmd("GET");
    cmd << key;
    return execute_and_get_string_reply(cmd, value);
}

int Client::mget(const string_array& keys, string_map& kv_map)
{
    MakeCmd cmd("MGET");
    cmd << keys;
    return execute_and_get_string_map_reply(cmd, keys, kv_map);
}

int Client::getset(const string& key, const string& value, string& original)
{
    MakeCmd cmd("GETSET");
    cmd << key << value;
    return execute_and_get_string_reply(cmd, original);
}

int Client::setrange(const string& key, const int offset, const string& value)
{
    MakeCmd cmd("SETRANGE");
    cmd << key << offset << value;
    return execute_and_get_int_reply(cmd);
}

int Client::getrange(const string& key, const int start, const int end, string& substring)
{
    MakeCmd cmd("GETRANGE");
    cmd << key << start << end;
    return execute_and_get_string_reply(cmd, substring);
}

int Client::append(const string& key, const string& value)
{
    MakeCmd cmd("APPEND");
    cmd << key << value;
    return execute_and_get_int_reply(cmd);
}

int Client::incr(const string& key)
{
    MakeCmd cmd("INCR");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::incrby(const string& key, const int increment)
{
    MakeCmd cmd("INCRBY");
    cmd << key << increment;
    return execute_and_get_int_reply(cmd);
}

float Client::incrbyfloat(const string& key, const float increment)
{
    MakeCmd cmd("INCRBYFLOAT");
    cmd << key << increment;
    return execute_and_get_float_reply(cmd);
}

int Client::decr(const string& key)
{
    MakeCmd cmd("DECR");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::decrby(const string& key, const int increment)
{
    MakeCmd cmd("DECRBY");
    cmd << key << increment;
    return execute_and_get_int_reply(cmd);
}

int Client::strlen(const string& key)
{
    MakeCmd cmd("STRLEN");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::setbit(const string& key, const int offset, const int value)
{
    MakeCmd cmd("SETBIT");
    cmd << key << offset << value;
    return execute_and_get_int_reply(cmd);
}

int Client::getbit(const string& key, const int offset)
{
    MakeCmd cmd("GETBIT");
    cmd << key << offset;
    return execute_and_get_int_reply(cmd);
}

int Client::bitcount(const string& key)
{
    MakeCmd cmd("BITCOUNT");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::bitcount(const string& key, const int start)
{
    MakeCmd cmd("BITCOUNT");
    cmd << key << start;
    return execute_and_get_int_reply(cmd);
}

int Client::bitcount(const string& key, const int start, const int end)
{
    MakeCmd cmd("BITCOUNT");
    cmd << key << start << end;
    return execute_and_get_int_reply(cmd);
}

int Client::bitop(const BitOp operation, const string& destkey, const string_array& keys)
{
    if (keys.empty())
        return -1;

    MakeCmd cmd("BITOP");
    cmd << bitop_strs[operation] << destkey << keys;
    return execute_and_get_int_reply(cmd);
}

int Client::exists(const std::string& key)
{
    MakeCmd cmd("EXISTS");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::expire(const string& key, const unsigned int seconds)
{
    MakeCmd cmd("EXPIRE");
    cmd << key << seconds;
    return execute_and_get_int_reply(cmd);
}

int Client::expireat(const string& key, const unsigned int timestamp)
{
    MakeCmd cmd("EXPIREAT");
    cmd << key << timestamp;
    return execute_and_get_int_reply(cmd);
}

int Client::pexpire(const string& key, const unsigned int milliseconds)
{
    MakeCmd cmd("PEXPIRE");
    cmd << key << milliseconds;
    return execute_and_get_int_reply(cmd);
}

int Client::pexpireat(const string& key, const unsigned int milliseconds_timestamp)
{
    MakeCmd cmd("PEXPIREAT");
    cmd << key << milliseconds_timestamp;
    return execute_and_get_int_reply(cmd);
}

int Client::ttl(const string& key)
{
    MakeCmd cmd("TTL");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::pttl(const string& key)
{
    MakeCmd cmd("PTTL");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::persist(const string& key)
{
    MakeCmd cmd("PERSIST");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::keys(const std::string& pattern, string_array& arr)
{
    MakeCmd cmd("KEYS");
    cmd << pattern;
    return execute_and_get_string_array_reply(cmd, arr);
}

int Client::del(const string_array& keys)
{
    MakeCmd cmd("DEL");
    cmd << keys;
    return execute_and_get_int_reply(cmd);
}

string Client::type(const string& key)
{
    MakeCmd cmd("TYPE");
    cmd << key;
    return execute_and_get_status_reply(cmd);
}

int Client::scard(const std::string& key)
{
    MakeCmd cmd("SCARD");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::sadd(const string& key, const string_array& members)
{
    if (members.empty())
        return 0;

    MakeCmd cmd("SADD");
    cmd << key << members;
    return execute_and_get_int_reply(cmd);
}

int Client::sdiff(const string_array& keys, string_set& s)
{
    s.clear();
    if (keys.empty())
        return 0;

    MakeCmd cmd("SDIFF");
    cmd << keys;
    return execute_and_get_string_set_reply(cmd, s);
}

int Client::sdiffstore(const string& destination, const string_array& keys)
{
    MakeCmd cmd("SDIFFSTORE");
    cmd << destination << keys;
    return execute_and_get_int_reply(cmd);
}

int Client::sinter(const string_array& keys, string_set& s)
{
    s.clear();
    if (keys.empty())
        return 0;

    MakeCmd cmd("SINTER");
    cmd << keys;
    return execute_and_get_string_set_reply(cmd, s);
}

int Client::sinterstore(const string& destination, const string_array& keys)
{
    MakeCmd cmd("SINTERSTORE");
    cmd << destination << keys;
    return execute_and_get_int_reply(cmd);
}

int Client::smembers(const string& key, string_set& members)
{
    MakeCmd cmd("SMEMBERS");
    cmd << key;
    return execute_and_get_string_set_reply(cmd, members);
}

int Client::sismember(const std::string& key, const std::string& member)
{
    MakeCmd cmd("SISMEMBER");
    cmd << key << member;
    return execute_and_get_int_reply(cmd);
}

int Client::smove(const string& source, const string& destination, const string& member)
{
    MakeCmd cmd("SMOVE");
    cmd << source << destination << member;
    return execute_and_get_int_reply(cmd);
}

int Client::spop(const string& key, string& member)
{
    MakeCmd cmd("SPOP");
    cmd << key;
    return execute_and_get_string_reply(cmd, member);
}

int Client::srandmember(const string& key, const int count, string_array& members)
{
    MakeCmd cmd("SRANDMEMBER");
    cmd << key << count;
    return execute_and_get_string_array_reply(cmd, members);
}

int Client::srem(const std::string& key, const std::string& member)
{
    MakeCmd cmd("SREM");
    cmd << key << member;
    return execute_and_get_int_reply(cmd);
}

int Client::sunion(const string_array& keys, string_set& s)
{
    s.clear();
    if (keys.empty())
        return 0;

    MakeCmd cmd("SUNION");
    cmd << keys;
    return execute_and_get_string_set_reply(cmd, s);
}

int Client::sunionstore(const string& destination, const string_array& keys)
{
    MakeCmd cmd("SUNIONSTORE");
    cmd << destination << keys;
    return execute_and_get_int_reply(cmd);
}

int Client::hget(const string& key, const string& field, string& value)
{
    MakeCmd cmd("HGET");
    cmd << key << field;
    return execute_and_get_string_reply(cmd, value);
}

int Client::hmget(const string& key, const string_array& fields, string_hash_map& h)
{
    MakeCmd cmd("HMGET");
    cmd << key << fields;
    return execute_and_get_string_hash_map_reply(cmd, fields, h);
}

int Client::hset(const string& key, const string& field, const string& value)
{
    MakeCmd cmd("HSET");
    cmd << key << field << value;
    return execute_and_get_int_reply(cmd);
}

bool Client::hmset(const string& key, const string_hash_map& h)
{
    MakeCmd cmd("HMSET");
    cmd << key << h;
    return execute_and_get_ok_reply(cmd);
}

int Client::hsetnx(const string& key, const string& field, const string& value)
{
    MakeCmd cmd("HSETNX");
    cmd << key << field << value;
    return execute_and_get_int_reply(cmd);
}

int Client::hexists(const string& key, const string& field)
{
    MakeCmd cmd("HEXISTS");
    cmd << key << field;
    return execute_and_get_int_reply(cmd);
}

int Client::hincrby(const string& key, const string& field, const int increment)
{
    MakeCmd cmd("HINCRBY");
    cmd << key << field << increment;
    return execute_and_get_int_reply(cmd);
}

float Client::hincrbyfloat(const string& key, const string& filed, const float increment)
{
    MakeCmd cmd("HINCRBYFLOAT");
    cmd << key << filed << increment;
    return execute_and_get_float_reply(cmd);
}

int Client::hkeys(const string& key, string_array& fields)
{
    MakeCmd cmd("HKEYS");
    cmd << key;
    return execute_and_get_string_array_reply(cmd, fields);
}

int Client::hvals(const string& key, string_array& values)
{
    MakeCmd cmd("HVALS");
    cmd << key;
    return execute_and_get_string_array_reply(cmd, values);
}

int Client::hgetall(const string& key, string_hash_map& h)
{
    MakeCmd cmd("HGETALL");
    cmd << key;
    return execute_and_get_string_hash_map_reply(cmd, h);
}

int Client::hlen(const string& key)
{
    MakeCmd cmd("HLEN");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::blpop(const string_array& keys, const int timeout, string_array& values)
{
    MakeCmd cmd("BLPOP");
    cmd << keys << timeout;
    return execute_and_get_string_array_reply(cmd, values);
}

int Client::linsert(const string& key, InsertDirection direction, 
    const int pivot, const string& value)
{
    MakeCmd cmd("LINSERT");
    cmd << key << insert_direction_str[direction] << pivot << value;
    return execute_and_get_int_reply(cmd);
}


int Client::lindex(const string& key, const int index, string& value)
{
    MakeCmd cmd("LINDEX");
    cmd << key << index;
    return execute_and_get_string_reply(cmd, value);
}

int Client::llen(const string& key)
{
    MakeCmd cmd("LLEN");
    cmd << key;
    return execute_and_get_int_reply(cmd);
}

int Client::lpop(const string& key, string& value)
{
    MakeCmd cmd("LPOP");
    cmd << key;
    return execute_and_get_string_reply(cmd, value);
}

int Client::lpush(const string& key, const string_array& values)
{
    MakeCmd cmd("LPUSH");
    cmd << key << values;
    return execute_and_get_int_reply(cmd);
}

int Client::lpushx(const string& key, const string& value)
{
    MakeCmd cmd("LPUSHX");
    cmd << key << value;
    return execute_and_get_int_reply(cmd);
}

int Client::lrange(const string& key, const int start, const int stop, string_array& values)
{
    MakeCmd cmd("LRANGE");
    cmd << key << start << stop;
    return execute_and_get_string_array_reply(cmd, values);
}

int Client::lrem(const string& key, const int count, const string& value)
{
    MakeCmd cmd("LREM");
    cmd << key << count << value;
    return execute_and_get_int_reply(cmd);
}

bool Client::lset(const string& key, const int count, const string& value)
{
    MakeCmd cmd("LSET");
    cmd << key << count << value;
    return execute_and_get_ok_reply(cmd);
}

bool Client::ltrim(const string& key, const int start, const int stop)
{
    MakeCmd cmd("LTRIM");
    cmd << key << start << stop;
    return execute_and_get_ok_reply(cmd);
}

int Client::rpop(const string& key, string& value)
{
    MakeCmd cmd("RPOP");
    cmd << key;
    return execute_and_get_string_reply(cmd, value);
}

int Client::rpoplpush(const string& source, const string& destination, string_array& values)
{
    MakeCmd cmd("RPOPLPUSH");
    cmd << source << destination;
    return execute_and_get_string_array_reply(cmd, values);
}

int Client::brpoplpush(const string& source, const string& destination, const int timeout, 
    string_array& values)
{
    MakeCmd cmd("BRPOPLPUSH");
    cmd << source << destination << timeout;
    return execute_and_get_string_array_reply(cmd, values);
}

int Client::rpush(const string& key, const string_array& values)
{
    MakeCmd cmd("RPUSH");
    cmd << key << values;
    return execute_and_get_int_reply(cmd);
}

int Client::rpushx(const string& key, const string& value)
{
    MakeCmd cmd("RPUSHX");
    cmd << key << value;
    return execute_and_get_int_reply(cmd);
}

int Client::publish(const string& channel, const string& message)
{
    MakeCmd cmd("PUBLISH");
    cmd << channel << message;
    return execute_and_get_int_reply(cmd);
}

}
