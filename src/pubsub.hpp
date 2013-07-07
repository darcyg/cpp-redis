#ifndef PUBSUB_HPP
#define PUBSUB_HPP

#include "async_connection.hpp"
#include "types.hpp"

namespace redis {

class Listener {
public:
    virtual void on_message(const string& channel, const string& message) = 0;
    virtual void on_pmessage(const string& pattern, const string& channel, const string& message) = 0;
};

class Subscriber {
public:
    int connect(const string& url);
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
        makecmd cmd("SUBSCRIBE");
        cmd << channels;
        async_conn_->send_command(cmd, sub_callback, this);
    }

    template<typename ... Args>
    void unsubscribe(const Args & ... args)
    {
        string_array channels = { args... };
        makecmd cmd("UNSUBSCRIBE");
        cmd << channels;
        async_conn_->send_command(cmd, sub_callback, this);
    }

    template<typename ... Args>
    void psubscribe(const Args & ... args)
    {
        string_array patterns = { args... };
        makecmd cmd("SUBSCRIBE");
        cmd << patterns;
        async_conn_->send_command(cmd, sub_callback, this);
    }

    template<typename ... Args>
    void punsubscribe(const Args & ... args)
    {
        string_array patterns = { args... };
        makecmd cmd("PUNSUBSCRIBE");
        cmd << patterns;
        async_conn_->send_command(cmd, sub_callback, this);
    }

public:
    static void sub_callback(redisAsyncContext *ac, void* reply, void* privatedata);

private:
    async_connection* async_conn_;
    string_array channels_;
    string_array patterns_;
    Listener* listener_;
};

}

#endif