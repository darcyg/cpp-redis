#include <gtest/gtest.h>
#include "async_service.hpp"
#include "sentinel.hpp"

TEST(SentinelTest, testconnect)
{
	vector<string> urls = {"redis://localhost:26379", "redis://localhost:26380",
		"redis://localhost:26381", "redis://localhost:26382"};

	redis::AsyncService::instance().start();

	redis::SentinelClient client(urls, "mymaster");
	EXPECT_TRUE(client.connect());

	sleep(600);

	redis::AsyncService::instance().stop();
}