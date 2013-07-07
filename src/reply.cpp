#include <cstring>
#include <cstdlib>
#include "reply.hpp"
#include "redis_exception.hpp"

namespace redis {

class ReplyGuard {
public:
    ReplyGuard(redisReply* reply)
        : reply_(reply)
    {
    }

    ~ReplyGuard()
    {
        freeReplyObject(reply_);
    }

private:
    redisReply* reply_;
};

static void throw_redis_exception(redisReply* reply)
{
    if (reply->type == REDIS_REPLY_ERROR)
        throw ValueException(reply->str);
    else if (reply->type == REDIS_REPLY_NIL)
        throw ValueException("nil reply");
    else
        throw RedisException("Unknown exception");
}

bool recv_ok_reply(redisReply* reply)
{
    bool ret = false;
    ReplyGuard guard(reply);

    if (reply->type == REDIS_REPLY_STATUS)
    {
        if (strncmp(reply->str, REDIS_STATUS_REPLY_OK, reply->len) == 0)
            ret = true;
    }
    else
        throw_redis_exception(reply);

    return ret;
}

string recv_status_reply(redisReply* reply)
{
    string ret;
    ReplyGuard guard(reply);

    if (reply->type == REDIS_REPLY_STATUS)
        ret.assign(reply->str, reply->len);
    else
        throw_redis_exception(reply);

    return ret;
}

int recv_int_reply(redisReply* reply)
{
    int ret;
    ReplyGuard guard(reply);

    if (reply->type == REDIS_REPLY_INTEGER)
        ret = reply->integer;
    else
        throw_redis_exception(reply);

    return ret;
}

float recv_float_reply(redisReply* reply)
{
    float ret;
    ReplyGuard guard(reply);

    string str;
    if (reply->type == REDIS_REPLY_STRING)
        ret = atof(reply->str);
    else
        throw_redis_exception(reply);

    return ret;
}

int recv_string_reply(redisReply* reply, string& str)
{
    int ret = 0;
    ReplyGuard guard(reply);

    if (reply->type == REDIS_REPLY_STRING)
        str.assign(reply->str, reply->len);
    else
        ret = -1;

    return ret;
}

int recv_string_array_reply(redisReply* reply, string_array& arr)
{
    int elements = 0;
    arr.clear();
    ReplyGuard guard(reply);

    if (reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements > 0)
        {
            arr.reserve(reply->elements);
            for (size_t i = 0; i < reply->elements; ++i)
            {
                redisReply* elem = reply->element[i];
                if (elem->type == REDIS_REPLY_STRING)
                {
                    arr.push_back(string(elem->str, elem->len));
                    elements++;
                }
            }
        }
    }
    else
        throw_redis_exception(reply);

    return elements;
}

int recv_string_set_reply(redisReply* reply, string_set& s)
{
    int elements = 0;
    s.clear();
    ReplyGuard guard(reply);

    if (reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements > 0)
        {
            for (size_t i = 0; i < reply->elements; ++i)
            {
                redisReply* elem = reply->element[i];
                if (elem->type == REDIS_REPLY_STRING)
                {
                    s.insert(string(elem->str, elem->len));
                    elements++;
                }
            }
        }
    }
    else
        throw_redis_exception(reply);

    return elements;
}

int recv_string_map_reply(redisReply* reply, const string_array& keys, 
    string_map& m)
{
    int elements = 0;
    ReplyGuard guard(reply);

    m.clear();
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements > 0)
        {
            for (size_t i = 0; i < reply->elements; ++i)
            {
                redisReply* elem = reply->element[i];
                if (elem->type == REDIS_REPLY_STRING)
                {
                    const string& key = keys[i];
                    m[key] = string(elem->str, elem->len);
                    elements++;
                }
                else if (elem->type == REDIS_REPLY_NIL)
                {
                    // log keys[i] get failed
                }
            }
        }
    }
    else
        throw_redis_exception(reply);

    return elements;
}

int recv_string_hash_map_reply(redisReply* reply, string_hash_map& h)
{
    int elements = 0;
    ReplyGuard guard(reply);

    h.clear();
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements > 0)
        {
            h.reserve(reply->elements / 2);
            for (size_t i = 0; i < reply->elements; ++i)
            {
                redisReply* filed = reply->element[i++];
                redisReply* value = reply->element[i];
                if ((filed->type == REDIS_REPLY_STRING) && (value->type == REDIS_REPLY_STRING))
                {
                    h[string(filed->str, filed->len)] = string(value->str, value->len);
                    elements++;
                }
                else
                    throw_redis_exception(reply);
            }
        }
    }
    else
        throw_redis_exception(reply);

    return elements;
}

int recv_string_hash_map_reply(redisReply* reply, const string_array& fields, 
    string_hash_map& h)
{
    int elements = 0;
    ReplyGuard guard(reply);

    h.clear();
    if (reply->type == REDIS_REPLY_ARRAY)
    {
        if (reply->elements > 0)
        {
            for (size_t i = 0; i < reply->elements; ++i)
            {
                redisReply* elem = reply->element[i];
                if (elem->type == REDIS_REPLY_STRING)
                {
                    const string& field = fields[i];
                    h[field] = string(elem->str, elem->len);
                    elements++;
                }
                else if (elem->type == REDIS_REPLY_NIL)
                {
                    // log fields[i] get failed
                }
            }
        }
    }
    else
        throw_redis_exception(reply);

    return elements;
}

}
