#include "redis_url.hpp"
#include <cstring>
#include <algorithm>

namespace redis {

#define REDIS_SCHEME "redis"

/*
 * <scheme>://<netloc>/<path>;<params>?<query>#<fragment>
 */
url parse_url(const string& in)
{
	url ret;

	do {
		string remain = in;
		// get scheme
		size_t pos = remain.find("://");
		if (pos == string::npos)
			break;
		ret.scheme = remain.substr(0, pos);
		if (strcasecmp(REDIS_SCHEME, ret.scheme.c_str()) != 0)
			break;

		// get netloc
		remain.erase(0, pos + 3);
		pos = remain.find('/');
		if (pos == string::npos)
			break;
		string netloc = remain.substr(0, pos);

		// get path
		remain.erase(0, pos + 1);
		pos = remain.find('?');
		string path;
		if (pos == string::npos)
			path = remain;
		else
			path = remain.substr(0, pos);
		ret.db = atoi(path.c_str());

		// get username & password from netloc
		pos = netloc.find(':');
		if (pos == string::npos)
			break;
		ret.username = netloc.substr(0, pos);

		netloc.erase(0, pos + 1);
		pos = netloc.find('@');
		if (pos == string::npos)
			break;
		ret.password = netloc.substr(0, pos);

		netloc.erase(0, pos + 1);
		pos = netloc.find(':');
		if (pos == string::npos)
			break;
		ret.host = netloc.substr(0, pos);
		ret.port = atoi(netloc.substr(pos + 1).c_str());
	} while (0);

	return ret;
}

}
