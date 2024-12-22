// // #include <iostream>
// #include <hiredis/hiredis.h>
// // #include <thread>
// // #include <chrono>
// #include <string.h>
// // using namespace std;

// void print_redis_reply(redisReply *reply, int indent) {
//     if (!reply) {
//         // std::cout << "NULL reply" << std::endl;
//         printf("null reply\n");
//         return;
//     }

//     // std::string indent_str(indent, ' ');
//     char indent_str[1024];

//     switch (reply->type) {
//         case REDIS_REPLY_STATUS:
//             // std::cout << indent_str << "Status: " << reply->str << std::endl;
//             printf("%s : status: %s\n",indent_str,reply->str);
//             break;
//         case REDIS_REPLY_ERROR:
//             printf("%s : error: %s\n",indent_str,reply->str);
//             break;
//         case REDIS_REPLY_INTEGER:
//             // std::cout << indent_str << "Integer: " << reply->integer << std::endl;
//             printf("%s : integer: %lld\n",indent_str,reply->integer);
//             break;
//         case REDIS_REPLY_STRING:
//             // std::cout << indent_str << "String (" << reply->len << " bytes): ";
//             printf("%s : string (%d bytes): ",indent_str,(int)reply->len);
//             if (reply->str) {
//                 // std::cout << std::string(reply->str, reply->len);
//                 printf("%s\n",reply->str);
//             } else {
//                 // std::cout << "NULL";
//                 printf("NULL\n");
//             }
//             // std::cout << std::endl;
//             break;
//         case REDIS_REPLY_ARRAY:
//             // std::cout << indent_str << "Array (" << reply->elements << " elements):" << std::endl;
//             printf("%s : array (%d elements): \n",indent_str,(int)reply->elements);
//             for (size_t i = 0; i < reply->elements; ++i) {
//                 print_redis_reply(reply->element[i], indent + 2);
//             }
//             break;
//         default:
//             // std::cout << indent_str << "Unknown type" << std::endl;
//             printf("%s : unknown type\n",indent_str);
//             break;
//     }
// }


// int main()
// {
//     redisContext* context = redisConnect("127.0.0.1",6379);
//     if(context == NULL || context->err)
//     {
//         return 1;
//     }
//     redisReply *reply = (redisReply*)(redisCommand(context,"auth 123456"));
//     // print_redis_reply(reply,0);
//     freeReplyObject(reply);
//     reply = (redisReply*)(redisCommand(context,"XADD test * username 123"));
//     print_redis_reply(reply,0);
//     freeReplyObject(reply);
//     reply = (redisReply*)(redisCommand(context,"XREAD COUNT 1 BLOCK 0 STREAMS test 0"));
//     print_redis_reply(reply,0);
//     freeReplyObject(reply);
//     reply = (redisReply*)(redisCommand(context,"XRANGE test - +"));
//     print_redis_reply(reply,0);
//     if(reply == NULL)
//     {
//         // cout<<"reply is null"<<endl;
//         printf("reply is null\n");
//     }
//     else if(reply->type == REDIS_REPLY_ERROR)
//     {
//         // cout<<"reply is error"<<endl;
//         printf("reply is error\n");
//     }
//     else if(reply->type == REDIS_REPLY_ARRAY)
//     {
//         // cout<<reply->elements<<endl;
//         printf("%d\n",(int)reply->elements);
//         for(int i = 0; i<reply->elements; i++)
//         {
//             redisReply *entry = reply->element[i];
//             if(entry->type == REDIS_REPLY_ARRAY && entry->elements == 2)
//             {
//                 redisReply *msg_id = entry->element[0];
//                 redisReply *msg_data = entry->element[1];
//                 // cout<<"messageId:"<<msg_id->str<<endl;
//                 printf("messageId:%s\n",msg_id->str);
//                 if(msg_data->type == REDIS_REPLY_ARRAY)
//                 {
//                     for(int j = 0; j<msg_data->elements; j+=2)
//                     {
//                         redisReply* field =  msg_data->element[j];
//                         redisReply* valve = msg_data->element[j+1];
//                         // cout<<"field:"<<field->str<<" value:"<<valve->str<<endl;
//                         printf("field:%s value:%s\n",field->str,valve->str);
//                     }
//                 }
//             }

//         }
        
