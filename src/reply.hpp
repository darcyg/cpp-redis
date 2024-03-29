#ifndef REDIS_REPLY_HPP
#define REDIS_REPLY_HPP

#include <hiredis/hiredis.h>
#include "types.hpp"

namespace redis {

    bool recv_ok_reply(redisReply* reply);

    string recv_status_reply(redisReply* reply);

    int recv_int_reply(redisReply* reply);

    float recv_float_reply(redisReply* reply);

    int recv_string_reply(redisReply* reply, string& str);

    int recv_string_array_reply(redisReply* reply, string_array& arr);

    int recv_string_set_reply(redisReply* reply, string_set& s);

    int recv_string_map_reply(redisReply* reply, const string_array& keys, 
        string_map& m);

    int recv_string_hash_map_reply(redisReply* reply, string_hash_map& h);

    int recv_string_hash_map_reply(redisReply* reply, const string_array& fields, 
        string_hash_map& h);
}

#endif
