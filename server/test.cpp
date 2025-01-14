// // // // // // // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // // // // // #include <hiredis/hiredis.h>
// // // // // // // // // // // // // // // // // // #include <thread>
// // // // // // // // // // // // // // // // // // #include <chrono>
// // // // // // // // // // // // // // // // // #include <string.h>
// // // // // // // // // // // // // // // // // // using namespace std;

// // // // // // // // // // // // // // // // // void print_redis_reply(redisReply *reply, int indent) {
// // // // // // // // // // // // // // // // //     if (!reply) {
// // // // // // // // // // // // // // // // //         // std::cout << "NULL reply" << std::endl;
// // // // // // // // // // // // // // // // //         printf("null reply\n");
// // // // // // // // // // // // // // // // //         return;
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     // std::string indent_str(indent, ' ');
// // // // // // // // // // // // // // // // //     char indent_str[1024];

// // // // // // // // // // // // // // // // //     switch (reply->type) {
// // // // // // // // // // // // // // // // //         case REDIS_REPLY_STATUS:
// // // // // // // // // // // // // // // // //             // std::cout << indent_str << "Status: " << reply->str << std::endl;
// // // // // // // // // // // // // // // // //             printf("%s : status: %s\n",indent_str,reply->str);
// // // // // // // // // // // // // // // // //             break;
// // // // // // // // // // // // // // // // //         case REDIS_REPLY_ERROR:
// // // // // // // // // // // // // // // // //             printf("%s : error: %s\n",indent_str,reply->str);
// // // // // // // // // // // // // // // // //             break;
// // // // // // // // // // // // // // // // //         case REDIS_REPLY_INTEGER:
// // // // // // // // // // // // // // // // //             // std::cout << indent_str << "Integer: " << reply->integer << std::endl;
// // // // // // // // // // // // // // // // //             printf("%s : integer: %lld\n",indent_str,reply->integer);
// // // // // // // // // // // // // // // // //             break;
// // // // // // // // // // // // // // // // //         case REDIS_REPLY_STRING:
// // // // // // // // // // // // // // // // //             // std::cout << indent_str << "String (" << reply->len << " bytes): ";
// // // // // // // // // // // // // // // // //             printf("%s : string (%d bytes): ",indent_str,(int)reply->len);
// // // // // // // // // // // // // // // // //             if (reply->str) {
// // // // // // // // // // // // // // // // //                 // std::cout << std::string(reply->str, reply->len);
// // // // // // // // // // // // // // // // //                 printf("%s\n",reply->str);
// // // // // // // // // // // // // // // // //             } else {
// // // // // // // // // // // // // // // // //                 // std::cout << "NULL";
// // // // // // // // // // // // // // // // //                 printf("NULL\n");
// // // // // // // // // // // // // // // // //             }
// // // // // // // // // // // // // // // // //             // std::cout << std::endl;
// // // // // // // // // // // // // // // // //             break;
// // // // // // // // // // // // // // // // //         case REDIS_REPLY_ARRAY:
// // // // // // // // // // // // // // // // //             // std::cout << indent_str << "Array (" << reply->elements << " elements):" << std::endl;
// // // // // // // // // // // // // // // // //             printf("%s : array (%d elements): \n",indent_str,(int)reply->elements);
// // // // // // // // // // // // // // // // //             for (size_t i = 0; i < reply->elements; ++i) {
// // // // // // // // // // // // // // // // //                 print_redis_reply(reply->element[i], indent + 2);
// // // // // // // // // // // // // // // // //             }
// // // // // // // // // // // // // // // // //             break;
// // // // // // // // // // // // // // // // //         default:
// // // // // // // // // // // // // // // // //             // std::cout << indent_str << "Unknown type" << std::endl;
// // // // // // // // // // // // // // // // //             printf("%s : unknown type\n",indent_str);
// // // // // // // // // // // // // // // // //             break;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // // int main()
// // // // // // // // // // // // // // // // // {
// // // // // // // // // // // // // // // // //     redisContext* context = redisConnect("127.0.0.1",6379);
// // // // // // // // // // // // // // // // //     if(context == NULL || context->err)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     redisReply *reply = (redisReply*)(redisCommand(context,"auth 123456"));
// // // // // // // // // // // // // // // // //     // print_redis_reply(reply,0);
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     reply = (redisReply*)(redisCommand(context,"XADD test * username 123"));
// // // // // // // // // // // // // // // // //     print_redis_reply(reply,0);
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     reply = (redisReply*)(redisCommand(context,"XREAD COUNT 1 BLOCK 0 STREAMS test 0"));
// // // // // // // // // // // // // // // // //     print_redis_reply(reply,0);
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     reply = (redisReply*)(redisCommand(context,"XRANGE test - +"));
// // // // // // // // // // // // // // // // //     print_redis_reply(reply,0);
// // // // // // // // // // // // // // // // //     if(reply == NULL)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         // cout<<"reply is null"<<endl;
// // // // // // // // // // // // // // // // //         printf("reply is null\n");
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if(reply->type == REDIS_REPLY_ERROR)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         // cout<<"reply is error"<<endl;
// // // // // // // // // // // // // // // // //         printf("reply is error\n");
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if(reply->type == REDIS_REPLY_ARRAY)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         // cout<<reply->elements<<endl;
// // // // // // // // // // // // // // // // //         printf("%d\n",(int)reply->elements);
// // // // // // // // // // // // // // // // //         for(int i = 0; i<reply->elements; i++)
// // // // // // // // // // // // // // // // //         {
// // // // // // // // // // // // // // // // //             redisReply *entry = reply->element[i];
// // // // // // // // // // // // // // // // //             if(entry->type == REDIS_REPLY_ARRAY && entry->elements == 2)
// // // // // // // // // // // // // // // // //             {
// // // // // // // // // // // // // // // // //                 redisReply *msg_id = entry->element[0];
// // // // // // // // // // // // // // // // //                 redisReply *msg_data = entry->element[1];
// // // // // // // // // // // // // // // // //                 // cout<<"messageId:"<<msg_id->str<<endl;
// // // // // // // // // // // // // // // // //                 printf("messageId:%s\n",msg_id->str);
// // // // // // // // // // // // // // // // //                 if(msg_data->type == REDIS_REPLY_ARRAY)
// // // // // // // // // // // // // // // // //                 {
// // // // // // // // // // // // // // // // //                     for(int j = 0; j<msg_data->elements; j+=2)
// // // // // // // // // // // // // // // // //                     {
// // // // // // // // // // // // // // // // //                         redisReply* field =  msg_data->element[j];
// // // // // // // // // // // // // // // // //                         redisReply* valve = msg_data->element[j+1];
// // // // // // // // // // // // // // // // //                         // cout<<"field:"<<field->str<<" value:"<<valve->str<<endl;
// // // // // // // // // // // // // // // // //                         printf("field:%s value:%s\n",field->str,valve->str);
// // // // // // // // // // // // // // // // //                     }
// // // // // // // // // // // // // // // // //                 }
// // // // // // // // // // // // // // // // //             }

// // // // // // // // // // // // // // // // //         }

// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     redisFree(context);
// // // // // // // // // // // // // // // // //     return 1;

// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // // #include <hiredis/hiredis.h>
// // // // // // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // // // // // #include <vector>
// // // // // // // // // // // // // // // // // #include <string>

// // // // // // // // // // // // // // // // // void add_message_to_stream(redisContext *context, const std::string& stream_name, const std::string& field, const std::string& value) {
// // // // // // // // // // // // // // // // //     // 构建 XADD 命令
// // // // // // // // // // // // // // // // //     redisReply *reply = (redisReply*)redisCommand(context, "XADD %s MAXLEN ~ 100 * %s %s",
// // // // // // // // // // // // // // // // //                                                   stream_name.c_str(), field.c_str(), value.c_str());

// // // // // // // // // // // // // // // // //     if (reply == nullptr) {
// // // // // // // // // // // // // // // // //         std::cerr << "Error adding message to stream: " << context->errstr << "\n";
// // // // // // // // // // // // // // // // //         return;
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     // 检查回复是否为字符串类型，并打印消息ID
// // // // // // // // // // // // // // // // //     if (reply->type == REDIS_REPLY_STRING) {
// // // // // // // // // // // // // // // // //         std::cout << "Message ID: " << reply->str << "\n";
// // // // // // // // // // // // // // // // //     } else {
// // // // // // // // // // // // // // // // //         std::cerr << "Unexpected reply type from XADD\n";
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     freeReplyObject(reply); // 释放回复对象内存
// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // // void read_messages_from_stream(redisContext *context, const std::string& stream_name) {
// // // // // // // // // // // // // // // // //     // 构建 XREAD 命令，从最新的消息开始读取
// // // // // // // // // // // // // // // // //     redisReply *reply = (redisReply*)redisCommand(context, "XREAD COUNT 1  BLOCK 0 STREAMS %s $", stream_name.c_str());

// // // // // // // // // // // // // // // // //     if (reply == nullptr || reply->type != REDIS_REPLY_ARRAY || reply->elements != 1) {
// // // // // // // // // // // // // // // // //         std::cerr << "Error reading messages from stream or no new messages.\n";
// // // // // // // // // // // // // // // // //         if (reply) freeReplyObject(reply);
// // // // // // // // // // // // // // // // //         return;
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     // 解析回复数组
// // // // // // // // // // // // // // // // //     redisReply *stream_reply = reply->element[0]; // 第一个元素是流回复
// // // // // // // // // // // // // // // // //     if (stream_reply->type != REDIS_REPLY_ARRAY || stream_reply->elements != 2) {
// // // // // // // // // // // // // // // // //         std::cerr << "Unexpected reply structure from XREAD\n";
// // // // // // // // // // // // // // // // //         freeReplyObject(reply);
// // // // // // // // // // // // // // // // //         return;
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     // 解析流名称和消息
// // // // // // // // // // // // // // // // //     std::string stream_key = std::string(stream_reply->element[0]->str, stream_reply->element[0]->len);
// // // // // // // // // // // // // // // // //     redisReply *messages_reply = stream_reply->element[1];

