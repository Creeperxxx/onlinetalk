#include "eventHandler.h"

const int MAX_EPOLL_EVENTS = 1024;
const int THREAD_NUMS = 10;
const int LISTEN_PORT = 8060;
const int MAX_DEQUEUE_NUMS = 5;

const int FIND_USERNAME_FAILED = -1;
const int FIND_USER_SOCKET_FAILED = -2;

std::atomic<bool> event_loop_running(true);

void ReactorEventHandler::init()
{
    // 其余初始化
    handle_sockets_recv_running = true;
    handle_sockets_send_running = true;
    analyze_recv_data_running = true;
    // epoll初始化
    init_epoll();

    // 信号处理
    signal(SIGINT, event_loop_running_signal_handler);
    signal(SIGTERM, event_loop_running_signal_handler);

    // 网络io模块初始化
    networkio = std::make_shared<NetworkIo>();
    networkio->init(LISTEN_PORT);
    // 监听套接字加入epoll
    add_socketfd_to_epoll(networkio->get_listenfd(), EPOLLIN | EPOLLET);

    // 线程池初始化
    thread_pool = std::make_shared<ThreadPool>(THREAD_NUMS);

    auto lambda = [this]()
    { this->handle_sockets_recv(); };
    thread_pool->commit(lambda); // 读线程，将就绪事件套接字中的数据读取到对应无锁队列中

    auto lambda1 = [this]()
    { this->handle_sockets_send(); };
    thread_pool->commit(lambda1); // 写线程，将就绪事件套接字中的数据发送

    auto lambda2 = [this](){
        this->analyze_recv_data();
    };
    thread_pool->commit(lambda2); // 分析线程，将就绪事件套接字中的数据分析

    // 序列化类初始化
    serializationMethod = std::make_shared<serializationMethodV1>();

    // 消息分析初始化
    msg_analysis_fsm = std::make_shared<msgAnalysisFSM>();
    msg_analysis_fsm->init(serializationMethod,this,thread_pool);
}

void ReactorEventHandler::event_loop()
{
    struct epoll_event events[MAX_EPOLL_EVENTS];
    int listen_fd = networkio->get_listenfd();
    int nfds = 0;
    std::vector<int> ready_sockets_vec;
    while (event_loop_running)
    {
        nfds = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            deleter();
            exit(EXIT_FAILURE);
        }
        else
        {
            // 专门让一个读线程处理ready_sockets
            for (int i = 0; i < nfds; ++i)
            {
                // ready_sockets.enqueue(events[i].data.fd);
                ready_sockets_vec.push_back(events[i].data.fd);
            }
            ready_sockets.enqueue_bulk(ready_sockets_vec.begin(), ready_sockets_vec.size());
            ready_sockets_vec.clear();
            // // std::lock_guard<std::mutex> lock(ready_sockets_mutex);//用无锁队列代替了锁
            // for(int i = 0;i < nfds;i++)
            // {
            //     if(events[i].data.fd == listen_fd)
            //     {
            //         //让线程池处理新连接
            //         auto lambda = [this](){
            //             handle_new_connections();
            //         };
            //         threadPool->commit(lambda);
            //         // handle_new_connections();
            //     }
            //     else
            //     {
            //         //让线程池处理就绪连接
            //         // handle_ready_connections(events[i].data.fd);
            //         // ready_sockets.push(events[i].data.fd);//这里使用无锁队列

            //     }
            // }
        }
    }
    deleter();
}

void ReactorEventHandler::init_epoll()
{
    this->epoll_fd = epoll_create(1);
    if (epoll_fd == -1)
    {
        perror("epoll_create");
        deleter();
        exit(EXIT_FAILURE);
    }
}

void ReactorEventHandler::deleter()
{
    networkio->deleter();
}

void ReactorEventHandler::add_socketfd_to_epoll(int socketfd, uint32_t events)
{
    struct epoll_event event;
    event.data.fd = socketfd;
    event.events = events;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socketfd, &event) == -1)
    {
        perror("epoll_ctl");
        deleter();
        exit(EXIT_FAILURE);
    }
}

void event_loop_running_signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM)
    {
        event_loop_running = false;
    }
}

