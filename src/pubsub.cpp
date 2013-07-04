#include "pubsub.hpp"

namespace redis {

int publisher::connect(const string& url)
{
    return 0;
}

int publisher::publish(const string& channel, const string& message)
{
    return 0;
}

void subscriber::subscribe_callback(redisAsyncContext *ac, void* r, void* privatedata)
{
    redisReply* reply = (redisReply*) r;
    subscriber* sub = (subscriber*) privatedata;

    if (reply == NULL)
        return;

    if (reply->type == REDIS_REPLY_ARRAY)
    {
        string command = string(reply->element[0]->str, reply->element[0]->len);
        string channel = string(reply->element[1]->str, reply->element[1]->len);
        string message = string(reply->element[2]->str, reply->element[2]->len);

        sub->on_message(channel, message);
    }
}

void subscriber::psubscribe_callback(redisAsyncContext *ac, void* r, void* privatedata)
{
    redisReply* reply = (redisReply*) r;
    subscriber* sub = (subscriber*) privatedata;

    if (reply == NULL)
        return;

    if (reply->type == REDIS_REPLY_ARRAY)
    {
        string command = string(reply->element[0]->str, reply->element[0]->len);
        string pattern = string(reply->element[1]->str, reply->element[1]->len);
        string message = string(reply->element[2]->str, reply->element[2]->len);

        sub->on_pmessage(pattern, message);
    }
}

}