#include "pipeline.hpp"

namespace redis {

void Pipeline::execute()
{
    conn_->flush();
}

Pipeline& Pipeline::set(const string& key, const string& value)
{
    MakeCmd cmd("SET");
    cmd << key << value;
    conn_->append_command(cmd);
    return *this;
}

Pipeline& Pipeline::get(const string& key)
{
    MakeCmd cmd("GET");
    cmd << key << value;
    conn_->append_command(cmd);
    return *this;
}

Pipeline& Pipeline::incr(const string& key)
{
    MakeCmd cmd("INCR");
    cmd << key;
    conn_->append_command(cmd);
    return *this;
}

}
