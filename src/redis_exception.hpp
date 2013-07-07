#ifndef REDIS_EXCEPTION_H
#define REDIS_EXCEPTION_H

#include <exception>
#include <string>
using namespace std;

namespace redis {

class RedisException : public exception {
public:
    RedisException(const string & err) throw ()
        : err_(err)
    {
    }

    virtual ~RedisException() throw ()
    {
    }

    operator const string() const throw ()
    {
        return err_;
    }

    virtual const char* what() const throw ()
    {
        return err_.c_str();
    }

private:
    string err_;
};

// Some socket-level I/O or general connection error.
class ConnectionException : public RedisException
{
public:
    ConnectionException(const string & err) : RedisException(err) {}
};

// Bad command
class CommandException : public RedisException
{
public:
    CommandException(const string & err) : RedisException(err) {}
};

// A value of an expected type or other semantics was found to be invalid.
class ValueException : public RedisException
{
public:
    ValueException(const string & err) : RedisException(err) {};
};

}

#endif
