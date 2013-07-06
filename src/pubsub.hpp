#ifndef PUBSUB_HPP
#define PUBSUB_HPP

#include "async_connection.hpp"
#include "types.hpp"

namespace redis {

class Publisher {
public:
    int connect(const string& url);

    int publish(const string& channel, const string& message);

private:
    async_connection* async_conn_;
};

class Listener {
public:
    virtual void on_message(const string& channel, const string& message) = 0;
    virtual void on_pmessage(const string& pattern, const string& message) = 0;
};

class Subscriber {
public:
    int connect(const string& url);

    void set_listener(Listener* listener)
    {
        listener_ = listener;
    }

    template<typename ... Args>
    void subscribe(const Args & ... args)
    {
        string_array channels = { args... };
        makecmd cmd("SUBSCRIBE");
        cmd << channels;
        async_conn_->send_command(cmd, subscribe_callback, listener_);
    }

    template<typename ... Args>
    void psubscribe(const Args & ... args)
    {
        string_array patterns = { args... };
        makecmd cmd("SUBSCRIBE");
        cmd << patterns;
        async_conn_->send_command(cmd, psubscribe_callback, listener_);
    }

    template<typename ... Args>
    void unsubscribe(const Args & ... args)
    {
        string_array channels = { args... };
        makecmd cmd("UNSUBSCRIBE");
        cmd << channels;
        async_conn_->send_command(cmd, unsubscribe_callback, this);
    }

    template<typename ... Args>
    void punsubscribe(const Args & ... args)
    {
        string_array patterns = { args... };
        makecmd cmd("PUNSUBSCRIBE");
        cmd << patterns;
        async_conn_->send_command(cmd, punsubscribe_callback, this);
    }

public:
    static void subscribe_callback(redisAsyncContext *ac, void* reply, void* privatedata);
    static void unsubscribe_callback(redisAsyncContext *ac, void* reply, void* privatedata);
    static void psubscribe_callback(redisAsyncContext *ac, void* reply, void* privatedata);
    static void punsubscribe_callback(redisAsyncContext *ac, void* reply, void* privatedata);

private:
    async_connection* async_conn_;
    Listener* listener_;
};

}

#endif