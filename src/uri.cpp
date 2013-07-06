#include "uri.hpp"
#include <cstring>
#include <algorithm>

namespace redis {

#define REDIS_SCHEME "redis"	

/*
 * <scheme>://<netloc>/<path>;<params>?<query>#<fragment>
 */
bool Uri::parse(const string& url, Uri& uri)
{
	// check redis scheme
	size_t pos = url.find("://");
	if (pos == string::npos)
		return false;

	string scheme = url.substr(0, pos);
	if (strcasecmp(REDIS_SCHEME, scheme.c_str()) != 0)
		return false;

	// get netloc
	string remain = url.substr(pos + 3);
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
		uri.password = userinfo.substr(pos + 1);
	}

	// get host port
	pos = netloc.find(':');
	if (pos == string::npos)
		return false;
	uri.host = netloc.substr(0, pos);
	uri.port = atoi(netloc.substr(pos + 1).c_str());

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
			uri.db = 0;
		else
			uri.db = atoi(path.c_str());
	}

	return true;
}

}