// void ReactorEventHandler::accept_new_connection()
// {
//     int listen_fd = networkio->get_listenfd();
//     struct sockaddr_in client_addr;
//     socklen_t client_addr_len = sizeof(client_addr);
//     int client_fd;
//     while(true)
//     {
//         client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_addr_len);
//         if(client_fd == -1)
//         {
//             if(errno == EAGAIN || errno == EWOULDBLOCK)
//             {
//                 break;
//             }
//             else
//             {
//                 perror("accept");
//                 deleter();
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             //将新连接加入epoll
//             networkio->set_blocking_mode(client_fd,false);
//             add_socketfd_to_epoll(client_fd,EPOLLIN|EPOLLET);
//         }
//     }
// }

void ReactorEventHandler::handle_new_connections()
{
    int listen_fd = networkio->get_listenfd();
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    int client_fd;
    while (true)
    {
        client_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd == -1)
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK)
            {
                break;
            }
            else
            {
                perror("accept");
                deleter();
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            // 将新连接加入epoll
            networkio->set_blocking_mode(client_fd, false);
            add_socketfd_to_epoll(client_fd, EPOLLIN | EPOLLET);
        }
    }
}

void ReactorEventHandler::handle_sockets_recv()
{
    int socketfd = 0;
    int listen_fd = networkio->get_listenfd();
    std::shared_ptr<std::vector<int>> ready_sockets_vec = std::make_shared<std::vector<int>>();
    while (handle_sockets_recv_running)
    {
        // n = ready_sockets.size_approx();
        // while (n > 0)
        // {
        //     if (ready_sockets.try_dequeue_bulk(ready_sockets_vec.end(), n))
        //     {
        //         continue;
        //     }
        //     else
        //     {
        //         n /= 2;
        //     }
        // }
        ready_sockets_vec = data_from_concurrentQueue(ready_sockets);

        if (ready_sockets_vec->size() > 0)
        {
            for (auto socketfd : *ready_sockets_vec)
            {
                // 判断套接字类型
                if (socketfd == listen_fd)
                {
                    // 处理新连接
                    handle_new_connections();
                }
                else
                {
                    // 读取就绪套接字的数据并放入对应缓冲区中
                    auto recvmsg = networkio->recv_data(socketfd);
                    if (recvmsg->empty())
                    {
                        // todo 关闭连接
                        close(socketfd);
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socketfd, nullptr);
                    }
                    else
                    {
                        sockets_recv_data[socketfd].enqueue_bulk(recvmsg->data(), recvmsg->size());
                    }
                }
            }
            // ready_sockets_vec->clear();无需清空，共享指针内存自动管理
        }
        else
        {
            // 等待新数据到来
            std::this_thread::yield();
        }
        // int dequeued_count = 0;
        // while(true)
        // {
        //     if(ready_sockets.try_dequeue_bulk(ready_sockets_vec.end(),10))
        //     {
        //         dequeued_count = ready_sockets_vec.size();
        //         continue;
        //     }
        //     else
        //     {
        //         while(true)
        //         {
        //             if(ready_sockets.try_dequeue(ready_sockets_vec.back()))
        //         }
        //     }
        // }
    }
}

// void ReactorEventHandler::handle_ready_connections(int socketfd)
// {
//     //处理就绪连接 读 业务处理 写
//     auto recvmsg = networkio->recv_data(socketfd);
//     if(recvmsg == nullptr)
//     {
//         //断开连接
//         close(socketfd);
//         epoll_ctl(epoll_fd,EPOLL_CTL_DEL,socketfd,nullptr);
//     }
//     else
//     {
//         //处理业务逻辑
//         //...

//         //处理完毕后，将数据发送回去
//     }

// }

void ReactorEventHandler::handle_sockets_send()
{
    int socket = 0;
    auto data = std::make_shared<std::vector<uint8_t>>();
    while (handle_sockets_send_running)
    {
        for (auto &it : sockets_send_data)
        {
            socket = it.first;
            data = data_from_concurrentQueue(it.second);

            if (data->size() > 0)
            {
                // 发送数据
                if (false == networkio->send_data(socket, data))
                {
                    // todo 发送失败
                    close(socket);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket, nullptr);
                    continue;
                }
                else
                {
                    // todo 发送成功
                }
            }
        }
    }
}

