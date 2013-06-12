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
	rc_.del("skey");
}

TEST_F(SetTest, smember)
{
	EXPECT_EQ(3, rc_.sadd("skey", 3, "value1", "value2", "value3"));
	redis::string_set members;
	EXPECT_EQ(3, rc_.smembers("skey", members));
	EXPECT_EQ((size_t)1, members.count("value1"));
	rc_.del("skey");
}