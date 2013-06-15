#include <gtest/gtest.h>
#include "redis_url.hpp"

TEST(UrlTest, urlparse)
{
	redis::url redis_url = redis::parse_url("redis://foo:bar@localhost:6379/10");
	EXPECT_EQ("redis", redis_url.scheme);
	EXPECT_EQ("foo", redis_url.username);
	EXPECT_EQ("bar", redis_url.password);
	EXPECT_EQ("localhost", redis_url.host);
	EXPECT_EQ(6379, redis_url.port);
	EXPECT_EQ(10, redis_url.db);
}