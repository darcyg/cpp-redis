#include <gtest/gtest.h>
#include <unistd.h>
#include "async_connection.hpp"
#include "async_service.hpp"

TEST(AsyncTest, connect)
{
	redis::async_service::instance().start();
	redis::async_connection conn;
	EXPECT_EQ(0, conn.connect("localhost"));
	redis::async_service::instance().stop();
}