//     }
//     freeReplyObject(reply);
//     redisFree(context);
//     return 1;
    
// }



// #include <hiredis/hiredis.h>
// #include <iostream>
// #include <vector>
// #include <string>

// void add_message_to_stream(redisContext *context, const std::string& stream_name, const std::string& field, const std::string& value) {
//     // 构建 XADD 命令
//     redisReply *reply = (redisReply*)redisCommand(context, "XADD %s MAXLEN ~ 100 * %s %s", 
//                                                   stream_name.c_str(), field.c_str(), value.c_str());
    
//     if (reply == nullptr) {
//         std::cerr << "Error adding message to stream: " << context->errstr << "\n";
//         return;
//     }

//     // 检查回复是否为字符串类型，并打印消息ID
//     if (reply->type == REDIS_REPLY_STRING) {
//         std::cout << "Message ID: " << reply->str << "\n";
//     } else {
//         std::cerr << "Unexpected reply type from XADD\n";
//     }

//     freeReplyObject(reply); // 释放回复对象内存
// }


// void read_messages_from_stream(redisContext *context, const std::string& stream_name) {
//     // 构建 XREAD 命令，从最新的消息开始读取
//     redisReply *reply = (redisReply*)redisCommand(context, "XREAD COUNT 1  BLOCK 0 STREAMS %s $", stream_name.c_str());

//     if (reply == nullptr || reply->type != REDIS_REPLY_ARRAY || reply->elements != 1) {
//         std::cerr << "Error reading messages from stream or no new messages.\n";
//         if (reply) freeReplyObject(reply);
//         return;
//     }

//     // 解析回复数组
//     redisReply *stream_reply = reply->element[0]; // 第一个元素是流回复
//     if (stream_reply->type != REDIS_REPLY_ARRAY || stream_reply->elements != 2) {
//         std::cerr << "Unexpected reply structure from XREAD\n";
//         freeReplyObject(reply);
//         return;
//     }

//     // 解析流名称和消息
//     std::string stream_key = std::string(stream_reply->element[0]->str, stream_reply->element[0]->len);
//     redisReply *messages_reply = stream_reply->element[1];

//     if (messages_reply->type != REDIS_REPLY_ARRAY) {
//         std::cerr << "Unexpected messages reply structure\n";
//         freeReplyObject(reply);
//         return;
//     }

//     for (size_t i = 0; i < messages_reply->elements; ++i) {
//         redisReply *message = messages_reply->element[i];
//         if (message->type != REDIS_REPLY_ARRAY || message->elements != 2) {
//             std::cerr << "Unexpected message structure\n";
//             continue;
//         }

//         // 获取消息ID
//         std::string message_id = std::string(message->element[0]->str, message->element[0]->len);

//         // 解析消息内容
//         redisReply *fields_reply = message->element[1];
//         if (fields_reply->type != REDIS_REPLY_ARRAY || fields_reply->elements % 2 != 0) {
//             std::cerr << "Unexpected fields structure\n";
//             continue;
//         }

//         std::cout << "Message ID: " << message_id << "\nFields:\n";
//         for (size_t j = 0; j < fields_reply->elements; j += 2) {
//             std::string field = std::string(fields_reply->element[j]->str, fields_reply->element[j]->len);
//             std::string value = std::string(fields_reply->element[j + 1]->str, fields_reply->element[j + 1]->len);
//             std::cout << "  " << field << ": " << value << "\n";
//         }
//     }

//     freeReplyObject(reply); // 释放回复对象内存
// }

// int main() {
//     // 创建 Redis 连接上下文
//     redisContext *context = redisConnect("127.0.0.1", 6379);
//     if (context == nullptr || context->err) {
//         if (context) {
//             std::cerr << "Connection error: " << context->errstr << "\n";
//             redisFree(context);
//         } else {
//             std::cerr << "Connection error: can't allocate redis context\n";
//         }
//         return 1;
//     }
//     redisReply* reply = (redisReply*)redisCommand(context,"auth 123456");
//     freeReplyObject(reply);

//     // 向 Stream 添加消息
//     add_message_to_stream(context, "mystream", "event", "Hello World");

//     // 从 Stream 读取消息
//     read_messages_from_stream(context, "mystream");

//     // 断开连接
//     redisFree(context);

//     return 0;
// }

