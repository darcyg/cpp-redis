#include <cstring>
#include <iostream>
#include "pubsub.hpp"
#include "uri.hpp"

using namespace std;

namespace redis {

#define SUBSCRIBE    "subscribe"
#define UNSUBSCRIBE  "unsubscribe"
#define PSUBSCRIBE   "psubscribe"
#define PUNSUBSCRIBE "punsubscribe"
#define MESSAGE      "message"
#define PMESSAGE     "pmessage"

int Subscriber::connect(const string& url)
{
    Uri uri;
    if (!Uri::parse(url, uri))
        return -1;

    async_conn_ = new async_connection();
    return async_conn_->connect(uri.host, uri.port, uri.db);
}

int Subscriber::reconnect()
{
    // reconnect and re-subscribe all channels & patterns
    return 0;
}

void Subscriber::set_listener(Listener* listener)
{
    listener_ = listener;
}

void Subscriber::on_subscribe(const string& channel, const int subscribed_channels)
{
    cout << channel << '\t' << subscribed_channels << endl;
    channels_.insert(channel);
}

void Subscriber::on_unsubscribe(const string& channel, const int subscribed_channels)
{
    cout << channel << '\t' << subscribed_channels << endl;
    channels_.erase(channel);
}

void Subscriber::on_psubscribe(const string& pattern, const int subscribed_patterns)
{
    cout << pattern << '\t' << subscribed_patterns << endl;
    patterns_.insert(pattern);
}

void Subscriber::on_punsubscribe(const string& pattern, const int subscribed_patterns)
{
    cout << pattern << '\t' << subscribed_patterns << endl;
    patterns_.erase(pattern);
}

void Subscriber::on_message(const string& channel, const string& message)
{
    listener_->on_message(channel, message);
}

void Subscriber::on_pmessage(const string& pattern, const string& channel, const string& message)
{
    listener_->on_pmessage(pattern, channel, message);
}

void Subscriber::sub_callback(redisAsyncContext *ac, void* r, void* privatedata)
{
    redisReply* reply = (redisReply*) r;
    Subscriber* sub = (Subscriber*) privatedata;

    if (reply == NULL)
        return;
    
    for (int index = 0; index < reply->elements; ++index)
    {
        if (strncmp(SUBSCRIBE, reply->element[index]->str, reply->element[index]->len) == 0)
        {
            ++index;
            string channel = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            sub->on_subscribe(channel, reply->element[index]->integer);
        }
        else if (strncmp(UNSUBSCRIBE, reply->element[index]->str, reply->element[index]->len) == 0)
        {
            ++index;
            string pattern = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            sub->on_unsubscribe(pattern, reply->element[index]->integer);
        }
        else if (strncmp(PSUBSCRIBE, reply->element[index]->str, reply->element[index]->len) == 0)
        {
            ++index;
            string pattern = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            sub->on_psubscribe(pattern, reply->element[index]->integer);
        }
        else if (strncmp(PUNSUBSCRIBE, reply->element[index]->str, reply->element[index]->len) == 0)
        {
            ++index;
            string pattern = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            sub->on_punsubscribe(pattern, reply->element[index]->integer);
        }
        else if (strncmp(MESSAGE, reply->element[index]->str, reply->element[index]->len) == 0)
        {
            ++index;
            string channel = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            string message = string(reply->element[index]->str, reply->element[index]->len);
            sub->on_message(channel, message);
        }
        else if (strncmp(PMESSAGE, reply->element[index]->str, reply->element[index]->len) == 0)
        {
            ++index;
            string pattern = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            string channel = string(reply->element[index]->str, reply->element[index]->len);
            ++index;
            string message = string(reply->element[index]->str, reply->element[index]->len);
            sub->on_pmessage(pattern, channel, message);
        }
    }
}

}