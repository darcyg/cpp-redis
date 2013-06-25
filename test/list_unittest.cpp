#include "redis_test.hpp"

class ListTest : public RedisTest {
};

TEST_F(ListTest, poppush)
{
	EXPECT_EQ(2, rc_.lpush("lkey", "value1", "value2"));

	string first_value;
	rc_.lpop("lkey", first_value);
	EXPECT_EQ("value2", first_value);
	rc_.lpop("lkey", first_value);
	EXPECT_EQ("value1", first_value);

	rc_.lpush("lkey", "value0");
	EXPECT_EQ(2, rc_.lpushx("lkey", "value1"));
	EXPECT_EQ(3, rc_.lpushx("lkey", "value2"));
	EXPECT_EQ(4, rc_.lpushx("lkey", "value2"));

	rc_.del("lkey");
}
