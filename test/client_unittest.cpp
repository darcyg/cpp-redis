#include <gtest/gtest.h>
#include "redis_client.hpp"

TEST(ClientTest, connect)
{
	redis::client rc;
	EXPECT_EQ(0, rc.connect_with_url("redis://foo:bar@127.0.0.1:6379/10"));
	rc.close();
}