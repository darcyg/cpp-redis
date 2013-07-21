#include <gtest/gtest.h>
#include "redis_client.hpp"

TEST(ClientTest, connect)
{
	redis::Client rc;
	EXPECT_EQ(0, rc.connect("redis://foo:bar@127.0.0.1:6379/10"));
	rc.close();
}