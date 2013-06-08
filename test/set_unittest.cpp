#include "redis_test.hpp"

class SetTest : public RedisTest {
};

TEST_F(SetTest, sadd)
{
	EXPECT_EQ(1, rc_.sadd("skey", "value1"));
	EXPECT_EQ(1, rc_.sadd("skey", "value2"));
	EXPECT_EQ(2, rc_.scard("skey"));
	EXPECT_EQ(1, rc_.sismember("skey", "value1"));
	EXPECT_EQ(1, rc_.srem("skey", "value1"));
	EXPECT_EQ(1, rc_.srem("skey", "value2"));
}