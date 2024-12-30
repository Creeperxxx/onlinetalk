// // // // // #include <iostream>
// // // // #include <hiredis/hiredis.h>
// // // // // #include <thread>
// // // // // #include <chrono>
// // // // #include <string.h>
// // // // // using namespace std;

// // // // void print_redis_reply(redisReply *reply, int indent) {
// // // //     if (!reply) {
// // // //         // std::cout << "NULL reply" << std::endl;
// // // //         printf("null reply\n");
// // // //         return;
// // // //     }

// // // //     // std::string indent_str(indent, ' ');
// // // //     char indent_str[1024];

// // // //     switch (reply->type) {
// // // //         case REDIS_REPLY_STATUS:
// // // //             // std::cout << indent_str << "Status: " << reply->str << std::endl;
// // // //             printf("%s : status: %s\n",indent_str,reply->str);
// // // //             break;
// // // //         case REDIS_REPLY_ERROR:
// // // //             printf("%s : error: %s\n",indent_str,reply->str);
// // // //             break;
// // // //         case REDIS_REPLY_INTEGER:
// // // //             // std::cout << indent_str << "Integer: " << reply->integer << std::endl;
// // // //             printf("%s : integer: %lld\n",indent_str,reply->integer);
// // // //             break;
// // // //         case REDIS_REPLY_STRING:
// // // //             // std::cout << indent_str << "String (" << reply->len << " bytes): ";
// // // //             printf("%s : string (%d bytes): ",indent_str,(int)reply->len);
// // // //             if (reply->str) {
// // // //                 // std::cout << std::string(reply->str, reply->len);
// // // //                 printf("%s\n",reply->str);
// // // //             } else {
// // // //                 // std::cout << "NULL";
// // // //                 printf("NULL\n");
// // // //             }
// // // //             // std::cout << std::endl;
// // // //             break;
// // // //         case REDIS_REPLY_ARRAY:
// // // //             // std::cout << indent_str << "Array (" << reply->elements << " elements):" << std::endl;
// // // //             printf("%s : array (%d elements): \n",indent_str,(int)reply->elements);
// // // //             for (size_t i = 0; i < reply->elements; ++i) {
// // // //                 print_redis_reply(reply->element[i], indent + 2);
// // // //             }
// // // //             break;
// // // //         default:
// // // //             // std::cout << indent_str << "Unknown type" << std::endl;
// // // //             printf("%s : unknown type\n",indent_str);
// // // //             break;
// // // //     }
// // // // }


// // // // int main()
// // // // {
// // // //     redisContext* context = redisConnect("127.0.0.1",6379);
// // // //     if(context == NULL || context->err)
// // // //     {
// // // //         return 1;
// // // //     }
// // // //     redisReply *reply = (redisReply*)(redisCommand(context,"auth 123456"));
// // // //     // print_redis_reply(reply,0);
// // // //     freeReplyObject(reply);
// // // //     reply = (redisReply*)(redisCommand(context,"XADD test * username 123"));
// // // //     print_redis_reply(reply,0);
// // // //     freeReplyObject(reply);
// // // //     reply = (redisReply*)(redisCommand(context,"XREAD COUNT 1 BLOCK 0 STREAMS test 0"));
// // // //     print_redis_reply(reply,0);
// // // //     freeReplyObject(reply);
// // // //     reply = (redisReply*)(redisCommand(context,"XRANGE test - +"));
// // // //     print_redis_reply(reply,0);
// // // //     if(reply == NULL)
// // // //     {
// // // //         // cout<<"reply is null"<<endl;
// // // //         printf("reply is null\n");
// // // //     }
// // // //     else if(reply->type == REDIS_REPLY_ERROR)
// // // //     {
// // // //         // cout<<"reply is error"<<endl;
// // // //         printf("reply is error\n");
// // // //     }
// // // //     else if(reply->type == REDIS_REPLY_ARRAY)
// // // //     {
// // // //         // cout<<reply->elements<<endl;
// // // //         printf("%d\n",(int)reply->elements);
// // // //         for(int i = 0; i<reply->elements; i++)
// // // //         {
// // // //             redisReply *entry = reply->element[i];
// // // //             if(entry->type == REDIS_REPLY_ARRAY && entry->elements == 2)
// // // //             {
// // // //                 redisReply *msg_id = entry->element[0];
// // // //                 redisReply *msg_data = entry->element[1];
// // // //                 // cout<<"messageId:"<<msg_id->str<<endl;
// // // //                 printf("messageId:%s\n",msg_id->str);
// // // //                 if(msg_data->type == REDIS_REPLY_ARRAY)
// // // //                 {
// // // //                     for(int j = 0; j<msg_data->elements; j+=2)
// // // //                     {
// // // //                         redisReply* field =  msg_data->element[j];
// // // //                         redisReply* valve = msg_data->element[j+1];
// // // //                         // cout<<"field:"<<field->str<<" value:"<<valve->str<<endl;
// // // //                         printf("field:%s value:%s\n",field->str,valve->str);
// // // //                     }
// // // //                 }
// // // //             }

