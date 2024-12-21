#include <iostream>
#include <hiredis/hiredis.h>
#include <thread>
#include <chrono>
#include <string.h>
using namespace std;

void print_redis_reply(redisReply *reply, int indent = 0) {
    if (!reply) {
        std::cout << "NULL reply" << std::endl;
        return;
    }

    std::string indent_str(indent, ' ');

    switch (reply->type) {
        case REDIS_REPLY_STATUS:
            std::cout << indent_str << "Status: " << reply->str << std::endl;
            break;
        case REDIS_REPLY_ERROR:
            std::cout << indent_str << "Error: " << reply->str << std::endl;
            break;
        case REDIS_REPLY_INTEGER:
            std::cout << indent_str << "Integer: " << reply->integer << std::endl;
            break;
        case REDIS_REPLY_STRING:
            std::cout << indent_str << "String (" << reply->len << " bytes): ";
            if (reply->str) {
                std::cout << std::string(reply->str, reply->len);
            } else {
                std::cout << "NULL";
            }
            std::cout << std::endl;
            break;
        case REDIS_REPLY_ARRAY:
            std::cout << indent_str << "Array (" << reply->elements << " elements):" << std::endl;
            for (size_t i = 0; i < reply->elements; ++i) {
                print_redis_reply(reply->element[i], indent + 2);
            }
            break;
        default:
            std::cout << indent_str << "Unknown type" << std::endl;
            break;
    }
}





int main()
{
    redisContext* context = redisConnect("127.0.0.1",6379);
    if(context == nullptr || context->err)
    {
        return 1;
    }
    redisReply *reply = static_cast<redisReply*>(redisCommand(context,"auth 123456"));
    freeReplyObject(reply);
    reply = static_cast<redisReply*>(redisCommand(context,"XADD test * username 123"));
    print_redis_reply(reply,0);
    freeReplyObject(reply);
    reply = static_cast<redisReply*>(redisCommand(context,"XRANGE test - +"));
    if(reply == NULL)
    {
        cout<<"reply is null"<<endl;
    }
    else if(reply->type == REDIS_REPLY_ERROR)
    {
        cout<<"reply is error"<<endl;
    }
    else if(reply->type == REDIS_REPLY_ARRAY)
    {
        cout<<reply->elements<<endl;
        for(int i = 0; i<reply->elements; i++)
        {
            redisReply *entry = reply->element[i];
            if(entry->type == REDIS_REPLY_ARRAY && entry->elements == 2)
            {
                redisReply *msg_id = entry->element[0];
                redisReply *msg_data = entry->element[1];
                cout<<"messageId:"<<msg_id->str<<endl;
                if(msg_data->type == REDIS_REPLY_ARRAY)
                {
                    for(int j = 0; j<msg_data->elements; j+=2)
                    {
                        redisReply* field =  msg_data->element[j];
                        redisReply* valve = msg_data->element[j+1];
                        cout<<"field:"<<field->str<<" value:"<<valve->str<<endl;
                    }
                }
            }

        }
        
    }
    freeReplyObject(reply);
    redisFree(context);
    return 1;
    
}