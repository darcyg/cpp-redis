#ifndef SENTINEL_HPP
#define SENTINEL_HPP

#include <vector>
#include <memory>
#include "connection.hpp"
#include "pubsub.hpp"

using namespace std;

namespace redis {

class SentinelClient {
public:
    SentinelClient(const vector<string>& urls, const string& master_name);
    bool connect();
    
private:
    bool discover_master(shared_ptr<Connection> conn);
    void subscribe_notification(const string& url);

private:
    vector<string> urls_;
    string master_name_;

    string master_ip_;
    int master_port_;

    Subscriber sentinel_subscriber_;
};

}   

#endif