// // // //         }
        
// // // //     }
// // // //     freeReplyObject(reply);
// // // //     redisFree(context);
// // // //     return 1;
    
// // // // }



// // // // #include <hiredis/hiredis.h>
// // // // #include <iostream>
// // // // #include <vector>
// // // // #include <string>

// // // // void add_message_to_stream(redisContext *context, const std::string& stream_name, const std::string& field, const std::string& value) {
// // // //     // 构建 XADD 命令
// // // //     redisReply *reply = (redisReply*)redisCommand(context, "XADD %s MAXLEN ~ 100 * %s %s", 
// // // //                                                   stream_name.c_str(), field.c_str(), value.c_str());
    
// // // //     if (reply == nullptr) {
// // // //         std::cerr << "Error adding message to stream: " << context->errstr << "\n";
// // // //         return;
// // // //     }

// // // //     // 检查回复是否为字符串类型，并打印消息ID
// // // //     if (reply->type == REDIS_REPLY_STRING) {
// // // //         std::cout << "Message ID: " << reply->str << "\n";
// // // //     } else {
// // // //         std::cerr << "Unexpected reply type from XADD\n";
// // // //     }

// // // //     freeReplyObject(reply); // 释放回复对象内存
// // // // }


// // // // void read_messages_from_stream(redisContext *context, const std::string& stream_name) {
// // // //     // 构建 XREAD 命令，从最新的消息开始读取
// // // //     redisReply *reply = (redisReply*)redisCommand(context, "XREAD COUNT 1  BLOCK 0 STREAMS %s $", stream_name.c_str());

// // // //     if (reply == nullptr || reply->type != REDIS_REPLY_ARRAY || reply->elements != 1) {
// // // //         std::cerr << "Error reading messages from stream or no new messages.\n";
// // // //         if (reply) freeReplyObject(reply);
// // // //         return;
// // // //     }

// // // //     // 解析回复数组
// // // //     redisReply *stream_reply = reply->element[0]; // 第一个元素是流回复
// // // //     if (stream_reply->type != REDIS_REPLY_ARRAY || stream_reply->elements != 2) {
// // // //         std::cerr << "Unexpected reply structure from XREAD\n";
// // // //         freeReplyObject(reply);
// // // //         return;
// // // //     }

// // // //     // 解析流名称和消息
// // // //     std::string stream_key = std::string(stream_reply->element[0]->str, stream_reply->element[0]->len);
// // // //     redisReply *messages_reply = stream_reply->element[1];

// // // //     if (messages_reply->type != REDIS_REPLY_ARRAY) {
// // // //         std::cerr << "Unexpected messages reply structure\n";
// // // //         freeReplyObject(reply);
// // // //         return;
// // // //     }

// // // //     for (size_t i = 0; i < messages_reply->elements; ++i) {
// // // //         redisReply *message = messages_reply->element[i];
// // // //         if (message->type != REDIS_REPLY_ARRAY || message->elements != 2) {
// // // //             std::cerr << "Unexpected message structure\n";
// // // //             continue;
// // // //         }

