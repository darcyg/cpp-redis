#include "redis_test.hpp"

class KeysTest : public RedisTest {
};

TEST_F(KeysTest, smember)
{
	rc_.set("testkey1", "testvalue1");
	EXPECT_EQ("string", rc_.type("testkey1"));

	rc_.sadd("testkey2", "testvalue2");
	EXPECT_EQ("set", rc_.type("testkey2"));

	EXPECT_EQ("none", rc_.type("testkey2notexistdeadbeaf"));

	rc_.del(2, "testkey1", "testkey2");
}
