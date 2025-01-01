#include "socketNetworkIo.h"
const int FULL_CONNECT_LENGTH = 10;

void socketNetworkIo::init(int listen_port)
{
    this->listen_port = listen_port;
    init_listenfd();
    // set_blocking_mode(listen_fd,false);
    
    // serialization_method = std::make_unique<serializationMethodV1>();
}

void socketNetworkIo::init_listenfd()
{
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
        std::cerr<<"Event_listen: listen_fd error"<<std::endl;//todo异常处理待优化
        exit(1);
    }

    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(listen_port);

    if(bind(listen_fd,(struct sockaddr*)&address,sizeof(address)) == -1)
    {
        std::cerr<<"Event_listen: bind error"<<std::endl;//todo异常处理待优化
        close(listen_fd);
        exit(1);
    }

    if(listen(listen_fd,FULL_CONNECT_LENGTH) == -1)
    {
        std::cerr<<"Event_listen: listen error"<<std::endl;//todo异常处理待优化
        close(listen_fd);
        exit(1);   
    }
}

// void socketNetworkIo::set_blocking_mode(int socket_fd,bool blocking)
// {
//     int flags = fcntl(socket_fd, F_GETFL, 0);
//     if (flags == -1) {
//         std::cerr << "Error getting file status flags: " << strerror(errno) << std::endl;
//     }

//     if (blocking) {
//         // 设置为阻塞模式
//         if (fcntl(socket_fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
//             std::cerr << "Error setting socket to blocking mode: " << strerror(errno) << std::endl;
//         }
//     } else {
//         // 设置为非阻塞模式
//         if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
//             std::cerr << "Error setting socket to non-blocking mode: " << strerror(errno) << std::endl;
//         }
//     }
// }

void socketNetworkIo::deleter()
{
    close(listen_fd);
}

// void NetworkIo::send_data(int socket_fd, std::shared_ptr<message> msg) {
// void NetworkIo::send_data(int socket_fd, std::shared_ptr<std::vector<uint8_t>> data) {
bool socketNetworkIo::send_data_binary(int socket_fd, std::shared_ptr<std::vector<uint8_t>> data) {
    // 序列化

    // auto data = serialization_method->serialize_message(msg);
    // uint32_t length = htonl(static_cast<uint32_t>(data->size()));
    // data->insert(data->begin(),reinterpret_cast<uint8_t*>(&length),reinterpret_cast<uint8_t*>(&length) + sizeof(length));
    //发送数据
    int byte = 0;
    int byte_sum = 0;
    while(byte_sum < data->size())
    {
        byte = send(socket_fd,data->data() + byte_sum,data->size() - byte_sum,0);
        if(byte == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                continue;
            }
            else
            {
                perror("send");//update send失败处理
                return false;
            }
        }
        byte_sum += byte;
    }
    return true;




    // // 创建一个临时缓冲区来存储数据类型和数据
    // std::vector<uint8_t> buffer;
    
    // // 添加数据类型
    // uint8_t type_byte = static_cast<uint8_t>(packet->type);
    // buffer.push_back(type_byte);

    // // 添加数据长度
    // uint32_t length = htonl(static_cast<uint32_t>(packet->data.size()));
    // buffer.insert(buffer.end(), reinterpret_cast<uint8_t*>(&length), reinterpret_cast<uint8_t*>(&length) + sizeof(length));

    // // 添加数据内容
    // buffer.insert(buffer.end(), packet->data.begin(), packet->data.end());

    // // 发送数据
    // ssize_t bytes_sent = send(socket_fd, buffer.data(), buffer.size(), 0);
    // if (bytes_sent == -1) {
    //     perror("send");
    // } else if (static_cast<size_t>(bytes_sent) != buffer.size()) {
    //     std::cerr << "Partial send: " << bytes_sent << " of " << buffer.size() << " bytes" << std::endl;
    // } else {
    //     std::cout << "Sent: " << buffer.size() << " bytes" << std::endl;
    // }
}