// // // //         // 获取消息ID
// // // //         std::string message_id = std::string(message->element[0]->str, message->element[0]->len);

// // // //         // 解析消息内容
// // // //         redisReply *fields_reply = message->element[1];
// // // //         if (fields_reply->type != REDIS_REPLY_ARRAY || fields_reply->elements % 2 != 0) {
// // // //             std::cerr << "Unexpected fields structure\n";
// // // //             continue;
// // // //         }

// // // //         std::cout << "Message ID: " << message_id << "\nFields:\n";
// // // //         for (size_t j = 0; j < fields_reply->elements; j += 2) {
// // // //             std::string field = std::string(fields_reply->element[j]->str, fields_reply->element[j]->len);
// // // //             std::string value = std::string(fields_reply->element[j + 1]->str, fields_reply->element[j + 1]->len);
// // // //             std::cout << "  " << field << ": " << value << "\n";
// // // //         }
// // // //     }

// // // //     freeReplyObject(reply); // 释放回复对象内存
// // // // }

// // // // int main() {
// // // //     // 创建 Redis 连接上下文
// // // //     redisContext *context = redisConnect("127.0.0.1", 6379);
// // // //     if (context == nullptr || context->err) {
// // // //         if (context) {
// // // //             std::cerr << "Connection error: " << context->errstr << "\n";
// // // //             redisFree(context);
// // // //         } else {
// // // //             std::cerr << "Connection error: can't allocate redis context\n";
// // // //         }
// // // //         return 1;
// // // //     }
// // // //     redisReply* reply = (redisReply*)redisCommand(context,"auth 123456");
// // // //     freeReplyObject(reply);

// // // //     // 向 Stream 添加消息
// // // //     add_message_to_stream(context, "mystream", "event", "Hello World");

// // // //     // 从 Stream 读取消息
// // // //     read_messages_from_stream(context, "mystream");

// // // //     // 断开连接
// // // //     redisFree(context);

// // // //     return 0;
// // // // }

// // // // #include <iostream>
// // // // #include <hiredis/hiredis.h>
// // // // using namespace std;
// // // // int main() {
// // // //     // 建立与Redis的连接
// // // //     redisContext* context = redisConnect("127.0.0.1", 6379);
// // // //     if (context == NULL || context->err) {
// // // //         if (context) {
// // // //             std::cerr << "Error: " << context->errstr << std::endl;
// // // //             redisFree(context);
// // // //         } else {
// // // //             std::cerr << "Can't allocate redis context" << std::endl;
// // // //         }
// // // //         return 1;
// // // //     }
// // // //     redisReply *reply1 = (redisReply*)(redisCommand(context , "auth 123456"));
// // // //     freeReplyObject(reply1);

// // // //     redisReply* reply = (redisReply*)(redisCommand(context , "get a"));
// // // //     if(reply == NULL)
// // // //     {
// // // //         cout<<"reply = null"<<endl;
// // // //     }
// // // //     else if (reply->type == REDIS_REPLY_NIL)
// // // //     {
// // // //         cout<<"type为nil"<<endl;
// // // //     }
// // // //     else if (reply->type == REDIS_REPLY_STATUS)
// // // //     {
// // // //         // std::string temp = std::string(reply->str,reply->len);
// // // //         // cout<<"status:"<<temp<<endl;
// // // //         cout<<"lens : "<<reply->len<<endl;
// // // //     }
// // // //     else if (reply->type = REDIS_REPLY_STRING)
// // // //     {
// // // //         cout<<"lens : "<<reply->len<<endl;
// // // //         cout<<"string : "<<reply->str<<endl;
// // // //     }
// // // //     freeReplyObject(reply);
// // // //     return 0;


// // // //     // redisReply* reply1 = (redisReply*)(redisCommand(context,"auth 123456"));
// // // //     // freeReplyObject(reply1);
    
