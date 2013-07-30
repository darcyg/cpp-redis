#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <tuple>

namespace redis {

class Pipeline {
public:
    Pipeline();
    ~Pipeline();

    void execute();
    
    get_reply();

    Pipeline& set(const string& key, const string& value);

    Pipeline& get(const string& key);

    Pipeline& incr(const string& key);

private:
    void append();

private:
    Connection* conn_;
};

}

#endif
