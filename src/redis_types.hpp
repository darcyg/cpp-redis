#ifndef REDIS_TYPES_HPP
#define REDIS_TYPES_HPP

#include <string>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <array>
#include <cstdarg>
using namespace std;

namespace redis {

	#define REDIS_STATUS_REPLY_OK "OK"
	#define REDIS_STATUS_REPLY_PONG "PONG"

	enum BitOp {AND = 0, OR, XOR, NOT, BitOp_Num};
	
	typedef vector<string> string_array;
	typedef map<string, string> string_map;
	typedef set<string> string_set;
	typedef unordered_map<string, string> string_hash_map;
}

#endif