// // // //     // // 消息字段和值
// // // //     // const char* field1 = "message";
// // // //     // const char* value1 = "Hello, Redis Stream!";
// // // //     // // 执行XADD命令
// // // //     // redisReply* reply = (redisReply*)redisCommand(context, "XADD mystream * %s %s", field1, value1);
// // // //     // if (reply == NULL) {
// // // //     //     std::cerr << "Failed to execute XADD command." << std::endl;
// // // //     //     redisFree(context);
// // // //     //     return 1;
// // // //     // }
// // // //     // if (reply->type == REDIS_REPLY_STRING) {
// // // //     //     std::cout << "Message added with ID: " << reply-> << std::endl;
// // // //     // }
// // // //     // freeReplyObject(reply);
// // // //     // redisFree(context);
// // // //     // return 0;
// // // // }]]


// // // #include <hiredis/hiredis.h>
// // // #include <iostream>
// // // #include <string.h>
// // // using namespace std;

// // // // int main() {
// // // //     redisContext* context = redisConnect("127.0.0.1", 6379);
// // // //     if (context == NULL || context->err) {
// // // //         if (context) {
// // // //             std::cerr << "Error: " << context->errstr << std::endl;
// // // //             redisFree(context);
// // // //         } else {
// // // //             std::cerr << "Can't allocate redis context" << std::endl;
// // // //         }
// // // //         return 1;
// // // //     }
// // // //     redisReply* reply = (redisReply*)(redisCommand(context, "auth 123456"));
// // // //     freeReplyObject(reply);
// // // //     reply = (redisReply*)(redisCommand(context, "xadd test1 * username creeper age 20"));
// // // //     if(reply == NULL)
// // // //     {
// // // //         cout<<"reply = null"<<endl;
// // // //     }
// // // //     else if (reply->type = REDIS_REPLY_STRING)
// // // //     {
// // // //         cout<<"reply->str : "<<reply->str<<endl;
// // // //         cout<<"reply->len : "<<reply->len<<endl;
// // // //     }
// // // //     freeReplyObject(reply);
// // // //     reply = (redisReply*)(redisCommand(context , "xrange test1 - +"));
// // // //     int count = 0;
// // // //     if(reply == NULL)
// // // //     {
// // // //         cout<<"reply = null"<<endl;
// // // //     }
// // // //     else if (reply->type = REDIS_REPLY_ARRAY)
// // // //     {
// // // //         for(int i = 0; i<reply->elements; i++)
// // // //         {
// // // //             redisReply* entry = reply->element[i];
// // // //             redisReply* msg_id = entry->element[0];
// // // //             redisReply* msg_data = entry->element[1];
// // // //             for(int j = 0; j<msg_data->elements - 1; j+=2)
// // // //             {
// // // //                 redisReply* field = msg_data->element[j];
// // // //                 redisReply* value = msg_data->element[j+1];
// // // //                 cout<<"序号为："<<count<<"  field :"<<field->str<<"\tvalue : "<<value->str<<endl;
// // // //                 count++;
// // // //             }
// // // //         }



// // // //         // cout<<"reply->elements : "<<reply->elements<<endl;
// // // //         // for(int i = 0; i<reply->elements; i++)
// // // //         // {
// // // //         //     redisReply* entry = reply->element[i];
// // // //         //     cout<<"entry->elements : "<<entry->elements<<endl;
// // // //         //     for(int j = 0; j<entry->elements; j++)
// // // //         //     {
// // // //         //         redisReply* msg_id = entry->element[0];
// // // //         //         cout<<"msg_id : "<<msg_id->str<<endl;
// // // //         //         redisReply* msg_data = entry->element[1];
// // // //         //         for(int k = 0; k<msg_data->elements - 2; k+=2)
// // // //         //         {
// // // //         //             redisReply* field = msg_data->element[k];
// // // //         //             redisReply* value = msg_data->element[k+1];
// // // //         //             cout<<"序号为："<<count<<"  field : "<<field->str<<" value : "<<value->str<<endl;
// // // //         //             count++;
// // // //         //         }
// // // //         //     }
// // // //         // }
// // // //     }
// // // //     freeReplyObject(reply);

