#include "connection_pool.hpp"

namespace redis {

ConnectionPool::ConnectionPool(const std::string& host, const int port, const int db,
        const int size, const int max_size)
    : host_(host), port_(port), db_(db), size_(size), max_size_(max_size)
{
}

ConnectionPool::~ConnectionPool()
{
    while (!connq_.empty())
    {
        Connection* conn = connq_.front();
        connq_.pop();
        delete conn;
    }
}

int ConnectionPool::initialize()
{
    int ret = 0;
    for (int i = 0; i < size_; i++)
    {
        Connection* conn = new Connection();
        if (conn->connect(host_, port_))
        {
            conn->select(db_);
            connq_.push(conn);
        }
        else
        {
            ret = -1;
            break;
        }
    }

    return ret;
}

Connection* ConnectionPool::acquire()
{
    lock_guard<mutex> lock(mtx_);
    if (connq_.empty())
    {
        if (size_ < max_size_)
        {
            Connection* conn = new Connection();
            if (conn->connect(host_, port_))
            {
                conn->select(db_);
                connq_.push(conn);
                ++size_;
            }
            else
                return NULL;
        }
        else
            return NULL;
    }

    Connection* conn = connq_.front();
    connq_.pop();
    return conn;
}

void ConnectionPool::release(Connection* conn)
{
    lock_guard<mutex> lock(mtx_);
    connq_.push(conn);
}

}
