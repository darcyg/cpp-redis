#include <iostream>
#include "async_service.hpp"

namespace redis {

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
    event_thread_ = new thread(event_base_loop, base_, EVLOOP_NO_EXIT_ON_EMPTY);
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