// // // //     redisFree(context);
// // // //     return 0;
// // // // }

// // // // #include <hiredis/hiredis.h>

// // // // #include <stdio.h>
// // // // #include <assert.h>

// // // // #define REDIS_HOST "127.0.0.1"
// // // // #define REDIS_PORT 6379


// // // // int main(void) {
// // // //     redisContext *c;
// // // //     redisReply *r;

// // // //     c = redisConnect(REDIS_HOST, REDIS_PORT);
// // // //     assert(c != NULL && c->err == 0);

// // // //     char *redis_key = "show_id:62fda075-4c03-4b88-9894-8ba7498051b1,proceed";

// // // //     r = redisCommand(c, "SET %s %s", "A", redis_key);
// // // //     assert(r != NULL && r->type == REDIS_REPLY_STATUS);
// // // //     printf("reply->str %s\n", r->str ? r->str : "(null)");
// // // //     freeReplyObject(r);

// // // //     r = redisCommand(c, "GET %s", "A");
// // // //     assert(r != NULL && r->type == REDIS_REPLY_STRING);
// // // //     printf("reply->str %s\n", r->str ? r->str : "(null)");
// // // //     freeReplyObject(r);

// // // //     redisFree(c);
// // // // }



// // // int main() {
// // //     redisContext* context = redisConnect("127.0.0.1", 6379);
// // //     if (context == NULL || context->err) {
// // //         if (context) {
// // //             std::cerr << "Error: " << context->errstr << std::endl;
// // //             redisFree(context);
// // //         } else {
// // //             std::cerr << "Can't allocate redis context" << std::endl;
// // //         }
// // //         return 1;
// // //     }
// // //     redisReply* reply1 = (redisReply*)redisCommand(context,"auth 123456");
// // //     // 执行xreadgroup命令
// // //     redisReply* reply = (redisReply*)redisCommand(context, "xreadgroup group consumer consumer1 count 1 block 0 streams test >");
// // //     if (reply == NULL) {
// // //         std::cerr << "Failed to execute xreadgroup command." << std::endl;
// // //         redisFree(context);
// // //         return 1;
// // //     }
// // //     if (reply->type == REDIS_REPLY_ARRAY) {
// // //         for (size_t i = 0; i < reply->elements; ++i) {
// // //             redisReply* stream_entry = reply->element[i];
// // //             if (stream_entry->type == REDIS_REPLY_ARRAY && stream_entry->elements == 2) {
// // //                 redisReply* stream_name_reply = stream_entry->element[0];
// // //                 cout<<"stream_name : "<<stream_name_reply->str<<endl;
// // //                 redisReply* messages_reply = stream_entry->element[1];
// // //                 if (messages_reply->type == REDIS_REPLY_ARRAY) {
// // //                     for (size_t j = 0; j < messages_reply->elements; ++j) {
// // //                         redisReply* message_entry = messages_reply->element[j];
// // //                         if (message_entry->type == REDIS_REPLY_ARRAY && message_entry->elements == 2) {
// // //                             redisReply* msg_id_reply = message_entry->element[0];
// // //                             redisReply* msg_fields_reply = message_entry->element[1];
// // //                             std::cout << "Message ID: " << msg_id_reply->str << std::endl;
// // //                             if (msg_fields_reply->type == REDIS_REPLY_ARRAY) {
// // //                                 for (size_t k = 0; k < msg_fields_reply->elements - 1; k += 2) {
// // //                                     redisReply* field_reply = msg_fields_reply->element[k];
// // //                                     redisReply* value_reply = msg_fields_reply->element[k + 1];
// // //                                     std::cout << "Field: " << field_reply->str << ", Value: " << value_reply->str << std::endl;
// // //                                 }
// // //                             }
// // //                         }
// // //                     }
// // //                 }
// // //             }
// // //         }
// // //     }
// // //     else
// // //     {
// // //         cout<<reply->len<<endl;
// // //         // cout<<reply->str<<endl;
// // //         // std::string temp(reply->str,reply->len);
// // //         // cout<<temp<<endl;
// // //     }
// // //     freeReplyObject(reply);
// // //     redisFree(context);
// // //     return 0;
// // // }


