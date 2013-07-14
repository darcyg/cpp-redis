#ifndef URI_HPP
#define URI_HPP

#include <string>
using namespace std;

namespace redis {

/*
 * We only support the following redis:// scheme
 * redis://username:password@host:port/db
 */
struct URI {
    URI() : password(""), host(""), port(6379), db(0) {}

    static bool parse(const string& url, URI& uri);
    
    string password;
    string host;
    int    port;
    int    db;
};

}

#endif