// // // // // // // // // // // // // // // // //     if (messages_reply->type != REDIS_REPLY_ARRAY) {
// // // // // // // // // // // // // // // // //         std::cerr << "Unexpected messages reply structure\n";
// // // // // // // // // // // // // // // // //         freeReplyObject(reply);
// // // // // // // // // // // // // // // // //         return;
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     for (size_t i = 0; i < messages_reply->elements; ++i) {
// // // // // // // // // // // // // // // // //         redisReply *message = messages_reply->element[i];
// // // // // // // // // // // // // // // // //         if (message->type != REDIS_REPLY_ARRAY || message->elements != 2) {
// // // // // // // // // // // // // // // // //             std::cerr << "Unexpected message structure\n";
// // // // // // // // // // // // // // // // //             continue;
// // // // // // // // // // // // // // // // //         }

// // // // // // // // // // // // // // // // //         // 获取消息ID
// // // // // // // // // // // // // // // // //         std::string message_id = std::string(message->element[0]->str, message->element[0]->len);

// // // // // // // // // // // // // // // // //         // 解析消息内容
// // // // // // // // // // // // // // // // //         redisReply *fields_reply = message->element[1];
// // // // // // // // // // // // // // // // //         if (fields_reply->type != REDIS_REPLY_ARRAY || fields_reply->elements % 2 != 0) {
// // // // // // // // // // // // // // // // //             std::cerr << "Unexpected fields structure\n";
// // // // // // // // // // // // // // // // //             continue;
// // // // // // // // // // // // // // // // //         }

// // // // // // // // // // // // // // // // //         std::cout << "Message ID: " << message_id << "\nFields:\n";
// // // // // // // // // // // // // // // // //         for (size_t j = 0; j < fields_reply->elements; j += 2) {
// // // // // // // // // // // // // // // // //             std::string field = std::string(fields_reply->element[j]->str, fields_reply->element[j]->len);
// // // // // // // // // // // // // // // // //             std::string value = std::string(fields_reply->element[j + 1]->str, fields_reply->element[j + 1]->len);
// // // // // // // // // // // // // // // // //             std::cout << "  " << field << ": " << value << "\n";
// // // // // // // // // // // // // // // // //         }
// // // // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // // // //     freeReplyObject(reply); // 释放回复对象内存
// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // // int main() {
// // // // // // // // // // // // // // // // //     // 创建 Redis 连接上下文
// // // // // // // // // // // // // // // // //     redisContext *context = redisConnect("127.0.0.1", 6379);
// // // // // // // // // // // // // // // // //     if (context == nullptr || context->err) {
// // // // // // // // // // // // // // // // //         if (context) {
// // // // // // // // // // // // // // // // //             std::cerr << "Connection error: " << context->errstr << "\n";
// // // // // // // // // // // // // // // // //             redisFree(context);
// // // // // // // // // // // // // // // // //         } else {
// // // // // // // // // // // // // // // // //             std::cerr << "Connection error: can't allocate redis context\n";
// // // // // // // // // // // // // // // // //         }
// // // // // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     redisReply* reply = (redisReply*)redisCommand(context,"auth 123456");
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);

// // // // // // // // // // // // // // // // //     // 向 Stream 添加消息
// // // // // // // // // // // // // // // // //     add_message_to_stream(context, "mystream", "event", "Hello World");

// // // // // // // // // // // // // // // // //     // 从 Stream 读取消息
// // // // // // // // // // // // // // // // //     read_messages_from_stream(context, "mystream");

// // // // // // // // // // // // // // // // //     // 断开连接
// // // // // // // // // // // // // // // // //     redisFree(context);

// // // // // // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // // // // // #include <hiredis/hiredis.h>
// // // // // // // // // // // // // // // // // using namespace std;
// // // // // // // // // // // // // // // // // int main() {
// // // // // // // // // // // // // // // // //     // 建立与Redis的连接
// // // // // // // // // // // // // // // // //     redisContext* context = redisConnect("127.0.0.1", 6379);
// // // // // // // // // // // // // // // // //     if (context == NULL || context->err) {
// // // // // // // // // // // // // // // // //         if (context) {
// // // // // // // // // // // // // // // // //             std::cerr << "Error: " << context->errstr << std::endl;
// // // // // // // // // // // // // // // // //             redisFree(context);
// // // // // // // // // // // // // // // // //         } else {
// // // // // // // // // // // // // // // // //             std::cerr << "Can't allocate redis context" << std::endl;
// // // // // // // // // // // // // // // // //         }
// // // // // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     redisReply *reply1 = (redisReply*)(redisCommand(context , "auth 123456"));
// // // // // // // // // // // // // // // // //     freeReplyObject(reply1);

// // // // // // // // // // // // // // // // //     redisReply* reply = (redisReply*)(redisCommand(context , "get a"));
// // // // // // // // // // // // // // // // //     if(reply == NULL)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         cout<<"reply = null"<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if (reply->type == REDIS_REPLY_NIL)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         cout<<"type为nil"<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if (reply->type == REDIS_REPLY_STATUS)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         // std::string temp = std::string(reply->str,reply->len);
// // // // // // // // // // // // // // // // //         // cout<<"status:"<<temp<<endl;
// // // // // // // // // // // // // // // // //         cout<<"lens : "<<reply->len<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if (reply->type = REDIS_REPLY_STRING)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         cout<<"lens : "<<reply->len<<endl;
// // // // // // // // // // // // // // // // //         cout<<"string : "<<reply->str<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     return 0;

// // // // // // // // // // // // // // // // //     // redisReply* reply1 = (redisReply*)(redisCommand(context,"auth 123456"));
// // // // // // // // // // // // // // // // //     // freeReplyObject(reply1);

// // // // // // // // // // // // // // // // //     // // 消息字段和值
// // // // // // // // // // // // // // // // //     // const char* field1 = "message";
// // // // // // // // // // // // // // // // //     // const char* value1 = "Hello, Redis Stream!";
// // // // // // // // // // // // // // // // //     // // 执行XADD命令
// // // // // // // // // // // // // // // // //     // redisReply* reply = (redisReply*)redisCommand(context, "XADD mystream * %s %s", field1, value1);
// // // // // // // // // // // // // // // // //     // if (reply == NULL) {
// // // // // // // // // // // // // // // // //     //     std::cerr << "Failed to execute XADD command." << std::endl;
// // // // // // // // // // // // // // // // //     //     redisFree(context);
// // // // // // // // // // // // // // // // //     //     return 1;
// // // // // // // // // // // // // // // // //     // }
// // // // // // // // // // // // // // // // //     // if (reply->type == REDIS_REPLY_STRING) {
// // // // // // // // // // // // // // // // //     //     std::cout << "Message added with ID: " << reply-> << std::endl;
// // // // // // // // // // // // // // // // //     // }
// // // // // // // // // // // // // // // // //     // freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     // redisFree(context);
// // // // // // // // // // // // // // // // //     // return 0;
// // // // // // // // // // // // // // // // // }]]

// // // // // // // // // // // // // // // // #include <hiredis/hiredis.h>
// // // // // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // // // // #include <string.h>
// // // // // // // // // // // // // // // // using namespace std;

// // // // // // // // // // // // // // // // // int main() {
// // // // // // // // // // // // // // // // //     redisContext* context = redisConnect("127.0.0.1", 6379);
// // // // // // // // // // // // // // // // //     if (context == NULL || context->err) {
// // // // // // // // // // // // // // // // //         if (context) {
// // // // // // // // // // // // // // // // //             std::cerr << "Error: " << context->errstr << std::endl;
// // // // // // // // // // // // // // // // //             redisFree(context);
// // // // // // // // // // // // // // // // //         } else {
// // // // // // // // // // // // // // // // //             std::cerr << "Can't allocate redis context" << std::endl;
// // // // // // // // // // // // // // // // //         }
// // // // // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     redisReply* reply = (redisReply*)(redisCommand(context, "auth 123456"));
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     reply = (redisReply*)(redisCommand(context, "xadd test1 * username creeper age 20"));
// // // // // // // // // // // // // // // // //     if(reply == NULL)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         cout<<"reply = null"<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if (reply->type = REDIS_REPLY_STRING)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         cout<<"reply->str : "<<reply->str<<endl;
// // // // // // // // // // // // // // // // //         cout<<"reply->len : "<<reply->len<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // // //     reply = (redisReply*)(redisCommand(context , "xrange test1 - +"));
// // // // // // // // // // // // // // // // //     int count = 0;
// // // // // // // // // // // // // // // // //     if(reply == NULL)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         cout<<"reply = null"<<endl;
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     else if (reply->type = REDIS_REPLY_ARRAY)
// // // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // // //         for(int i = 0; i<reply->elements; i++)
// // // // // // // // // // // // // // // // //         {
// // // // // // // // // // // // // // // // //             redisReply* entry = reply->element[i];
// // // // // // // // // // // // // // // // //             redisReply* msg_id = entry->element[0];
// // // // // // // // // // // // // // // // //             redisReply* msg_data = entry->element[1];
// // // // // // // // // // // // // // // // //             for(int j = 0; j<msg_data->elements - 1; j+=2)
// // // // // // // // // // // // // // // // //             {
// // // // // // // // // // // // // // // // //                 redisReply* field = msg_data->element[j];
// // // // // // // // // // // // // // // // //                 redisReply* value = msg_data->element[j+1];
// // // // // // // // // // // // // // // // //                 cout<<"序号为："<<count<<"  field :"<<field->str<<"\tvalue : "<<value->str<<endl;
// // // // // // // // // // // // // // // // //                 count++;
// // // // // // // // // // // // // // // // //             }
// // // // // // // // // // // // // // // // //         }

