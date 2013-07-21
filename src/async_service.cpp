#include <iostream>
#include "async_service.hpp"

namespace redis {

static void donothing_cb(evutil_socket_t fd, short event, void *arg)
{
}

AsyncService::AsyncService()
{
    base_ = event_base_new();
    event_thread_ = NULL;
}

AsyncService::~AsyncService()
{
    delete event_thread_;
}

AsyncService& AsyncService::instance()
{
    static AsyncService obj;
    return obj;
}

void AsyncService::attach(redisAsyncContext* ac)
{
    redisLibeventAttach(ac, base_);
}

void AsyncService::start()
{
    struct event* timeout = event_new(base_, -1, EV_PERSIST, donothing_cb, NULL);
    struct timeval tv;
    evutil_timerclear(&tv);
    tv.tv_sec = 3600;
    evtimer_add(timeout, &tv);
    event_thread_ = new thread(event_base_dispatch, base_);
}

void AsyncService::stop()
{
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec  = 0;
    event_base_loopexit(base_, &tv);

    event_thread_->join();
}

}