// #include <iostream>
// #include <hiredis/hiredis.h>
// using namespace std;
// int main() {
//     // 建立与Redis的连接
//     redisContext* context = redisConnect("127.0.0.1", 6379);
//     if (context == NULL || context->err) {
//         if (context) {
//             std::cerr << "Error: " << context->errstr << std::endl;
//             redisFree(context);
//         } else {
//             std::cerr << "Can't allocate redis context" << std::endl;
//         }
//         return 1;
//     }
//     redisReply *reply1 = (redisReply*)(redisCommand(context , "auth 123456"));
//     freeReplyObject(reply1);

//     redisReply* reply = (redisReply*)(redisCommand(context , "get a"));
//     if(reply == NULL)
//     {
//         cout<<"reply = null"<<endl;
//     }
//     else if (reply->type == REDIS_REPLY_NIL)
//     {
//         cout<<"type为nil"<<endl;
//     }
//     else if (reply->type == REDIS_REPLY_STATUS)
//     {
//         // std::string temp = std::string(reply->str,reply->len);
//         // cout<<"status:"<<temp<<endl;
//         cout<<"lens : "<<reply->len<<endl;
//     }
//     else if (reply->type = REDIS_REPLY_STRING)
//     {
//         cout<<"lens : "<<reply->len<<endl;
//         cout<<"string : "<<reply->str<<endl;
//     }
//     freeReplyObject(reply);
//     return 0;


//     // redisReply* reply1 = (redisReply*)(redisCommand(context,"auth 123456"));
//     // freeReplyObject(reply1);
    
//     // // 消息字段和值
//     // const char* field1 = "message";
//     // const char* value1 = "Hello, Redis Stream!";
//     // // 执行XADD命令
//     // redisReply* reply = (redisReply*)redisCommand(context, "XADD mystream * %s %s", field1, value1);
//     // if (reply == NULL) {
//     //     std::cerr << "Failed to execute XADD command." << std::endl;
//     //     redisFree(context);
//     //     return 1;
//     // }
//     // if (reply->type == REDIS_REPLY_STRING) {
//     //     std::cout << "Message added with ID: " << reply-> << std::endl;
//     // }
//     // freeReplyObject(reply);
//     // redisFree(context);
//     // return 0;
// }]]


#include <hiredis/hiredis.h>
#include <iostream>
#include <string.h>
using namespace std;

int main() {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == nullptr || c->err) {
        if (c) {
            std::cerr << "Connection error: " << c->errstr << "\n";
            redisFree(c);
        } else {
            std::cerr << "Connection error: can't allocate redis context\n";
        }
        return 1;
    }

    // Ping the server
    redisReply *reply = (redisReply*)redisCommand(c, "auth 123456");
    if(reply == nullptr)
    {
        cout<<"reply == nullptr"<<endl;
    }
    else if (reply->type == REDIS_REPLY_STATUS)
    {
        cout<<"TYPE = status"<<endl;
        int len = (int)reply->len;
        cout<<"len = "<<reply->len<<endl;
        cout<<"len = "<<len<<endl;
        cout<<"str = "<<reply->str<<endl;
    }
    else
    {
        cout<<"wrong"<<endl;
    }
    
    freeReplyObject(reply);

    redisFree(c);
    return 0;
}

// #include <hiredis/hiredis.h>

// #include <stdio.h>
// #include <assert.h>

// #define REDIS_HOST "127.0.0.1"
// #define REDIS_PORT 6379


// int main(void) {
//     redisContext *c;
//     redisReply *r;

//     c = redisConnect(REDIS_HOST, REDIS_PORT);
//     assert(c != NULL && c->err == 0);

//     char *redis_key = "show_id:62fda075-4c03-4b88-9894-8ba7498051b1,proceed";

//     r = redisCommand(c, "SET %s %s", "A", redis_key);
//     assert(r != NULL && r->type == REDIS_REPLY_STATUS);
//     printf("reply->str %s\n", r->str ? r->str : "(null)");
//     freeReplyObject(r);

//     r = redisCommand(c, "GET %s", "A");
//     assert(r != NULL && r->type == REDIS_REPLY_STRING);
//     printf("reply->str %s\n", r->str ? r->str : "(null)");
//     freeReplyObject(r);

//     redisFree(c);
// }