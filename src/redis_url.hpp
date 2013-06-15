#ifndef REDIS_URL_HPP
#define REDIS_URL_HPP

#include <string>
using namespace std;

namespace redis {

/*
 * We only support the following redis:// scheme
 * redis://username:password@host:port/db
 */
struct url {
	url()
		: scheme(""), username(""), password(""), host(""), port(-1), db(-1)
	{
	}

	string scheme;
	string username;
	string password;
	string host;
	int    port;
	int    db;
};

url parse_url(const string& in);

}

#endif
