#include <gtest/gtest.h>
#include "redis_client.hpp"
#include "async_service.hpp"

TEST(SentinelTest, testconnect)
{
    redis::AsyncService::instance().start();

    vector<string> urls = {"redis://localhost:26379", "redis://localhost:26380",
        "redis://localhost:26381", "redis://localhost:26382"};

    redis::Client client;
    client.connect(urls, "mymaster");

    sleep(100);

    redis::AsyncService::instance().stop();
}
