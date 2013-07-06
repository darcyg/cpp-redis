#include <iostream>
#include "pubsub.hpp"
#include "uri.hpp"

using namespace std;

namespace redis {

void publish_callback(redisAsyncContext *ac, void* r, void* privatedata)
{
    redisReply* reply = (redisReply*)r;
    if (reply)
    {
         cout << __func__ << '\t' << reply->type << endl;
    }
}
       
int Publisher::connect(const string& url)
{
    Uri uri;
    if (!Uri::parse(url, uri))
        return -1;

    return async_conn_->connect(uri.host, uri.port, uri.db);
}

int Publisher::publish(const string& channel, const string& message)
{
    makecmd cmd("PUBLISH");
    cmd << channel << message;
    async_conn_->send_command(cmd, publish_callback, this);
}

int Subscriber::connect(const string& url)
{
    Uri uri;
    if (!Uri::parse(url, uri))
        return -1;

    return async_conn_->connect(uri.host, uri.port, uri.db);
}

void Subscriber::subscribe_callback(redisAsyncContext *ac, void* r, void* privatedata)
{
    redisReply* reply = (redisReply*) r;
    Listener* listener = (Listener*) privatedata;

    if (reply == NULL)
        return;

    if (reply->type == REDIS_REPLY_ARRAY)
    {
        string command = string(reply->element[0]->str, reply->element[0]->len);
        string channel = string(reply->element[1]->str, reply->element[1]->len);
        string message = string(reply->element[2]->str, reply->element[2]->len);

        listener->on_message(channel, message);
    }
}

void Subscriber::psubscribe_callback(redisAsyncContext *ac, void* r, void* privatedata)
{
    redisReply* reply = (redisReply*) r;
    Listener* listener = (Listener*) privatedata;

    if (reply == NULL)
        return;

    if (reply->type == REDIS_REPLY_ARRAY)
    {
        string command = string(reply->element[0]->str, reply->element[0]->len);
        string pattern = string(reply->element[1]->str, reply->element[1]->len);
        string message = string(reply->element[2]->str, reply->element[2]->len);

        listener->on_pmessage(pattern, message);
    }
}

}