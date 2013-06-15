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
	url() : password(""), host(""), port(6379), db(0) {}
	
	string password;
	string host;
	int    port;
	int    db;
};

bool parse_url(const string& in, url& out);

}

#endif
