#ifndef REDIS_EXCEPTION_H
#define REDIS_EXCEPTION_H

#include <exception>
#include <string>
using namespace std;

namespace redis {

class redis_exception : public exception {
public:
	redis_exception(const string & err) throw ()
		: err_(err)
	{
	}

	virtual ~redis_exception() throw ()
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
class connection_exception : public redis_exception
{
public:
	connection_exception(const string & err) : redis_exception(err) {}
};

// Bad command
class command_exception : public redis_exception
{
public:
	command_exception(const string & err) : redis_exception(err) {}
};

// A value of an expected type or other semantics was found to be invalid.
class value_exception : public redis_exception
{
public:
	value_exception(const string & err) : redis_exception(err) {};
};

}

#endif