// // // // // // // // // // // // // // // // //         // cout<<"reply->elements : "<<reply->elements<<endl;
// // // // // // // // // // // // // // // // //         // for(int i = 0; i<reply->elements; i++)
// // // // // // // // // // // // // // // // //         // {
// // // // // // // // // // // // // // // // //         //     redisReply* entry = reply->element[i];
// // // // // // // // // // // // // // // // //         //     cout<<"entry->elements : "<<entry->elements<<endl;
// // // // // // // // // // // // // // // // //         //     for(int j = 0; j<entry->elements; j++)
// // // // // // // // // // // // // // // // //         //     {
// // // // // // // // // // // // // // // // //         //         redisReply* msg_id = entry->element[0];
// // // // // // // // // // // // // // // // //         //         cout<<"msg_id : "<<msg_id->str<<endl;
// // // // // // // // // // // // // // // // //         //         redisReply* msg_data = entry->element[1];
// // // // // // // // // // // // // // // // //         //         for(int k = 0; k<msg_data->elements - 2; k+=2)
// // // // // // // // // // // // // // // // //         //         {
// // // // // // // // // // // // // // // // //         //             redisReply* field = msg_data->element[k];
// // // // // // // // // // // // // // // // //         //             redisReply* value = msg_data->element[k+1];
// // // // // // // // // // // // // // // // //         //             cout<<"序号为："<<count<<"  field : "<<field->str<<" value : "<<value->str<<endl;
// // // // // // // // // // // // // // // // //         //             count++;
// // // // // // // // // // // // // // // // //         //         }
// // // // // // // // // // // // // // // // //         //     }
// // // // // // // // // // // // // // // // //         // }
// // // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // // //     freeReplyObject(reply);

// // // // // // // // // // // // // // // // //     redisFree(context);
// // // // // // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // // #include <hiredis/hiredis.h>

// // // // // // // // // // // // // // // // // #include <stdio.h>
// // // // // // // // // // // // // // // // // #include <assert.h>

// // // // // // // // // // // // // // // // // #define REDIS_HOST "127.0.0.1"
// // // // // // // // // // // // // // // // // #define REDIS_PORT 6379

// // // // // // // // // // // // // // // // // int main(void) {
// // // // // // // // // // // // // // // // //     redisContext *c;
// // // // // // // // // // // // // // // // //     redisReply *r;

// // // // // // // // // // // // // // // // //     c = redisConnect(REDIS_HOST, REDIS_PORT);
// // // // // // // // // // // // // // // // //     assert(c != NULL && c->err == 0);

// // // // // // // // // // // // // // // // //     char *redis_key = "show_id:62fda075-4c03-4b88-9894-8ba7498051b1,proceed";

// // // // // // // // // // // // // // // // //     r = redisCommand(c, "SET %s %s", "A", redis_key);
// // // // // // // // // // // // // // // // //     assert(r != NULL && r->type == REDIS_REPLY_STATUS);
// // // // // // // // // // // // // // // // //     printf("reply->str %s\n", r->str ? r->str : "(null)");
// // // // // // // // // // // // // // // // //     freeReplyObject(r);

// // // // // // // // // // // // // // // // //     r = redisCommand(c, "GET %s", "A");
// // // // // // // // // // // // // // // // //     assert(r != NULL && r->type == REDIS_REPLY_STRING);
// // // // // // // // // // // // // // // // //     printf("reply->str %s\n", r->str ? r->str : "(null)");
// // // // // // // // // // // // // // // // //     freeReplyObject(r);

// // // // // // // // // // // // // // // // //     redisFree(c);
// // // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // int main() {
// // // // // // // // // // // // // // // //     redisContext* context = redisConnect("127.0.0.1", 6379);
// // // // // // // // // // // // // // // //     if (context == NULL || context->err) {
// // // // // // // // // // // // // // // //         if (context) {
// // // // // // // // // // // // // // // //             std::cerr << "Error: " << context->errstr << std::endl;
// // // // // // // // // // // // // // // //             redisFree(context);
// // // // // // // // // // // // // // // //         } else {
// // // // // // // // // // // // // // // //             std::cerr << "Can't allocate redis context" << std::endl;
// // // // // // // // // // // // // // // //         }
// // // // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // //     redisReply* reply1 = (redisReply*)redisCommand(context,"auth 123456");
// // // // // // // // // // // // // // // //     // 执行xreadgroup命令
// // // // // // // // // // // // // // // //     redisReply* reply = (redisReply*)redisCommand(context, "xreadgroup group consumer consumer1 count 1 block 0 streams test >");
// // // // // // // // // // // // // // // //     if (reply == NULL) {
// // // // // // // // // // // // // // // //         std::cerr << "Failed to execute xreadgroup command." << std::endl;
// // // // // // // // // // // // // // // //         redisFree(context);
// // // // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // //     if (reply->type == REDIS_REPLY_ARRAY) {
// // // // // // // // // // // // // // // //         for (size_t i = 0; i < reply->elements; ++i) {
// // // // // // // // // // // // // // // //             redisReply* stream_entry = reply->element[i];
// // // // // // // // // // // // // // // //             if (stream_entry->type == REDIS_REPLY_ARRAY && stream_entry->elements == 2) {
// // // // // // // // // // // // // // // //                 redisReply* stream_name_reply = stream_entry->element[0];
// // // // // // // // // // // // // // // //                 cout<<"stream_name : "<<stream_name_reply->str<<endl;
// // // // // // // // // // // // // // // //                 redisReply* messages_reply = stream_entry->element[1];
// // // // // // // // // // // // // // // //                 if (messages_reply->type == REDIS_REPLY_ARRAY) {
// // // // // // // // // // // // // // // //                     for (size_t j = 0; j < messages_reply->elements; ++j) {
// // // // // // // // // // // // // // // //                         redisReply* message_entry = messages_reply->element[j];
// // // // // // // // // // // // // // // //                         if (message_entry->type == REDIS_REPLY_ARRAY && message_entry->elements == 2) {
// // // // // // // // // // // // // // // //                             redisReply* msg_id_reply = message_entry->element[0];
// // // // // // // // // // // // // // // //                             redisReply* msg_fields_reply = message_entry->element[1];
// // // // // // // // // // // // // // // //                             std::cout << "Message ID: " << msg_id_reply->str << std::endl;
// // // // // // // // // // // // // // // //                             if (msg_fields_reply->type == REDIS_REPLY_ARRAY) {
// // // // // // // // // // // // // // // //                                 for (size_t k = 0; k < msg_fields_reply->elements - 1; k += 2) {
// // // // // // // // // // // // // // // //                                     redisReply* field_reply = msg_fields_reply->element[k];
// // // // // // // // // // // // // // // //                                     redisReply* value_reply = msg_fields_reply->element[k + 1];
// // // // // // // // // // // // // // // //                                     std::cout << "Field: " << field_reply->str << ", Value: " << value_reply->str << std::endl;
// // // // // // // // // // // // // // // //                                 }
// // // // // // // // // // // // // // // //                             }
// // // // // // // // // // // // // // // //                         }
// // // // // // // // // // // // // // // //                     }
// // // // // // // // // // // // // // // //                 }
// // // // // // // // // // // // // // // //             }
// // // // // // // // // // // // // // // //         }
// // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // //     else
// // // // // // // // // // // // // // // //     {
// // // // // // // // // // // // // // // //         cout<<reply->len<<endl;
// // // // // // // // // // // // // // // //         // cout<<reply->str<<endl;
// // // // // // // // // // // // // // // //         // std::string temp(reply->str,reply->len);
// // // // // // // // // // // // // // // //         // cout<<temp<<endl;
// // // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // // //     freeReplyObject(reply);
// // // // // // // // // // // // // // // //     redisFree(context);
// // // // // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // // // #include <vector>
// // // // // // // // // // // // // // // #include <cmath>
// // // // // // // // // // // // // // // #include <numeric> // for std::gcd

// // // // // // // // // // // // // // // // Function to compute the modular exponentiation: (base^exp) % mod
// // // // // // // // // // // // // // // long long mod_exp(long long base, long long exp, long long mod) {
// // // // // // // // // // // // // // //     long long result = 1;
// // // // // // // // // // // // // // //     base = base % mod;
// // // // // // // // // // // // // // //     while (exp > 0) {
// // // // // // // // // // // // // // //         if (exp & 1)
// // // // // // // // // // // // // // //             result = (result * base) % mod;
// // // // // // // // // // // // // // //         exp = exp >> 1;
// // // // // // // // // // // // // // //         base = (base * base) % mod;
// // // // // // // // // // // // // // //     }
// // // // // // // // // // // // // // //     return result;
// // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // // Function to find the multiplicative inverse of e modulo phi using Extended Euclidean Algorithm
// // // // // // // // // // // // // // // long long mod_inverse(long long a, long long m) {
// // // // // // // // // // // // // // //     long long m0 = m, t, q;
// // // // // // // // // // // // // // //     long long x0 = 0, x1 = 1;

// // // // // // // // // // // // // // //     if (m == 1)
// // // // // // // // // // // // // // //       return 0;

// // // // // // // // // // // // // // //     while (a > 1) {
// // // // // // // // // // // // // // //         // q is quotient
// // // // // // // // // // // // // // //         q = a / m;
// // // // // // // // // // // // // // //         t = m;

