#include <gtest/gtest.h>
#include "redis_url.hpp"

TEST(UrlTest, urlparse)
{
	redis::url u1;
	EXPECT_TRUE(redis::parse_url("redis://foo:bar@localhost:6379/10", u1));
	EXPECT_EQ("bar", u1.password);
	EXPECT_EQ("localhost", u1.host);
	EXPECT_EQ(6379, u1.port);
	EXPECT_EQ(10, u1.db);

	redis::url u2;
	EXPECT_TRUE(redis::parse_url("redis://localhost:6379", u2));
	EXPECT_EQ("", u2.password);
	EXPECT_EQ("localhost", u2.host);
	EXPECT_EQ(6379, u2.port);
	EXPECT_EQ(0, u2.db);
}