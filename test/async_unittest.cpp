#include <gtest/gtest.h>
#include <unistd.h>
#include "async_connection.hpp"
#include "async_service.hpp"
#include "command.hpp"

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
    redis::AsyncService::instance().start();
    redis::AsyncConnection conn;
    EXPECT_EQ(0, conn.connect("redis://localhost:6379"));

    redis::MakeCmd cmd("PING");
    conn.send_command(cmd, ping_callback, NULL);

    redis::AsyncService::instance().stop();
}