// // // // // // // // // // // // // // //         // m is remainder now, process same as Euclid's algo
// // // // // // // // // // // // // // //         m = a % m, a = t;
// // // // // // // // // // // // // // //         t = x0;
// // // // // // // // // // // // // // //         x0 = x1 - q * x0;
// // // // // // // // // // // // // // //         x1 = t;
// // // // // // // // // // // // // // //     }

// // // // // // // // // // // // // // //     // Make x1 positive
// // // // // // // // // // // // // // //     if (x1 < 0)
// // // // // // // // // // // // // // //        x1 += m0;

// // // // // // // // // // // // // // //     return x1;
// // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // void rsa_example(int p, int q, int e, int plaintext) {
// // // // // // // // // // // // // // //     // Step 2: Compute n = pq
// // // // // // // // // // // // // // //     int n = p * q;

// // // // // // // // // // // // // // //     // Step 3: Compute φ(n) = (p-1)(q-1)
// // // // // // // // // // // // // // //     int phi_n = (p - 1) * (q - 1);

// // // // // // // // // // // // // // //     // Step 4 and 5: Given e, find d such that (d * e) % phi_n = 1
// // // // // // // // // // // // // // //     int d = mod_inverse(e, phi_n);
// // // // // // // // // // // // // // //     std::cout << "私钥 d : " << d << std::endl;

// // // // // // // // // // // // // // //     // RSA Encryption: c = m^e % n
// // // // // // // // // // // // // // //     int ciphertext = mod_exp(plaintext, e, n);
// // // // // // // // // // // // // // //     std::cout << "密文 c : " << ciphertext << std::endl;

// // // // // // // // // // // // // // //     // RSA Decryption: m = c^d % n
// // // // // // // // // // // // // // //     int decrypted_message = mod_exp(ciphertext, d, n);
// // // // // // // // // // // // // // //     std::cout << "明文 e : " << decrypted_message << std::endl;
// // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // // int main() {
// // // // // // // // // // // // // // //     // Data 1
// // // // // // // // // // // // // // //     std::cout << "小素数 :" << std::endl;
// // // // // // // // // // // // // // //     rsa_example(3, 11, 7, 6); // p=3, q=11, e=7, plaintext=6

// // // // // // // // // // // // // // //     // Data 2
// // // // // // // // // // // // // // //     std::cout << "\n大素数 :" << std::endl;
// // // // // // // // // // // // // // //     rsa_example(16979, 27901, 39839, 10000039); // p=16979, q=27901, e=39839, plaintext=10000039

// // // // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // // // }

// // // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // // #include <sys/socket.h>
// // // // // // // // // // // // // // #include <unistd.h>

// // // // // // // // // // // // // // int main()
// // // // // // // // // // // // // // {
// // // // // // // // // // // // // //     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
// // // // // // // // // // // // // //     if (sockfd == -1)
// // // // // // // // // // // // // //     {
// // // // // // // // // // // // // //         std::cerr << "Error creating socket" << std::endl;
// // // // // // // // // // // // // //         return 1;
// // // // // // // // // // // // // //     }

// // // // // // // // // // // // // //     std::cout << "Socket created successfully" << std::endl;
// // // // // // // // // // // // // //     std::string data = "Hello, world!";
// // // // // // // // // // // // // //     send(sockfd,data.c_str(),data.size(),0);
// // // // // // // // // // // // // //     char buff[1023];
// // // // // // // // // // // // // //     recv(sockfd,buff,1023,0);
// // // // // // // // // // // // // //     std::cout<<buff<<std::endl;
// // // // // // // // // // // // // //     close(sockfd);
// // // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // // }

// // // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // // #include <cstring>
// // // // // // // // // // // // // #include <sys/socket.h>
// // // // // // // // // // // // // #include <netinet/in.h>
// // // // // // // // // // // // // #include <unistd.h> // for close()
// // // // // // // // // // // // // #include <vector>

// // // // // // // // // // // // // #define PORT 8080

// // // // // // // // // // // // // int main() {
// // // // // // // // // // // // //     int server_fd, new_socket;
// // // // // // // // // // // // //     struct sockaddr_in address;
// // // // // // // // // // // // //     int opt = 1;
// // // // // // // // // // // // //     int addrlen = sizeof(address);
// // // // // // // // // // // // //     char buffer[1024] = {0};
// // // // // // // // // // // // //     const std::string hello = "vector";
// // // // // // // // // // // // //     std::vector<uint8_t> v(hello.begin(),hello.end());
// // // // // // // // // // // // //     char* p = (char*)v.data();
// // // // // // // // // // // // //     size_t size = v.size();

// // // // // // // // // // // // //     // 创建socket文件描述符
// // // // // // // // // // // // //     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
// // // // // // // // // // // // //         perror("socket failed");
// // // // // // // // // // // // //         exit(EXIT_FAILURE);
// // // // // // // // // // // // //     }

// // // // // // // // // // // // //     // 设置地址重用选项
// // // // // // // // // // // // //     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
// // // // // // // // // // // // //         perror("setsockopt");
// // // // // // // // // // // // //         exit(EXIT_FAILURE);
// // // // // // // // // // // // //     }
// // // // // // // // // // // // //     address.sin_family = AF_INET;
// // // // // // // // // // // // //     address.sin_addr.s_addr = INADDR_ANY;
// // // // // // // // // // // // //     address.sin_port = htons(PORT);

// // // // // // // // // // // // //     // 绑定
// // // // // // // // // // // // //     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
// // // // // // // // // // // // //         perror("bind failed");
// // // // // // // // // // // // //         exit(EXIT_FAILURE);
// // // // // // // // // // // // //     }
// // // // // // // // // // // // //     if (listen(server_fd, 3) < 0) {
// // // // // // // // // // // // //         perror("listen");
// // // // // // // // // // // // //         exit(EXIT_FAILURE);
// // // // // // // // // // // // //     }
// // // // // // // // // // // // //     if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
// // // // // // // // // // // // //         perror("accept");
// // // // // // // // // // // // //         exit(EXIT_FAILURE);
// // // // // // // // // // // // //     }
// // // // // // // // // // // // //     read(new_socket, buffer, 1024);
// // // // // // // // // // // // //     printf("Received from client: %s\n", buffer);
// // // // // // // // // // // // //     // send(new_socket, hello.c_str(), hello.size(), 0);
// // // // // // // // // // // // //     send(new_socket, p,size, 0); // 发送数据 (bytes)
// // // // // // // // // // // // //     printf("Hello message sent\n");
// // // // // // // // // // // // //     close(new_socket);
// // // // // // // // // // // // //     close(server_fd);
// // // // // // // // // // // // //     return 0;
// // // // // // // // // // // // // }

// // // // // // // // // // // // #include <optional>
// // // // // // // // // // // // #include <iostream>
// // // // // // // // // // // // using namespace std;

// // // // // // // // // // // // int main()
// // // // // // // // // // // // {
// // // // // // // // // // // //     optional<int> a = 1;
// // // // // // // // // // // //     int b = a.value();
// // // // // // // // // // // //     cout<<b<<endl;
// // // // // // // // // // // //     return 0;
// // // // // // // // // // // // }

// // // // // // // // #include <cereal/types/string.hpp>
// // // // // // // // #include <cereal/types/variant.hpp>
// // // // // // // // #include <cereal/types/optional.hpp>
// // // // // // // // // #include <cereal/types/vector.hpp>
// // // // // // // // #include <cereal/archives/binary.hpp>
// // // // // // // // #include <cereal/cereal.hpp>
// // // // // // // // #include <iostream>
// // // // // // // // #include <string>
// // // // // // // // #include <vector>
// // // // // // // // #include <optional>
// // // // // // // // #include <variant>
// // // // // // // // using namespace std;

// // // // // // // // class test
// // // // // // // // {
// // // // // // // // public:
// // // // // // // //     void set_string(const string &str) { m_string = str; }
// // // // // // // //     void set_optional(const string &str) { m_optional_string = str; }
// // // // // // // //     // void set_vector(const vector<string> str) { m_vector_string = str; }
// // // // // // // //     void set_variant(const variant<string, int> str) { m_variant_string = str; }
// // // // // // // //     string get_string() const { return m_string; }
// // // // // // // //     optional<string> get_optional() const { return m_optional_string; }
// // // // // // // //     // vector<string> get_vector() const { return m_vector_string; }
// // // // // // // //     variant<string, int> get_variant() const { return m_variant_string; }

// // // // // // // // private:
// // // // // // // //     string m_string;
// // // // // // // //     optional<string> m_optional_string;
// // // // // // // //     // vector<string> m_vector_string;
// // // // // // // //     variant<string, int> m_variant_string;
// // // // // // // // };
// // // // // // // // // 定义 load 函数用于反序列化
// // // // // // // // // template<class Archive>
// // // // // // // // // void load(Archive & ar, test& fc) {
// // // // // // // // //     std::string s;
// // // // // // // // //     optional<string> optional_s;
// // // // // // // // //     // vector<string> vector_s;
// // // // // // // // //     variant<string, int> variant_s;
// // // // // // // // //     ar(s,optional_s,variant_s); // 从归档中读取数
// // // // // // // // //     fc.set_string(s);
// // // // // // // // //     fc.set_optional(optional_s.value_or("default"));
// // // // // // // // //     // fc.set_vector(vector_s);
// // // // // // // // //     fc.set_variant(variant_s);
// // // // // // // // // }

