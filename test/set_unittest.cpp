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
	EXPECT_EQ(3, rc_.sadd("skey", "value1", "value2", "value3"));
	redis::string_set members;
	EXPECT_EQ(3, rc_.smembers("skey", members));
	EXPECT_EQ((size_t)1, members.count("value1"));
	rc_.del("skey");
}

TEST_F(SetTest, sdiff)
{
	EXPECT_EQ(4, rc_.sadd("key1", "a", "b", "c", "d"));
	EXPECT_EQ(1, rc_.sadd("key2", "c"));
	EXPECT_EQ(3, rc_.sadd("key3", "a", "c", "e"));

	redis::string_set members;
	EXPECT_EQ(2, rc_.sdiff(members, "key1", "key2", "key3"));
	EXPECT_EQ((size_t)1, members.count("b"));
	EXPECT_EQ((size_t)1, members.count("d"));

	redis::string_array keys;
	keys.push_back("key1");
	keys.push_back("key2");
	keys.push_back("key3");
	EXPECT_EQ(2, rc_.sdiff(keys, members));

	rc_.del("key1", "key2", "key3");
}