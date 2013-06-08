#include "redis_test.hpp"

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
	redis::string_pair_vector kv_pairs;
	kv_pairs.push_back(make_pair<string, string>("testkey1", "testvalue1"));
	kv_pairs.push_back(make_pair<string, string>("testkey2", "testvalue2"));
	EXPECT_TRUE(rc_.mset(kv_pairs));

	vector<string> keys;
	keys.push_back("testkey1");
	keys.push_back("testkey2");
	kv_pairs.clear();
	EXPECT_EQ(2, rc_.mget(keys, kv_pairs));

	pair<string, string>& kv1 = kv_pairs.front();
	EXPECT_EQ("testkey1", kv1.first);
	EXPECT_EQ("testvalue1", kv1.second);

	pair<string, string>& kv2 = kv_pairs.back();
	EXPECT_EQ("testkey2", kv2.first);
	EXPECT_EQ("testvalue2", kv2.second);

	kv_pairs.clear();

	EXPECT_TRUE(rc_.mset(2, "testkey3", "testvalue3", "testkey4", "testvalue4"));
	EXPECT_EQ(2, rc_.mget(kv_pairs, 2, "testkey3", "testkey4"));

	pair<string, string>& kv3 = kv_pairs.front();
	EXPECT_EQ("testkey3", kv3.first);
	EXPECT_EQ("testvalue3", kv3.second);
	pair<string, string>& kv4 = kv_pairs.back();
	EXPECT_EQ("testkey4", kv4.first);
	EXPECT_EQ("testvalue4", kv4.second);
}