// // // // // // // // // // 定义 save 函数用于序列化
// // // // // // // // // template<class Archive>
// // // // // // // // // void save(Archive & ar, const test& fc){
// // // // // // // // //     // ar(fc.getName(), fc.getAge()); // 将数据写入归档
// // // // // // // // //     ar(fc.get_string(), fc.get_optional(),fc.get_variant())
// // // // // // // // // }

// // // // // // // // // // 特化 cereal 的序列化函数以使用自定义的 load 和 save 函数
// // // // // // // // // namespace cereal {
// // // // // // // // // template<class Archive>
// // // // // // // // // inline void serialize(Archive & ar, test& fc) {
// // // // // // // // //     if (cereal::is_loading<Archive>::value) {
// // // // // // // // //         load(ar, fc);
// // // // // // // // //     } else {
// // // // // // // // //         save(ar, fc);
// // // // // // // // //     }
// // // // // // // // // }
// // // // // // // // // } // namespace cereal

// // // // // // // // vector<uint8_t> serialize_test(const test &t)
// // // // // // // // {
// // // // // // // //     stringstream ss;
// // // // // // // //     {
// // // // // // // //         cereal::BinaryOutputArchive oarchive(ss);
// // // // // // // //         oarchive(CEREAL_NVP(t.get_string()), CEREAL_NVP(t.get_optional()), CEREAL_NVP(t.get_variant()));
// // // // // // // //         // archive(t);
// // // // // // // //     }
// // // // // // // //     auto s = ss.str();
// // // // // // // //     cout << s << endl;
// // // // // // // //     // return vector<uint8_t>(s.begin(), s.end());
// // // // // // // //     return vector<uint8_t>();
// // // // // // // // }

// // // // // // // // test deserialize_test(const vector<uint8_t> &v)
// // // // // // // // {
// // // // // // // //     stringstream ss(std::string(v.begin(), v.end()));
// // // // // // // //     test t;
// // // // // // // //     {
// // // // // // // //         cereal::BinaryInputArchive iarchive(ss);
// // // // // // // //         string s;
// // // // // // // //         string optiona_s;
// // // // // // // //         // vector<string> vector_s;
// // // // // // // //         variant<string, int> variant_s;
// // // // // // // //         iarchive(CEREAL_NVP(s), CEREAL_NVP(optiona_s), CEREAL_NVP(variant_s));
// // // // // // // //         t.set_string(s);
// // // // // // // //         t.set_optional(optiona_s);
// // // // // // // //         // t.set_vector(vector_s);
// // // // // // // //         t.set_variant(variant_s);
// // // // // // // //     }
// // // // // // // //     return t;
// // // // // // // // }

// // // // // // // // int main()
// // // // // // // // {
// // // // // // // //     test t;
// // // // // // // //     t.set_string("hello");
// // // // // // // //     t.set_optional("world");
// // // // // // // //     // t.set_vector({"hello", "world"});
// // // // // // // //     t.set_variant("hello");
// // // // // // // //     try
// // // // // // // //     {
// // // // // // // //         auto v = serialize_test(t);
// // // // // // // //         auto t2 = deserialize_test(v);
// // // // // // // //     }
// // // // // // // //     catch (const std::exception &e)
// // // // // // // //     {
// // // // // // // //         std::cerr << e.what() << '\n';
// // // // // // // //     }

// // // // // // // //     // cout << "string:" << t2.get_string() << endl;
// // // // // // // //     // cout << "optional:" << t2.get_optional().value_or("default") << endl;
// // // // // // // //     // // cout << "vector: ";
// // // // // // // //     // // for (auto &s : t2.get_vector())
// // // // // // // //     // //     cout << s << " ";
// // // // // // // //     // // cout << endl;
// // // // // // // //     // cout << "variant:" << get<string>(t2.get_variant()) << endl;
// // // // // // // //     return 0;
// // // // // // // // }

// // // // // // // // // // #include <cereal/types/string.hpp>
// // // // // // // // // // #include <cereal/archives/json.hpp> // 或者其他档案类型如 binary, xml 等
// // // // // // // // // // #include <sstream>

// // // // // // // // // // // 假设这是你不能修改的固定类
// // // // // // // // // // class FixedClass {
// // // // // // // // // // private:
// // // // // // // // // //     std::string name;
// // // // // // // // // //     int age;

// // // // // // // // // // public:
// // // // // // // // // //     FixedClass() : name("default"), age(0) {}

// // // // // // // // // //     // Getter 和 Setter 方法
// // // // // // // // // //     std::string getName() const { return name; }
// // // // // // // // // //     void setName(const std::string& value) { name = value; }

// // // // // // // // // //     int getAge() const { return age; }
// // // // // // // // // //     void setAge(int value) { age = value; }
// // // // // // // // // // };

// // // // // // // // // // // 定义 load 函数用于反序列化
// // // // // // // // // // template<class Archive>
// // // // // // // // // // void load(Archive & ar, FixedClass& fc) {
// // // // // // // // // //     std::string name;
// // // // // // // // // //     int age;
// // // // // // // // // //     ar(name, age); // 从归档中读取数据
// // // // // // // // // //     fc.setName(name);
// // // // // // // // // //     fc.setAge(age);
// // // // // // // // // // }

// // // // // // // // // // // 定义 save 函数用于序列化
// // // // // // // // // // template<class Archive>
// // // // // // // // // // void save(Archive & ar, const FixedClass& fc) const {
// // // // // // // // // //     ar(fc.getName(), fc.getAge()); // 将数据写入归档
// // // // // // // // // // }

// // // // // // // // // // // 特化 cereal 的序列化函数以使用自定义的 load 和 save 函数
// // // // // // // // // // namespace cereal {
// // // // // // // // // // template<class Archive>
// // // // // // // // // // inline void serialize(Archive & ar, FixedClass& fc) {
// // // // // // // // // //     if (cereal::is_loading<Archive>::value) {
// // // // // // // // // //         load(ar, fc);
// // // // // // // // // //     } else {
// // // // // // // // // //         save(ar, fc);
// // // // // // // // // //     }
// // // // // // // // // // }
// // // // // // // // // // } // namespace cereal

// // // // // // // // // // int main() {
// // // // // // // // // //     // 创建一个对象并设置其属性
// // // // // // // // // //     FixedClass obj;
// // // // // // // // // //     obj.setName("John Doe");
// // // // // // // // // //     obj.setAge(30);

// // // // // // // // // //     // 序列化到JSON字符串
// // // // // // // // // //     std::stringstream ss;
// // // // // // // // // //     {
// // // // // // // // // //         cereal::JSONOutputArchive archive(ss); // 选择适当的档案类型
// // // // // // // // // //         archive(obj); // 使用特化的序列化函数
// // // // // // // // // //     }

// // // // // // // // // //     // 打印序列化后的结果
// // // // // // // // // //     std::cout << ss.str() << std::endl;

// // // // // // // // // //     // 反序列化回另一个对象
// // // // // // // // // //     FixedClass newObj;
// // // // // // // // // //     {
// // // // // // // // // //         cereal::JSONInputArchive archive(ss);
// // // // // // // // // //         archive(newObj); // 使用特化的序列化函数进行反序列化
// // // // // // // // // //     }

// // // // // // // // // //     // 验证反序列化后的值
// // // // // // // // // //     std::cout << "Deserialized: " << newObj.getName() << ", " << newObj.getAge() << std::endl;

// // // // // // // // // //     return 0;
// // // // // // // // // // }

// // // // // // // // // #include <cereal/types/string.hpp>
// // // // // // // // // #include <cereal/types/vector.hpp>
// // // // // // // // // #include <cereal/archives/binary.hpp> // 使用二进制存档
// // // // // // // // // #include <iostream>
// // // // // // // // // #include <sstream>
// // // // // // // // // #include <vector>
// // // // // // // // // class MyClass {
// // // // // // // // // private:
// // // // // // // // //     friend class cereal::access; // 允许 Cereal 访问私有成员

// // // // // // // // //     std::string name;
// // // // // // // // //     int age;
// // // // // // // // //     double height;

// // // // // // // // // public:
// // // // // // // // //     MyClass() = default;
// // // // // // // // //     MyClass(std::string n, int a, double h) : name(n), age(a), height(h) {}

// // // // // // // // //     // 序列化函数
// // // // // // // // //     template <class Archive>
// // // // // // // // //     void serialize(Archive & ar)
// // // // // // // // //     {
// // // // // // // // //         ar(CEREAL_NVP(name), CEREAL_NVP(age), CEREAL_NVP(height));
// // // // // // // // //     }

// // // // // // // // //     // 打印对象信息
// // // // // // // // //     void printInfo() const {
// // // // // // // // //         std::cout << "Name: " << name << ", Age: " << age << ", Height: " << height << "\n";
// // // // // // // // //     }
// // // // // // // // // };

// // // // // // // // // std::vector<uint8_t> serializeToVector(const MyClass& obj) {
// // // // // // // // //     std::ostringstream oss(std::ios::binary); // 创建二进制模式的字符串流
// // // // // // // // //     {
// // // // // // // // //         cereal::BinaryOutputArchive archive(oss); // 创建二进制输出存档
// // // // // // // // //         archive(obj);                              // 序列化对象
// // // // // // // // //     } // 确保在此作用域结束时，存档被正确关闭

// // // // // // // // //     // 将流的内容复制到 vector<uint8_t>, 并进行边界检查
// // // // // // // // //     std::string tempStr = oss.str();
// // // // // // // // //     if (tempStr.size() > std::numeric_limits<size_t>::max()) {
// // // // // // // // //         throw std::length_error("Serialized data exceeds maximum size.");
// // // // // // // // //     }
// // // // // // // // //     std::vector<uint8_t> data(tempStr.begin(), tempStr.end());
// // // // // // // // //     return data;
// // // // // // // // // }

