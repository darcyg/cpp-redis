#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <hiredis/hiredis.h>
#include "command.hpp"

using namespace std;

namespace redis {

class Connection {
public:
    Connection(const string& host, const int port, const int db);
    virtual ~Connection();

    // hope this function will be never used.
    redisContext* get_handle() const;

    bool connect();
    void disconnect();
    bool ping();

    redisReply* send_command(const RedisCmd& cmd);

private:
    bool auth(const string& password);

    bool select(const int db);

    bool quit();

private:
    redisContext* context_;
    string host_;
    int port_;
    int db_;
};

}

#endif
