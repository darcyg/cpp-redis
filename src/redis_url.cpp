#include "redis_url.hpp"
#include <cstring>
#include <algorithm>

namespace redis {

#define REDIS_SCHEME "redis"	

/*
 * <scheme>://<netloc>/<path>;<params>?<query>#<fragment>
 */
bool parse_url(const string& in, url& u)
{
	// check redis scheme
	size_t pos = in.find("://");
	if (pos == string::npos)
		return false;

	string scheme = in.substr(0, pos);
	if (strcasecmp(REDIS_SCHEME, scheme.c_str()) != 0)
		return false;

	// get netloc
	string remain = in.substr(pos + 3);
	string netloc;
	pos = remain.find('/');
	if (pos == string::npos)
	{
		netloc = remain;
		remain = "";
	}
	else
	{
		netloc = remain.substr(0, pos);
		remain = remain.substr(pos + 1);
	}

	// get username & password from netloc
	string host;
	pos = netloc.find('@');
	if (pos != string::npos)
	{
		string userinfo = netloc.substr(0, pos);
		netloc.erase(0, pos + 1);
		pos = userinfo.find(':');
		if (pos == string::npos)
			return false;
		u.password = userinfo.substr(pos + 1);
	}

	// get host port
	pos = netloc.find(':');
	if (pos == string::npos)
		return false;
	u.host = netloc.substr(0, pos);
	u.port = atoi(netloc.substr(pos + 1).c_str());

	// get db
	if (!remain.empty())
	{
		pos = remain.find('?');
		string path;
		if (pos == string::npos)
			path = remain;
		else
			path = remain.substr(0, pos);

		if (path.empty())
			u.db = 0;
		else
			u.db = atoi(path.c_str());
	}

	return true;
}

}