// // // // // // // // // void deserializeFromVector(MyClass& obj, const std::vector<uint8_t>& data) {
// // // // // // // // //     // 边界检查
// // // // // // // // //     if (data.size() > std::numeric_limits<size_t>::max()) {
// // // // // // // // //         throw std::length_error("Deserialized data exceeds maximum size.");
// // // // // // // // //     }

// // // // // // // // //     std::string tempStr(data.begin(), data.end());
// // // // // // // // //     std::istringstream iss(tempStr, std::ios::binary); // 创建二进制模式的字符串流
// // // // // // // // //     {
// // // // // // // // //         cereal::BinaryInputArchive archive(iss); // 创建二进制输入存档
// // // // // // // // //         archive(obj);                             // 反序列化对象
// // // // // // // // //     } // 确保在此作用域结束时，存档被正确关闭
// // // // // // // // // }

// // // // // // // // // int main() {
// // // // // // // // //     // 创建原始对象
// // // // // // // // //     MyClass original("Alice", 30, 1.65);

// // // // // // // // //     // 序列化对象到 vector<uint8_t>
// // // // // // // // //     std::vector<uint8_t> serializedData = serializeToVector(original);

// // // // // // // // //     // 反序列化对象
// // // // // // // // //     MyClass loaded;
// // // // // // // // //     deserializeFromVector(loaded, serializedData);

// // // // // // // // //     // 打印反序列化后的对象信息
// // // // // // // // //     loaded.printInfo(); // 应该输出 Alice 的信息

// // // // // // // // //     return 0;
// // // // // // // // // }
// // // // // // // // // #include <iostream>
// // // // // // // // // #include <fstream>
// // // // // // // // // #include <vector>
// // // // // // // // // #include <string>

// // // // // // // // // using namespace std;

// // // // // // // // // #include <cereal/archives/json.hpp>
// // // // // // // // // #include <cereal/types/vector.hpp>
// // // // // // // // // // See details in http://uscilab.github.io/cereal/stl_support.html

// // // // // // // // // class MyClass {
// // // // // // // // //   int x, y, z;

// // // // // // // // //   class MyOtherClass {
// // // // // // // // //     string name, description;

// // // // // // // // //   public:
// // // // // // // // //     template <class Archive>
// // // // // // // // //     void serialize( Archive &archive )
// // // // // // // // //     {
// // // // // // // // //       archive( CEREAL_NVP( name ), CEREAL_NVP( description ) );
// // // // // // // // //     }
// // // // // // // // //   };

// // // // // // // // //   vector<MyOtherClass> Vector;
// // // // // // // // //   vector<int>          ints;

// // // // // // // // // public:
// // // // // // // // //   template <class Archive>
// // // // // // // // //   void serialize( Archive &archive )
// // // // // // // // //   {
// // // // // // // // //     archive( CEREAL_NVP( x ), CEREAL_NVP( y ), CEREAL_NVP( z ), CEREAL_NVP( ints ) );
// // // // // // // // //   }

// // // // // // // // //   // Add one element to the private vector
// // // // // // // // //   void populateVector( const int value ) {
// // // // // // // // //     ints.push_back( value );
// // // // // // // // //   }
// // // // // // // // // };

// // // // // // // // // int main()
// // // // // // // // // {
// // // // // // // // //   MyClass  MyObject{};

// // // // // // // // //   MyObject.populateVector( 101 );
// // // // // // // // //   MyObject.populateVector( 202 );
// // // // // // // // //   MyObject.populateVector( 303 );

// // // // // // // // //   // For brevity I just print the serialization to the standard output instead of the binary file
// // // // // // // // //   cereal::JSONOutputArchive oarchive( cout );
// // // // // // // // //   oarchive( MyObject );

// // // // // // // // //   return 0;
// // // // // // // // // }

// // // // // // // #include <iostream>
// // // // // // // #include <fstream>
// // // // // // // #include <vector>
// // // // // // // #include <cereal/types/vector.hpp>  // 用于序列化vector类型
// // // // // // // // #include "PersonSerialization.h"

// // // // // // // // 序列化函数，将Person对象序列化为vector<uint8_t>
// // // // // // // template <class T>
// // // // // // // std::vector<uint8_t> serializeToVector(const T& obj) {
// // // // // // //     std::vector<uint8_t> buffer;
// // // // // // //     {
// // // // // // //         // 创建一个基于std::back_insert_iterator的归档对象，用于将数据序列化到vector
// // // // // // //         cereal::BinaryOutputArchive oarchive(cereal::BinaryOutputArchive::create(buffer));
// // // // // // //         oarchive(obj);
// // // // // // //     }
// // // // // // //     return buffer;
// // // // // // // }

// // // // // // // // 反序列化函数，从vector<uint8_t>中还原出Person对象
// // // // // // // template <class T>
// // // // // // // T deserializeFromVector(const std::vector<uint8_t>& buffer) {
// // // // // // //     T result;
// // // // // // //     {
// // // // // // //         // 创建一个基于std::istream的归档对象，从vector数据构造输入流进行反序列化
// // // // // // //         std::stringstream ss;
// // // // // // //         ss.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
// // // // // // //         cereal::BinaryInputArchive iarchive(ss);
// // // // // // //         iarchive(result);
// // // // // // //     }
// // // // // // //     return result;
// // // // // // // }

// // // // // // // int main() {
// // // // // // //     // 创建一个Person对象示例
// // // // // // //     Person person("John Doe", 30);

// // // // // // //     // 序列化Person对象到vector<uint8_t>
// // // // // // //     std::vector<uint8_t> serializedData = serializeToVector(person);
// // // // // // //     std::cout << "Serialized data size: " << serializedData.size() << " bytes" << std::endl;

// // // // // // //     // 反序列化，从vector<uint8_t>还原出Person对象
// // // // // // //     Person deserializedPerson = deserializeFromVector<Person>(serializedData);
// // // // // // //     std::cout << "Deserialized Person: Name = " << deserializedPerson.name << ", Age = " << deserializedPerson.age << std::endl;

// // // // // // //     return 0;
// // // // // // // }

// // // // // // #include <cereal/archives/binary.hpp>
// // // // // // #include <cereal/types/vector.hpp>
// // // // // // #include <iostream>
// // // // // // #include <sstream>
// // // // // // #include <vector>

// // // // // // class MyClass {
// // // // // // public:
// // // // // //     int id;
// // // // // //     std::string name;
// // // // // //     double value;

// // // // // //     // 添加Cereal宏以便能够序列化这个类
// // // // // //     template<class Archive>
// // // // // //     void serialize(Archive & archive)
// // // // // //     {
// // // // // //         archive(id, name, value);
// // // // // //     }
// // // // // // };

// // // // // // int main() {
// // // // // //     // 创建一个MyClass对象实例
// // // // // //     MyClass myObject;
// // // // // //     myObject.id = 42;
// // // // // //     myObject.name = "Example";
// // // // // //     myObject.value = 3.14;

// // // // // //     // 使用stringstream作为中间缓冲区
// // // // // //     std::ostringstream oss(std::ios::binary);

// // // // // //     {
// // // // // //         // 创建二进制输出档案
// // // // // //         cereal::BinaryOutputArchive oarchive(oss);
// // // // // //         // 序列化myObject对象
// // // // // //         oarchive(myObject);
// // // // // //     }

// // // // // //     // 获取二进制数据并存入vector<uint8_t>
// // // // // //     std::vector<uint8_t> binaryData(
// // // // // //         reinterpret_cast<const uint8_t*>(oss.str().data()),
// // // // // //         reinterpret_cast<const uint8_t*>(oss.str().data()) + oss.str().size()
// // // // // //     );

// // // // // //     // 输出结果以验证
// // // // // //     for (uint8_t byte : binaryData) {
// // // // // //         std::cout << static_cast<int>(byte) << " ";
// // // // // //     }
// // // // // //     std::cout << std::endl;

// // // // // //     return 0;
// // // // // // }



// // // // // #include <cereal/archives/binary.hpp>
// // // // // #include <cereal/types/vector.hpp>
// // // // // #include <iostream>
// // // // // #include <sstream>
// // // // // #include <vector>
// // // // // #include <string>

// // // // // // 类定义
// // // // // class MyFixedClass {
// // // // // public:
// // // // //     int id;
// // // // //     std::string name;
// // // // //     double value;

// // // // //     // 假设没有其他成员函数或构造函数
// // // // // };

// // // // // // 序列化支持
// // // // // namespace cereal {
// // // // // template<class Archive>
// // // // // void serialize(Archive & archive, MyFixedClass & obj) {
// // // // //     archive(obj.id, obj.name, obj.value);
// // // // // }
// // // // // }

// // // // // // 重载==运算符以便比较两个MyFixedClass对象
// // // // // bool operator==(const MyFixedClass& lhs, const MyFixedClass& rhs) {
// // // // //     return lhs.id == rhs.id && lhs.name == rhs.name && lhs.value == rhs.value;
// // // // // }

// // // // // int main() {
// // // // //     // 创建一个MyFixedClass对象实例
// // // // //     MyFixedClass myObject;
// // // // //     myObject.id = 42;
// // // // //     myObject.name = "Example";
// // // // //     myObject.value = 3.14;

// // // // //     // 使用stringstream作为中间缓冲区
// // // // //     std::ostringstream oss(std::ios::binary);

// // // // //     {
// // // // //         // 创建二进制输出档案
// // // // //         cereal::BinaryOutputArchive oarchive(oss);
// // // // //         // 序列化myObject对象
// // // // //         oarchive(myObject);
// // // // //     }

