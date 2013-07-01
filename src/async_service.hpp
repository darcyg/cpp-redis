#ifndef ASYNC_SERVICE_HPP
#define ASYNC_SERVICE_HPP

#include <thread>
#include <hiredis/adapters/libevent.h>

using namespace std;

namespace redis {

class async_service {
public:
	async_service();
	~async_service();

	static async_service& instance();

	void start();
	void stop();

	void attach(redisAsyncContext* ac);

private:
	thread* event_thread_;
	event_base* base_;
};

}

#endif