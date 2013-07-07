#ifndef CONNECYION_POOL_HPP
#define CONNECYION_POOL_HPP

#include <queue>
#include <mutex>
#include "connection.hpp"
using namespace std;

namespace redis {

class ConnectionPool {
public:
    ConnectionPool(const string& host, const int port, const int db,
            const int size = 5, const int max_size = 10);
    ~ConnectionPool();

    int initialize();

    Connection* acquire();
    void release(Connection* conn);

private:
    queue<Connection*> connq_;

    string host_;
    int port_;
    int db_;

    int size_;
    int max_size_;

    mutex mtx_;
};

class ConnectionGuard {
public:
    ConnectionGuard(ConnectionPool* pool)
        : pool_(pool)
    {
        conn_ = pool_->acquire();
    }

    ~ConnectionGuard()
    {
        pool_->release(conn_);
    }

    Connection* get_connection()
    {
        return conn_;
    }

private:
    Connection* conn_;
    ConnectionPool* pool_;
};

}

#endif