// // // // //     // 获取二进制数据并存入vector<uint8_t>
// // // // //     std::vector<uint8_t> binaryData(
// // // // //         reinterpret_cast<const uint8_t*>(oss.str().data()),
// // // // //         reinterpret_cast<const uint8_t*>(oss.str().data()) + oss.str().size()
// // // // //     );

// // // // //     // 输出结果以验证
// // // // //     std::cout << "Serialized data: ";
// // // // //     for (uint8_t byte : binaryData) {
// // // // //         std::cout << static_cast<int>(byte) << " ";
// // // // //     }
// // // // //     std::cout << std::endl;

// // // // //     // 反序列化过程
// // // // //     MyFixedClass deserializedObject;
// // // // //     std::istringstream iss(std::string(binaryData.begin(), binaryData.end()), std::ios::binary);

// // // // //     {
// // // // //         // 创建二进制输入档案
// // // // //         cereal::BinaryInputArchive iarchive(iss);
// // // // //         // 反序列化对象
// // // // //         iarchive(deserializedObject);
// // // // //     }

// // // // //     // 验证反序列化结果
// // // // //     if (myObject == deserializedObject) {
// // // // //         std::cout << "Deserialization successful and verified." << std::endl;
// // // // //     } else {
// // // // //         std::cout << "Deserialization failed or verification failed." << std::endl;
// // // // //     }

// // // // //     return 0;
// // // // // }


// // // // #include <cereal/archives/binary.hpp>
// // // // #include <cereal/types/string.hpp> // 包含字符串类型的支持
// // // // #include <iostream>
// // // // #include <sstream>
// // // // #include <vector>
// // // // #include <string>

// // // // // 类定义
// // // // class MyFixedClass {
// // // // public:
// // // //     int id;
// // // //     std::string name;
// // // //     double value;

// // // //     // 假设没有其他成员函数或构造函数
// // // // };

// // // // // 序列化支持
// // // // namespace cereal {
// // // // template<class Archive>
// // // // void serialize(Archive & archive, MyFixedClass & obj) {
// // // //     archive(obj.id, obj.name, obj.value);
// // // // }
// // // // }

// // // // // 重载==运算符以便比较两个MyFixedClass对象
// // // // bool operator==(const MyFixedClass& lhs, const MyFixedClass& rhs) {
// // // //     return lhs.id == rhs.id && lhs.name == rhs.name && lhs.value == rhs.value;
// // // // }

// // // // int main() {
// // // //     // 创建一个MyFixedClass对象实例
// // // //     MyFixedClass myObject;
// // // //     myObject.id = 42;
// // // //     myObject.name = "Example";
// // // //     myObject.value = 3.14;

// // // //     // 使用stringstream作为中间缓冲区
// // // //     std::ostringstream oss(std::ios::binary);

// // // //     {
// // // //         // 创建二进制输出档案
// // // //         cereal::BinaryOutputArchive oarchive(oss);
// // // //         // 序列化myObject对象
// // // //         oarchive(myObject);
// // // //     }

// // // //     // 获取二进制数据并存入vector<uint8_t>
// // // //     std::vector<uint8_t> binaryData(
// // // //         reinterpret_cast<const uint8_t*>(oss.str().data()),
// // // //         reinterpret_cast<const uint8_t*>(oss.str().data()) + oss.str().size()
// // // //     );

// // // //     // 输出结果以验证
// // // //     std::cout << "Serialized data: ";
// // // //     for (uint8_t byte : binaryData) {
// // // //         std::cout << static_cast<int>(byte) << " ";
// // // //     }
// // // //     std::cout << std::endl;

// // // //     // 反序列化过程
// // // //     MyFixedClass deserializedObject;
// // // //     std::istringstream iss(std::string(binaryData.begin(), binaryData.end()), std::ios::binary);

// // // //     {
// // // //         // 创建二进制输入档案
// // // //         cereal::BinaryInputArchive iarchive(iss);
// // // //         // 反序列化对象
// // // //         iarchive(deserializedObject);
// // // //     }

// // // //     // 验证反序列化结果
// // // //     if (myObject == deserializedObject) {
// // // //         std::cout << "Deserialization successful and verified." << std::endl;
// // // //     } else {
// // // //         std::cout << "Deserialization failed or verification failed." << std::endl;
// // // //     }

// // // //     return 0;
// // // // }


// // // #include <cereal/archives/binary.hpp>
// // // #include <cereal/types/string.hpp> // 包含字符串类型的支持
// // // #include <iostream>
// // // #include <sstream>
// // // #include <vector>
// // // #include <string>
// // // using namespace std;

// // // // 类定义
// // // class MyFixedClass {
// // // public:
// // //     int id;
// // //     std::string name;
// // //     double value;
// // //     void print()
// // //     {
// // //         cout<<"id:"<<id<<" name:"<<name<<" value:"<<value<<endl;
// // //     }
// // //     // 假设没有其他成员函数或构造函数
// // // };

// // // // 序列化支持
// // // namespace cereal {
// // // template<class Archive>
// // // void serialize(Archive & archive, MyFixedClass & obj) {
// // //     archive(obj.id, obj.name, obj.value);
// // // }
// // // }

// // // // 重载==运算符以便比较两个MyFixedClass对象
// // // bool operator==(const MyFixedClass& lhs, const MyFixedClass& rhs) {
// // //     return lhs.id == rhs.id && lhs.name == rhs.name && lhs.value == rhs.value;
// // // }

// // // int main() {
// // //     // 创建一个MyFixedClass对象实例
// // //     MyFixedClass myObject;
// // //     myObject.id = 42;
// // //     myObject.name = "Example";
// // //     myObject.value = 3.14;

// // //     // 使用stringstream作为中间缓冲区
// // //     std::ostringstream oss(std::ios::binary);

// // //     {
// // //         // 创建二进制输出档案
// // //         cereal::BinaryOutputArchive oarchive(oss);
// // //         // 序列化myObject对象
// // //         oarchive(myObject);
// // //     }

// // //     // 获取二进制数据并存入vector<uint8_t>
// // //     std::string serializedString = oss.str();
// // //     std::cout << "Serialized data size: " << serializedString.size() << " bytes" << std::endl;

// // //     std::vector<uint8_t> binaryData(
// // //         reinterpret_cast<const uint8_t*>(serializedString.data()),
// // //         reinterpret_cast<const uint8_t*>(serializedString.data()) + serializedString.size()
// // //     );

// // //     // 输出结果以验证
// // //     std::cout << "Serialized data: ";
// // //     for (uint8_t byte : binaryData) {
// // //         std::cout << static_cast<int>(byte) << " ";
// // //     }
// // //     std::cout << std::endl;

// // //     // 反序列化过程
// // //     MyFixedClass deserializedObject;
// // //     std::istringstream iss(serializedString, std::ios::binary);

// // //     {
// // //         // 创建二进制输入档案
// // //         cereal::BinaryInputArchive iarchive(iss);
// // //         // 反序列化对象
// // //         iarchive(deserializedObject);
// // //     }

// // //     // 验证反序列化结果
// // //     if (myObject == deserializedObject) {
// // //         std::cout << "Deserialization successful and verified." << std::endl;
// // //     } else {
// // //         std::cout << "Deserialization failed or verification failed." << std::endl;
// // //     }

// // //     deserializedObject.print();

// // //     return 0;
// // // }



// // #include <cereal/archives/binary.hpp>
// // #include <cereal/types/string.hpp> // 包含字符串类型的支持
// // #include <cereal/types/vector.hpp>
// // #include <cereal/types/variant.hpp>
// // #include <cereal/types/optional.hpp>
// // #include <iostream>
// // #include <sstream>
// // #include <vector>
// // #include <string>
// // using namespace std;

// // // class test
// // // {
// // // private:
// // //     int id;
// // // public:
// // //     void setid(int id){this->id = id;}
// // //     int getid(){return id;}
// // // };
// // // 类定义
// // class MyFixedClass {
// // private:
// //     int id;
// //     std::string name;
// //     double value;
// //     optional<string> optional_value;
// //     vector<string> string_vector;
// //     variant<int,string> variant_value;
// //     // test t;   

// // public:
// //     // Setters
// //     void setId(int _id) { id = _id; }
// //     void setName(const std::string& _name) { name = _name; }
// //     void setValue(double _value) { value = _value; }
// //     void setOptionalValue(const optional<string>& _optional_value) { optional_value = _optional_value; }
// //     void setStringVector(const vector<string>& _string_vector) { string_vector = _string_vector; }
// //     void setVariantValue(const variant<int,string>& _variant_value) { variant_value = _variant_value; }
// //     // void setTest(const test &t){this->t = t;}
// //     // Getters
// //     int getId() const { return id; }
// //     std::string getName() const { return name; }
// //     double getValue() const { return value; }
// //     vector<string> getStringVector() const { return string_vector; }
// //     variant<int,string> getVariantValue() const { return variant_value; }
// //     optional<string> getOptionalValue() const { return optional_value; }
// //     // test getTest() const{return t;}
// // public:
// //     void print()
// //     {
// //         std::cout << "id: " << id << ", name: " << name << ", value: " << value << std::endl;
// //         cout<<"string_vector: ";
// //         for(auto& s : string_vector)
// //             cout<<s<<" ";
// //             cout<<endl;
// //         cout<<"variant_value: "<<get<string>(variant_value)<<endl;
// //         cout<<"optional_value: "<<optional_value.value_or("default")<<endl;
// //         // cout<<"test中id : " <<t.getid()<<endl;
// //     }
// // };

