#ifndef REDIS_TEST_HPP
#define REDIS_TEST_HPP

#include <gtest/gtest.h>
#include "redis_client.hpp"

class RedisTest : public testing::Test {
public:
	virtual void SetUp()
	{
		rc_.connect("127.0.0.1", 6379);

	}

	virtual void TearDown()
	{
		rc_.close();
	}

protected:
	redis::Client rc_;
};

#endif