void ReactorEventHandler::analyze_recv_data()
{
    auto data = std::make_shared<std::vector<uint8_t>>();
    // int socket = 0;
    // int offset = 0;
    // int data_size = 0;
    // std::shared_ptr<std::vector<uint8_t>> msg;
    // std::shared_ptr<message> msg_ptr;
    while (analyze_recv_data_running)
    {
        for (auto &it : sockets_recv_data)
        {
            socket = it.first;
            // offset = 0;
            data = data_from_concurrentQueue(it.second);
            msg_analysis_fsm->process_data(data)
            // data_size = data->size();
            // if (data_size > 0)
            // {
            //     // 分7步 1. 取出标识符 2. 取出长度 3. 检查校验和 4. 取出消息序列号 5. 取出消息 6. 反序列化消息 7. 放入队列

            //     while (offset < data_size)
            //     {
            //         // 1. 取出标识符
            //         if (memcmp(data->data() + offset, MSG_IDENTIFIER, MSG_IDENTIFIER_SIZE) == 0)
            //         {
            //             offset += MSG_IDENTIFIER_SIZE;
            //             // 2. 取出长度
            //             uint32_t length = 0;
            //             memcpy(&length, data->data() + offset, sizeof(length));
            //             length = ntohl(length);
            //             offset += sizeof(length);
            //             // 3. 检查校验和
            //             uint32_t crc = 0;
            //             memcpy(&crc, data->data() + offset, sizeof(crc));
            //             crc = ntohl(crc);
            //             offset += sizeof(crc);
            //             // 4. 取出消息序列号
            //             uint32_t seq = 0;
            //             memcpy(&seq, data->data() + offset, sizeof(seq));
            //             seq = ntohl(seq);
            //             offset += sizeof(seq);
            //             // 5. 比对校验和并取出消息
            //             if (crc != calculateCRC32(data->data() + offset, length))
            //             {
            //                 // todo 校验失败，重发消息
            //                 continue;
            //             }
            //             // 6. 取出消息
            //             msg->assign(data->begin() + offset, data->begin() + offset + length);
            //             offset += length;
            //             // 7. 序列化
            //             msg_ptr = serializationMethod->deserialize_message(msg);
            //             if (nullptr == msg_ptr)
            //             {
            //                 // todo 反序列化失败
            //                 continue;
            //             }
            //             // 这里对消息调用解析函数然后commit
            //             auto lambda = [this, msg_ptr]()
            //             {
            //                 auto retmsg = this->msgAnalysis->handle(msg_ptr);
            //                 this->enqueue_send_message(retmsg);
            //             };
            //             thread_pool->commit(lambda);
            //         }
            //         else
            //         {
            //             offset++;
            //         }
                // }
            // }
        }
    }
}

// std::shared_ptr<std::vector<uint8_t>> ReactorEventHandler::data_from_concurrentQueue(moodycamel::ConcurrentQueue<uint8_t> &queue)
// {
//     int n = 0;
//     int dequeued_count = MAX_DEQUEUE_NUMS;
//     std::shared_ptr<std::vector<uint8_t>> data = std::make_shared<std::vector<uint8_t>>();
//     do
//     {
//         n = queue.size_approx();
//         if (queue.try_dequeue_bulk(data->end(), n))
//         {
//             --dequeued_count;
//             continue;
//         }
//     } while (n > 0 && dequeued_count > 0);
//     return data;
// }

uint32_t ReactorEventHandler::calculateCRC32(const uint8_t *data, size_t length)
{
    uint32_t crc = crc32(0L, Z_NULL, 0); // 初始化CRC32
    crc = crc32(crc, data, length);      // 计算CRC32
    return crc;
}

int ReactorEventHandler::get_socket_from_username(const std::string &name)
{
}

void ReactorEventHandler::enqueue_send_message(std::shared_ptr<message> data)
{
    std::string username;
    auto msg = serializationMethod->serialize_message(data);
    try
    {
        username = data->getHeader().getReceiverName().value();
    } catch (const std::bad_optional_access &e)
    {
        std::perror("get_socket_from_username");
        return;
    }

    int socket = get_socket_from_username(username);
    if(socket == FIND_USERNAME_FAILED)
    {
        // todo 发送失败
        std::perror("get_socket_from_username");
        return;
    }
    else if (socket == FIND_USER_SOCKET_FAILED)
    {
        //用户未上线
        username_send_data[username].enqueue_bulk(msg->data(), msg->size());
        return;
    }
    sockets_send_data[socket].enqueue_bulk(msg->data(), msg->size());
    return;
}