// // #include <iostream>
// // #include <vector>
// // #include <cmath>
// // #include <numeric> // for std::gcd

// // // Function to compute the modular exponentiation: (base^exp) % mod
// // long long mod_exp(long long base, long long exp, long long mod) {
// //     long long result = 1;
// //     base = base % mod;
// //     while (exp > 0) {
// //         if (exp & 1)
// //             result = (result * base) % mod;
// //         exp = exp >> 1;
// //         base = (base * base) % mod;
// //     }
// //     return result;
// // }

// // // Function to find the multiplicative inverse of e modulo phi using Extended Euclidean Algorithm
// // long long mod_inverse(long long a, long long m) {
// //     long long m0 = m, t, q;
// //     long long x0 = 0, x1 = 1;

// //     if (m == 1)
// //       return 0;

// //     while (a > 1) {
// //         // q is quotient
// //         q = a / m;
// //         t = m;

// //         // m is remainder now, process same as Euclid's algo
// //         m = a % m, a = t;
// //         t = x0;
// //         x0 = x1 - q * x0;
// //         x1 = t;
// //     }

// //     // Make x1 positive
// //     if (x1 < 0)
// //        x1 += m0;

// //     return x1;
// // }

// // void rsa_example(int p, int q, int e, int plaintext) {
// //     // Step 2: Compute n = pq
// //     int n = p * q;

// //     // Step 3: Compute φ(n) = (p-1)(q-1)
// //     int phi_n = (p - 1) * (q - 1);

// //     // Step 4 and 5: Given e, find d such that (d * e) % phi_n = 1
// //     int d = mod_inverse(e, phi_n);
// //     std::cout << "私钥 d : " << d << std::endl;

// //     // RSA Encryption: c = m^e % n
// //     int ciphertext = mod_exp(plaintext, e, n);
// //     std::cout << "密文 c : " << ciphertext << std::endl;

// //     // RSA Decryption: m = c^d % n
// //     int decrypted_message = mod_exp(ciphertext, d, n);
// //     std::cout << "明文 e : " << decrypted_message << std::endl;
// // }

// // int main() {
// //     // Data 1
// //     std::cout << "小素数 :" << std::endl;
// //     rsa_example(3, 11, 7, 6); // p=3, q=11, e=7, plaintext=6

// //     // Data 2
// //     std::cout << "\n大素数 :" << std::endl;
// //     rsa_example(16979, 27901, 39839, 10000039); // p=16979, q=27901, e=39839, plaintext=10000039

// //     return 0;
// // }



// #include <iostream>
// #include <sys/socket.h>
// #include <unistd.h>

// int main()
// {
//     int sockfd = socket(AF_INET, SOCK_STREAM, 0);
//     if (sockfd == -1)
//     {
//         std::cerr << "Error creating socket" << std::endl;
//         return 1;
//     }

//     std::cout << "Socket created successfully" << std::endl;
//     std::string data = "Hello, world!";
//     send(sockfd,data.c_str(),data.size(),0);
//     char buff[1023];
//     recv(sockfd,buff,1023,0);
//     std::cout<<buff<<std::endl;
//     close(sockfd);
//     return 0;
// }

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h> // for close()
#include <vector>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const std::string hello = "vector";
    std::vector<uint8_t> v(hello.begin(),hello.end());
    char* p = (char*)v.data();
    size_t size = v.size();

    // 创建socket文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置地址重用选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    read(new_socket, buffer, 1024);
    printf("Received from client: %s\n", buffer);
    // send(new_socket, hello.c_str(), hello.size(), 0);
    send(new_socket, p,size, 0); // 发送数据 (bytes)
    printf("Hello message sent\n");
    close(new_socket);
    close(server_fd);
    return 0;
}