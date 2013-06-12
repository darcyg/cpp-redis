#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <utility>
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace redis {

	#define REDIS_STATUS_REPLY_OK "OK"
	#define REDIS_STATUS_REPLY_PONG "PONG"

	typedef vector<string> string_array;

	typedef pair<string, string> string_pair;

	typedef vector<string_pair> string_pair_vector;

	typedef map<string, string> string_key_value_map;
}

#endif
