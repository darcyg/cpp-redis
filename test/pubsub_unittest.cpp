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

    virtual void on_pmessage(const string& pattern, const string& channel, const string& message)
    {
        cout << pattern << '\t' << channel << '\t' << message << endl;
    }
};

TEST_F(PubSubTest, pubsub)
{
    redis::async_service::instance().start();

    redis::Subscriber sub;
    EXPECT_EQ(0, sub.connect("redis://localhost::6379"));
    sub.subscribe("foo", "bar"); 
    sub.psubscribe("test*", "zoo*");

    sub.set_listener(new TestListener());

    for (int i = 0; i < 10; i++)
        rc_.publish("foo", "test");

    redis::async_service::instance().stop();
    
    EXPECT_TRUE(true);
}
