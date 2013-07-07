#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <sstream>
#include "types.hpp"
using namespace std;

namespace redis {

struct RedisCmd {
    RedisCmd() : argc(0), argv(NULL), argvlen(NULL) {}

    ~RedisCmd()
    {
        if (argv) delete [] argv;
        if (argvlen) delete [] argvlen;
    }

    void shallow_assign(const string_array& args)
    {
        argc = args.size();
        argv = new char*[argc];
        argvlen = new size_t[argc];
        for (int i = 0; i < argc; ++i)
        {
            const string& arg = args[i];
            argv[i] = (char*)arg.c_str();
            argvlen[i] = arg.size();
        }
    }

    int argc;
    char** argv;
    size_t* argvlen;
};

class MakeCmd {
public:
    explicit MakeCmd(const string& cmdname)
    {
        args_.push_back(cmdname);
    }

    ~MakeCmd()
    {
        args_.clear();
    }

    inline MakeCmd& operator << (const string& data)
    {
        args_.push_back(data);
        return *this;
    }

    inline MakeCmd& operator << (const int& data) 
    {
        ostringstream oss;
        oss << data;
        args_.push_back(oss.str());
        return *this;
    }

    inline MakeCmd& operator << (const unsigned int& data) 
    {
        ostringstream oss;
        oss << data;
        args_.push_back(oss.str());
        return *this;
    }

    inline MakeCmd& operator << (const float& data)
    {
        ostringstream oss;
        oss << data;
        args_.push_back(oss.str());
        return *this;
    }

    MakeCmd& operator << (const string_array& strs)
    {
        for (const string& s : strs)
            args_.push_back(s);
        return *this;
    }

    MakeCmd& operator << (const string_map& m)
    {
        for (auto it = m.begin(); it != m.end(); ++it)
        {
            args_.push_back(it->first);
            args_.push_back(it->second);
        }
        return *this;
    }

    MakeCmd& operator << (const string_hash_map& h)
    {
        for (auto it = h.begin(); it != h.end(); ++it)
        {
            args_.push_back(it->first);
            args_.push_back(it->second);
        }
        return *this;
    }

    operator const RedisCmd& ()
    {
        cmd_.shallow_assign(args_);
        return cmd_;
    }

private:
    string_array args_;
    RedisCmd cmd_;
};

}

#endif
