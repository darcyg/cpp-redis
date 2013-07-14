#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <hiredis/hiredis.h>
#include "command.hpp"

using namespace std;

namespace redis {

class Connection {
public:
    Connection(const string& host, const int port, const int db);
    Connection(const string& url);

    virtual ~Connection();

    bool connect(const int timeout = 0);
    void disconnect();

    bool ping();
    bool select(const int db);
    bool auth(const string& password);
    bool quit();

    redisReply* send_command(const RedisCmd& cmd);

private:
    redisContext* context_;
    string host_;
    int port_;
    int db_;
};

}

#endif
