#include <iostream>

#include "async_service.hpp"

namespace redis {

async_service::async_service()
{
	base_ = event_base_new();
	event_thread_ = NULL;
}

async_service::~async_service()
{
}

async_service& async_service::instance()
{
	static async_service obj;
	return obj;
}

void async_service::attach(redisAsyncContext* ac)
{
	redisLibeventAttach(ac, base_);
}

void async_service::start()
{
	event_thread_ = new thread(event_base_loop, base_, EVLOOP_NO_EXIT_ON_EMPTY);
}

void async_service::stop()
{
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec  = 0;
	event_base_loopexit(base_, &tv);

	event_thread_->join();
}

}