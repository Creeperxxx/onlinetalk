#pragma once
#include <memory>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include "../logSystem/log.h"
#include "../message/message.h"
// #include "serializationMethod.h"

extern const int FULL_CONNECT_LENGTH;

// enum class DataType
// {
//     TEXT,
//     FILE
// };

// // 定义数据结构
// struct DataPacket {
//     DataType type;
    
//     std::vector<uint8_t> data;

//     // 构造函数
//     DataPacket(DataType type, const std::vector<uint8_t>& data) : type(type), data(data) {}
// };

class NetworkIo
{
public:
    void init(int listen_port);
    void deleter();
    // void send_data(int socketfd,std::shared_ptr<message> msg);//通过套接字发送消息
    // void send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
    bool send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
    // std::vector<uint8_t> recv_data(int socketfd);//通过套接字接收并返回消息
    std::shared_ptr<std::vector<uint8_t>> recv_data(int socketfd);

    void set_blocking_mode(int socket_fd,bool blocking);// 对套接字进行设置，设置为非阻塞

    int get_listenfd(){return listen_fd;}
private:
    void init_listenfd();
    // std::vector<uint8_t> serialize_message(const message& msg);
private:
    // std::unique_ptr<IserializationMethod> serialization_method;
    int listen_fd;//监听套接字
    int listen_port;//监听端口
};




