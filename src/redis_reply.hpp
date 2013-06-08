#ifndef REDIS_REPLY_HPP
#define REDIS_REPLY_HPP

#include <hiredis/hiredis.h>
#include "redis_types.hpp"

namespace redis {

	bool recv_status_reply(redisReply* reply);

	int recv_int_reply(redisReply* reply);

	int recv_string_reply(redisReply* reply, string& str);

	int recv_string_array_reply(redisReply* reply, string_array& arr);

	int recv_string_pair_array_reply(redisReply* reply, const string_array& keys, 
		string_pair_vector& key_value_pairs);
}

#endif
