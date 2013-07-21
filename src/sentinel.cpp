#include <cstring>
#include <iostream>
#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include "types.hpp"
#include "reply.hpp"
#include "command.hpp"
#include "sentinel.hpp"
#include "redis_exception.hpp"
#include "redis_client.hpp"
#include "async_service.hpp"

using namespace std;

namespace redis {

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

Sentinel::Sentinel(Client* client, const vector<string>& urls, const string& master_name, const int db)
    : client_(client), urls_(urls), master_name_(master_name), master_ip_(""), master_port_(0), db_(db)
{
}

bool Sentinel::start_client_connect()
{
    pair<string, int> addr;
    string url;

    if (discover_master(addr, url) == false)
        return false;

    if (client_->connect(addr.first, addr.second, db_) == 0)
    {
        master_ip_ = addr.first;
        master_port_ = addr.second;
        sentinel_url_ = url;

        subscribe_sentinel_notification(url);
        start_monitor_timer();
        return true;
    }

    return false;
}

bool Sentinel::discover_master(pair<string, int>& addr, string& url)
{
    bool succ = false;
    for (vector<string>::iterator it = urls_.begin(); it != urls_.end(); ++it)
    {
        url = *it;
        Connection conn;
        if (conn.connect(url))
        {
            if (get_master_address(conn, addr))
            {
                succ = true;
                // give priority to the replying Sentinel
                urls_.erase(it);
                urls_.insert(urls_.begin(), url);
                break;
            }
        }
    }

    return succ;
}

bool Sentinel::get_master_address(Connection& conn, pair<string, int>& addr)
{
    MakeCmd cmd("SENTINEL");
    cmd << "get-master-addr-by-name" << master_name_;
    redisReply* reply(conn.send_command(cmd));

    string_array address;
    try {
        int ret = recv_string_array_reply(reply, address);
        const string& master_ip = address[0];
        int master_port = atoi(address[1].c_str());
        cout << master_ip << ' ' << master_port << endl;
        addr = make_pair(master_ip, master_port);
    } catch (const RedisException& e) {
        return false;
    }
    
    return true;
}

void Sentinel::subscribe_sentinel_notification(const string& url)
{
    sentinel_subscriber_.set_listener(new SentinelListener());
    sentinel_subscriber_.connect(url);
    sentinel_subscriber_.psubscribe("*");
}

static void timeout_cb(evutil_socket_t fd, short event, void *arg)
{
    Sentinel* sentinel = (Sentinel*) arg;

    cout << "monitor master" << endl;
    sentinel->check_master();
}

void Sentinel::start_monitor_timer()
{
    event_base* base = AsyncService::instance().get_event_base();
    struct event* timeout = event_new(base, -1, EV_PERSIST, timeout_cb, this);
    struct timeval tv;
    evutil_timerclear(&tv);
    tv.tv_sec = 1;
    evtimer_add(timeout, &tv);
}

void Sentinel::check_master()
{
    pair<string, int> addr;
    string url;
    if (discover_master(addr, url) == false)
        cout << "failed to discover master" << endl;

    if (addr.first != master_ip_ || addr.second != master_port_)
    {
        cout << "master change, do reconnect" << endl;
        client_->connect(addr.first, addr.second, db_);
    }

    if (url != sentinel_url_)
    {
        cout << "re-subscribe sentinel pattern" << endl;
        subscribe_sentinel_notification(url);
    }
        
}

}
