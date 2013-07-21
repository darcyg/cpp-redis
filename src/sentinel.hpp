#ifndef SENTINEL_HPP
#define SENTINEL_HPP

#include <vector>
#include <memory>
#include "connection.hpp"
#include "pubsub.hpp"

using namespace std;

namespace redis {
class Client;

class Sentinel {
public:
    Sentinel(Client* client, const vector<string>& urls, const string& master_name, const int db = 0);

    bool start_client_connect();

    bool discover_master(pair<string, int>& addr, string& sentinel_url);
    bool get_master_address(Connection& conn, pair<string, int>& addr);
    void subscribe_sentinel_notification(const string& url);
    void start_monitor_timer();
    void check_master();

private:
    vector<string> urls_;
    string master_name_;

    string sentinel_url_;
    string master_ip_;
    int master_port_;
    int db_;

    Client* client_;
    Subscriber sentinel_subscriber_;
};

}   

#endif
