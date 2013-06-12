#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <string>
#include <vector>
#include <set>
#include <map>
#include <cstdarg>
using namespace std;

namespace redis {

	#define REDIS_STATUS_REPLY_OK "OK"
	#define REDIS_STATUS_REPLY_PONG "PONG"

	typedef vector<string> string_array;
	typedef map<string, string> string_map;
	typedef set<string> string_set;

	#define arguments_to_string_array(num, strs) \
		va_list ap; \
		va_start(ap, num); \
		for (int i = 0; i < num; ++i) \
		{ \
			string arg = va_arg(ap, char*); \
			strs.push_back(arg); \
		} \
		va_end(ap);

	#define pair_arguments_to_string_array(pair_num, strs) \
		const int num = pair_num * 2; \
		va_list ap; \
		va_start(ap, pair_num); \
		for (int i = 0; i < num; ++i) \
		{ \
			string arg = va_arg(ap, char*); \
			strs.push_back(arg); \
		} \
		va_end(ap);
}

#endif