// std::vector<uint8_t> NetworkIo::recv_data(int socket_fd) {
std::shared_ptr<std::vector<uint8_t>> socketNetworkIo::recv_data(int socket_fd) {
    // 接收数据
    std::shared_ptr<std::vector<std::uint8_t>> data = std::make_shared<std::vector<uint8_t>>();
    ssize_t byte = 0;
    uint8_t buffer[1024];

    while(true)
    {
        byte = recv(socket_fd,buffer,sizeof(buffer),0);
        if(byte == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                //没有数据可读了
                break;
            }
            else
            {
                //出现错误
                perror("recv");
                break;
            }
        }
        else if(byte == 0)
        {
            //todo 连接关闭处理
        }
        else
        {
            data->insert(data->end(),buffer,buffer + byte);
        }
    }
    return data;


    //反序列化


    // // 读取数据类型
    // uint8_t type_byte;
    // ssize_t bytes_received = recv(socket_fd, &type_byte, 1, 0);
    // if (bytes_received <= 0) {
    //     perror("recv");
    //     return nullptr;
    // }

    // // 读取数据长度
    // uint32_t length;
    // bytes_received = recv(socket_fd, &length, sizeof(length), 0);
    // if (bytes_received <= 0) {
    //     perror("recv");
    //     return nullptr;
    // }
    // length = ntohl(length);

    // // 读取数据内容
    // std::vector<uint8_t> data(length);
    // bytes_received = recv(socket_fd, data.data(), length, 0);//非阻塞套接字只调用一次recv接收数据，可以全部接收吗？
    // if (bytes_received <= 0) {
    //     perror("recv");
    //     return nullptr;
    // }

    // // 返回数据包
    // return std::make_shared<DataPacket>(static_cast<DataType>(type_byte), data);
}

// std::vector<uint8_t> serialize_message(const message& msg){
//     std::stringstream ss;
//     {
//         cereal::BinaryOutputArchive oarchive(ss);

//         // 序列化 messageHeader
//         oarchive(CEREAL_NVP(msg.getHeader().getType()),
//                  CEREAL_NVP(msg.getHeader().getAction()),
//                  CEREAL_NVP(msg.getHeader().getSenderName()),
//                  CEREAL_NVP(msg.getHeader().getSenderId()),
//                 //  CEREAL_NVP(msg.getHeader().getMessageSize()),
//                  CEREAL_NVP(msg.getHeader().getSessionId()),
//                  CEREAL_NVP(msg.getHeader().getGroupId()),
//                  CEREAL_NVP(msg.getHeader().getReceiverId()),
//                  CEREAL_NVP(msg.getHeader().getReceiverName()),
//                  CEREAL_NVP(msg.getHeader().getIsCompressed()));

//         // 序列化 message 数据
//         oarchive(CEREAL_NVP(msg.getData()));
//     }
//     auto str = ss.str();
//     return std::vector<uint8_t>(str.begin(), str.end());
// }

// bool NetworkIo::send_data_string(int socket_fd,const std::string& data)
// {
//     auto res = send()
// }

bool socketNetworkIo::send_data(int socketfd,const char* data,size_t length)
{
    int byte = 0;
    int byte_sum = 0;
    while(byte_sum < length)
    {
        byte = send(socketfd,data + byte_sum,length - byte_sum,0);
        if(byte == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                LOG_WARING("%s:%s:%d // send data eagain or ewouldblock",__FILE__,__FUNCTION__,__LINE__);
                continue;
            }
            else
            {
                LOG_ERROR("%s:%s:%d // send data error",__FILE__,__FUNCTION__,__LINE__);
                return false;
            }
        }
        byte_sum += byte;
    }
    return true;
}

std::shared_ptr<std::vector<uint8_t>> socketNetworkIo::recv_data(int socketfd)
{
    std::shared_ptr<std::vector<uint8_t>> ret_data = std::make_shared<std::vector<uint8_t>>();
    char buffer[1024] = {0};
    int byte = 0;
    while(true)
    {
        byte = recv(socketfd,buffer,sizeof(buffer),0);
        if(byte == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
            {
                LOG_WARING("%s:%s:%d // recv data eagain or ewouldblock",__FILE__,__FUNCTION__,__LINE__);
                continue;
            }
            else
            {
                LOG_ERROR("%s:%s:%d // recv data error",__FILE__,__FUNCTION__,__LINE__);
                //todo 这里要清理套接字
                return ret_data;
            }
        }
        else if(byte == 0)
        {
            //recv函数返回0表示连接已关闭
            LOG_INFO("%s:%s:%d // 对应的socket关闭",__FILE__,__FUNCTION__,__LINE__);
            //todo 这里要清理套接字
            return ret_data;
        }
        else
        {
            ret_data->insert(ret_data->end(),buffer,buffer + byte);
        }
    }
    return ret_data;
}

socketNetworkIo::~socketNetworkIo()
{
    deleter();
}
