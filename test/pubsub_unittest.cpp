#include "redis_test.hpp"
#include "async_service.hpp"
#include "pubsub.hpp"

class PubSubTest : public RedisTest {
};

class TestListener : public redis::Listener {
    virtual void on_message(const string& channel, const string& message)
    {
        cout << channel << '\t' << message << endl;
    }

    virtual void on_pmessage(const string& pattern, const string& message)
    {
        cout << pattern << '\t' << message << endl;
    }
};

TEST_F(PubSubTest, pubsub)
{
    redis::async_service::instance().start();

    redis::Publisher pub;
    EXPECT_EQ(0, pub.connect("redis://localhost:6379"));

    TestListener listener;
    redis::Subscriber sub;
    EXPECT_EQ(0, sub.connect("redis://localhost::6379"));
    sub.set_listener(&listener);
    // sub.subscribe("testtopic"); 

    for (int i = 0; i < 100; i++)
    {
        pub.publish("testtopic", "testmessage");
        sleep(1);
    }

    redis::async_service::instance().stop();

    EXPECT_TRUE(true);
}
