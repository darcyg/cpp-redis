#include <gtest/gtest.h>
#include <unistd.h>
#include "async_connection.hpp"
#include "async_service.hpp"
#include "redis_command.hpp"

void ping_callback(redisAsyncContext *ac, void* reply, void* privatedata)
{
    (void)ac;
    (void)privatedata;

    redisReply* r = (redisReply*)(reply);
    cout << r->str << endl;
    return;
}

TEST(AsyncTest, connect)
{
	redis::async_service::instance().start();
	redis::async_connection conn;
	EXPECT_EQ(0, conn.connect("localhost"));

    redis::makecmd cmd("PING");
    conn.send_command(cmd, ping_callback, NULL);

	redis::async_service::instance().stop();
}