// // // 序列化支持
// // namespace cereal {
// // /* template<class Archive>
// // void serialize(Archive &archive,test& obj)
// // {
// //     if constexpr(Archive::is_loading::valve)
// //     {
// //         int id;
// //         archive(id);
// //         obj.setid(id);
// //     }
// //     else
// //     {
// //         int id = obj.getid();
// //         archive(id);
// //     }
// // } */
// // template<class Archive>
// // void serialize(Archive & archive, MyFixedClass & obj) {
// //     int id;
// //     std::string name;
// //     double value;
// //     vector<string> string_vector;
// //     variant<int, string> variant_value;
// //     optional<string> optional_value;
// //     // test t;

// //     if constexpr (Archive::is_loading::value) {
// //         archive(id, name, value,string_vector,variant_value,optional_value);
// //         obj.setId(id);
// //         obj.setName(name);
// //         obj.setValue(value);
// //         obj.setStringVector(string_vector);
// //         obj.setVariantValue(variant_value);
// //         obj.setOptionalValue(optional_value);
// //         // obj.setTest(t);
// //     } else {
// //         id = obj.getId();
// //         name = obj.getName();
// //         value = obj.getValue();
// //         string_vector = obj.getStringVector();
// //         variant_value = obj.getVariantValue();
// //         optional_value = obj.getOptionalValue();
// //         // t = obj.getTest();
// //         archive(id, name, value,string_vector,variant_value,optional_value);
// //     }
// // }
// // }

// // // 重载==运算符以便比较两个MyFixedClass对象
// // bool operator==(const MyFixedClass& lhs, const MyFixedClass& rhs) {
// //     return lhs.getId() == rhs.getId() && lhs.getName() == rhs.getName() && lhs.getValue() == rhs.getValue() && lhs.getStringVector() == rhs.getStringVector() && lhs.getVariantValue() == rhs.getVariantValue() && lhs.getOptionalValue() == rhs.getOptionalValue();
// // }

// // int main() {
// //     // 创建一个MyFixedClass对象实例
// //     MyFixedClass myObject;
// //     myObject.setId(42);
// //     myObject.setName("Example");
// //     myObject.setValue(3.14);
// //     myObject.setStringVector({"Hello", "World"});
// //     variant<int,string> variantValue = "Hello, World!";
// //     myObject.setVariantValue(variantValue);
// //     optional<string> optionalString = "Optional String";
// //     myObject.setOptionalValue(optionalString);
// //     // 使用stringstream作为中间缓冲区
// //     std::ostringstream oss(std::ios::binary);

// //     {
// //         // 创建二进制输出档案
// //         cereal::BinaryOutputArchive oarchive(oss);
// //         // 序列化myObject对象
// //         oarchive(myObject);
// //     }

// //     // 获取二进制数据并存入vector<uint8_t>
// //     std::string serializedString = oss.str();
// //     std::cout << "Serialized data size: " << serializedString.size() << " bytes" << std::endl;

// //     std::vector<uint8_t> binaryData(
// //         reinterpret_cast<const uint8_t*>(serializedString.data()),
// //         reinterpret_cast<const uint8_t*>(serializedString.data()) + serializedString.size()
// //     );

// //     // 输出结果以验证
// //     std::cout << "Serialized data: ";
// //     for (uint8_t byte : binaryData) {
// //         std::cout << static_cast<int>(byte) << " ";
// //     }
// //     std::cout << std::endl;

// //     // 反序列化过程
// //     MyFixedClass deserializedObject;
// //     std::istringstream iss(serializedString, std::ios::binary);

// //     {
// //         // 创建二进制输入档案
// //         cereal::BinaryInputArchive iarchive(iss);
// //         // 反序列化对象
// //         iarchive(deserializedObject);
// //     }

// //     // 验证反序列化结果
// //     if (myObject == deserializedObject) {
// //         std::cout << "Deserialization successful and verified." << std::endl;
// //     } else {
// //         std::cout << "Deserialization failed or verification failed." << std::endl;
// //     }
// //     deserializedObject.print();

// //     return 0;
// // }
// #include <cereal/archives/binary.hpp>
// #include <cereal/types/string.hpp> // 包含字符串类型的支持
// #include <iostream>
// #include <sstream>
// #include <vector>
// #include <string>
// using namespace std;

// // 自定义类 NestedClass
// class NestedClass {
// private:
//     int nestedId;
//     std::string nestedName;

// public:
//     // Setters
//     void setNestedId(int _nestedId) { nestedId = _nestedId; }
//     void setNestedName(const std::string& _nestedName) { nestedName = _nestedName; }

//     // Getters
//     int getNestedId() const { return nestedId; }
//     std::string getNestedName() const { return nestedName; }
// };

// // 序列化支持 for NestedClass
// namespace cereal {
// template<class Archive>
// void serialize(Archive & archive, NestedClass & obj) {
//     int nestedId;
//     std::string nestedName;

//     if constexpr (Archive::is_loading::value) {
//         archive(nestedId, nestedName);
//         obj.setNestedId(nestedId);
//         obj.setNestedName(nestedName);
//     } else {
//         nestedId = obj.getNestedId();
//         nestedName = obj.getNestedName();
//         archive(nestedId, nestedName);
//     }
// }
// }

// // 类定义 MyFixedClass
// class MyFixedClass {
// private:
//     int id;
//     std::string name;
//     double value;
//     NestedClass nestedObject;

// public:
//     // Setters
//     void setId(int _id) { id = _id; }
//     void setName(const std::string& _name) { name = _name; }
//     void setValue(double _value) { value = _value; }
//     void setNestedObject(const NestedClass& _nestedObject) { nestedObject = _nestedObject; }

//     // Getters
//     int getId() const { return id; }
//     std::string getName() const { return name; }
//     double getValue() const { return value; }
//     NestedClass getNestedObject() const { return nestedObject; }
//     void print()
//     {
//         cout<<"id:"<<id<<" name:"<<name<<" value:"<<value<<" nestedObject:"<<nestedObject.getNestedId()<<" nestedObject.name:"<<nestedObject.getNestedName()<<endl;
//     }
// };

// // 序列化支持 for MyFixedClass
// namespace cereal {
// template<class Archive>
// void serialize(Archive & archive, MyFixedClass & obj) {
//     int id;
//     std::string name;
//     double value;
//     NestedClass nestedObject;

//     if constexpr (Archive::is_loading::value) {
//         archive(id, name, value, nestedObject);
//         obj.setId(id);
//         obj.setName(name);
//         obj.setValue(value);
//         obj.setNestedObject(nestedObject);
//     } else {
//         id = obj.getId();
//         name = obj.getName();
//         value = obj.getValue();
//         nestedObject = obj.getNestedObject();
//         archive(id, name, value, nestedObject);
//     }
// }
// }

// // 重载==运算符以便比较两个NestedClass对象
// bool operator==(const NestedClass& lhs, const NestedClass& rhs) {
//     return lhs.getNestedId() == rhs.getNestedId() && lhs.getNestedName() == rhs.getNestedName();
// }

// // 重载==运算符以便比较两个MyFixedClass对象
// bool operator==(const MyFixedClass& lhs, const MyFixedClass& rhs) {
//     return lhs.getId() == rhs.getId() &&
//            lhs.getName() == rhs.getName() &&
//            lhs.getValue() == rhs.getValue() &&
//            lhs.getNestedObject() == rhs.getNestedObject();
// }

// int main() {
//     // 创建一个NestedClass对象实例
//     NestedClass nestedObj;
//     nestedObj.setNestedId(100);
//     nestedObj.setNestedName("Nested Example");

//     // 创建一个MyFixedClass对象实例
//     MyFixedClass myObject;
//     myObject.setId(42);
//     myObject.setName("Example");
//     myObject.setValue(3.14);
//     myObject.setNestedObject(nestedObj);

//     // 使用stringstream作为中间缓冲区
//     std::ostringstream oss(std::ios::binary);

//     {
//         // 创建二进制输出档案
//         cereal::BinaryOutputArchive oarchive(oss);
//         // 序列化myObject对象
//         oarchive(myObject);
//     }

//     // 获取二进制数据并存入vector<uint8_t>
//     std::string serializedString = oss.str();
//     std::cout << "Serialized data size: " << serializedString.size() << " bytes" << std::endl;

//     std::vector<uint8_t> binaryData(
//         reinterpret_cast<const uint8_t*>(serializedString.data()),
//         reinterpret_cast<const uint8_t*>(serializedString.data()) + serializedString.size()
//     );

//     // 输出结果以验证
//     std::cout << "Serialized data: ";
//     for (uint8_t byte : binaryData) {
//         std::cout << static_cast<int>(byte) << " ";
//     }
//     std::cout << std::endl;

//     // 反序列化过程
//     MyFixedClass deserializedObject;
//     std::istringstream iss(serializedString, std::ios::binary);

//     {
//         // 创建二进制输入档案
//         cereal::BinaryInputArchive iarchive(iss);
//         // 反序列化对象
//         iarchive(deserializedObject);
//     }

//     // 验证反序列化结果
//     if (myObject == deserializedObject) {
//         std::cout << "Deserialization successful and verified." << std::endl;
//     } else {
//         std::cout << "Deserialization failed or verification failed." << std::endl;
//     }

//     myObject.print();

//     return 0;
// }


#include <iostream>
#include <memory>
#include <vector>
using namespace std;

int main()
{
    std::unique_ptr<std::vector<int>> a = std::make_unique<std::vector<int>>();
    a->push_back(1);
    a->push_back(2);
    std::unique_ptr<std::vector<int>> b = std::move(a);
    if(a == nullptr)
    {
        cout<<"a is nullptr"<<endl;
    }
    else
    {
        cout<<"a is not nullptr"<<endl;
        cout<<"a size is "<<a->size()<<endl;
        a->push_back(3);
        for(auto i:*a)
            cout<<i<<endl;
    }
    return 0;
}