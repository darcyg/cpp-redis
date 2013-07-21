#ifndef PUBSUB_HPP
#define PUBSUB_HPP

#include "async_connection.hpp"
#include "types.hpp"

namespace redis {

class Listener {
public:
    virtual void on_message(const string& channel, const string& message) {}
    virtual void on_pmessage(const string& pattern, const string& channel, const string& message) {}
};

class Subscriber {
public:
    int connect(const string& url);
    int reconnect();

    void set_listener(Listener* listener);

    void on_subscribe(const string& channel, const int subscribed_channels);
    void on_unsubscribe(const string& channel, const int subscribed_channels);
    void on_psubscribe(const string& pattern, const int subscribed_patterns);
    void on_punsubscribe(const string& pattern, const int subscribed_patterns);

    void on_message(const string& channel, const string& message);
    void on_pmessage(const string& pattern,  const string& channel, const string& message);

    template<typename ... Args>
    void subscribe(const Args & ... args)
    {
        string_array channels = { args... };
        MakeCmd cmd("SUBSCRIBE");
        cmd << channels;
        async_conn_->send_command(cmd, sub_callback, this);
    }

    template<typename ... Args>
    void unsubscribe(const Args & ... args)
    {
        string_array channels = { args... };
        MakeCmd cmd("UNSUBSCRIBE");
        cmd << channels;
        async_conn_->send_command(cmd, sub_callback, this);
    }

    template<typename ... Args>
    void psubscribe(const Args & ... args)
    {
        string_array patterns = { args... };
        MakeCmd cmd("PSUBSCRIBE");
        cmd << patterns;
        async_conn_->send_command(cmd, sub_callback, this);
    }

    template<typename ... Args>
    void punsubscribe(const Args & ... args)
    {
        string_array patterns = { args... };
        MakeCmd cmd("PUNSUBSCRIBE");
        cmd << patterns;
        async_conn_->send_command(cmd, sub_callback, this);
    }

public:
    static void sub_callback(redisAsyncContext *ac, void* reply, void* privatedata);

private:
    AsyncConnection* async_conn_;
    Listener* listener_;
    string_set channels_;
    string_set patterns_;
};

}

#endif
