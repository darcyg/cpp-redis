#include "redis_test.hpp"
#include "redis_exception.hpp"

class StringTest : public RedisTest {
};

TEST_F(StringTest, setget)
{
	EXPECT_TRUE(rc_.set("testkey1", "testvalue1"));

	string val;
	EXPECT_EQ(0, rc_.get("testkey1", val));
	EXPECT_EQ("testvalue1", val);

	EXPECT_EQ(1, rc_.del("testkey1"));
}

TEST_F(StringTest, mgetmset)
{
	vector<string> keys;
	redis::string_map kv_map;

	EXPECT_TRUE(rc_.mset(2, "testkey1", "testvalue1", "testkey2", "testvalue2"));
	EXPECT_EQ(2, rc_.mget(kv_map, 2, "testkey1", "testkey2"));

	kv_map["testkey3"] = "testvalue3";
	kv_map["testkey4"] = "testvalue4";
	kv_map["testkey5"] = "testvalue5";
	EXPECT_TRUE(rc_.mset(kv_map));

	keys.clear();
	kv_map.clear();
	keys.push_back("testkey1");
	keys.push_back("testkey2");
	keys.push_back("testkey5");
	EXPECT_EQ(3, rc_.mget(keys, kv_map));

	EXPECT_EQ(2, rc_.del(2, "testkey1", "testkey2"));
}

TEST_F(StringTest, incr)
{
	rc_.set("testkey1", "10.0");
	EXPECT_FLOAT_EQ(10.5, rc_.incrbyfloat("testkey1", 0.5));
	EXPECT_FLOAT_EQ(9.5, rc_.incrbyfloat("testkey1", -1.0));
	rc_.del("testkey1");

	rc_.set("testkey1", "foo");
	try {
		rc_.incrbyfloat("testkey1", 0.1);
	} catch (const redis::value_exception& e) {
		EXPECT_TRUE(true);
	} catch (const exception& ex) {
		EXPECT_TRUE(false);
	}
}