#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <hiredis/hiredis.h>
#include "command.hpp"

using namespace std;

namespace redis {

class Connection {
public:
    Connection();
    ~Connection();

    bool connect(const string& host, const int port);
    bool connect(const string& url);
    void disconnect();

    bool ping();
    bool select(const int db);
    bool auth(const string& password);
    bool quit();

    redisReply* send_command(const RedisCmd& cmd);

private:
    redisContext* context_;
};

}

#endif
