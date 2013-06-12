#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <string>
#include <vector>
#include <map>
using namespace std;

namespace redis {

	#define REDIS_STATUS_REPLY_OK "OK"
	#define REDIS_STATUS_REPLY_PONG "PONG"

	typedef vector<string> string_array;
	typedef map<string, string> string_map;
}

#endif
