#include <cstring>
#include <iostream>
#include "types.hpp"
#include "reply.hpp"
#include "command.hpp"
#include "sentinel.hpp"
#include "redis_exception.hpp"

using namespace std;

namespace redis {

const int CONNECT_TIMEOUT_MILLISECONDS = 2000;
const string SENTINEL_PATTERN = "__sentinel__:*";

class SentinelListener : public Listener {
public:
    virtual void on_message(const string& channel, const string& message);
    virtual void on_pmessage(const string& pattern, const string& channel, const string& message);
};

void SentinelListener::on_message(const string& channel, const string& message)
{
    cout << channel << ' ' << message << endl;
}

void SentinelListener::on_pmessage(const string& pattern, const string& channel, const string& message)
{
    cout << pattern << ' ' << channel << ' ' << message << endl;
}

SentinelClient::SentinelClient(const vector<string>& urls, const string& master_name)
    : urls_(urls), master_name_(master_name), master_ip_(""), master_port_(0)
{
}

bool SentinelClient::connect()
{
    bool ret = false;
    for (vector<string>::iterator it = urls_.begin(); it != urls_.end(); ++it)
    {
        const string& url = *it;
        shared_ptr<Connection> conn(new Connection(url));
        if (conn->connect(CONNECT_TIMEOUT_MILLISECONDS))
        {
            if (discover_master(conn))
            {
                ret = true;

                // give priority to the replying Sentinel
                urls_.erase(it);
                urls_.insert(urls_.begin(), url);

                // subscribe sentinel notification
                subscribe_notification(url);
                break;
            }
        }
    }

    return ret;
}

bool SentinelClient::discover_master(shared_ptr<Connection> conn)
{
    MakeCmd cmd("SENTINEL");
    cmd << "get-master-addr-by-name" << master_name_;
    redisReply* reply(conn->send_command(cmd));

    string_array address;
    try {
        int ret = recv_string_array_reply(reply, address);
        master_ip_ = address[0];
        master_port_ = atoi(address[0].c_str());
        cout << master_ip_ << ' ' << master_port_ << endl;
    } catch (const RedisException& e) {
        return false;
    }
    
    return true;
}

void SentinelClient::subscribe_notification(const string& url)
{
    sentinel_subscriber_.set_listener(new SentinelListener());
    sentinel_subscriber_.connect(url);
    sentinel_subscriber_.psubscribe(SENTINEL_PATTERN);
}

}
