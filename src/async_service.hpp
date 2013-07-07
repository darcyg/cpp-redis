#ifndef ASYNC_SERVICE_HPP
#define ASYNC_SERVICE_HPP

#include <thread>
#include <hiredis/adapters/libevent.h>

using namespace std;

namespace redis {

class AsyncService {
public:
    AsyncService();
    ~AsyncService();

    static AsyncService& instance();

    void start();
    void stop();

    void attach(redisAsyncContext* ac);

private:
    thread* event_thread_;
    event_base* base_;
};

}

#endif
