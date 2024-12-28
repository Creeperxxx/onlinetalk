#include "eventHandler.h"



// std::atomic<bool> event_loop_running(true);

void ReactorEventHandler::init()
{
    // 线程池初始化
    thread_pool = std::make_unique<threadPool>(THREAD_NUMS);
    // 日志初始化
    auto lambda = [this](){
        log::get_instance().flush_log();
    };
    thread_pool->commit(lambda);

    
    // LOG_WRITE(logLevel::LOG_LEVEL_INFO, "%s:%s:%d // 开始初始化", __FILE__, __FUNCTION__, __LINE__);
    LOG_INFO("%s:%s:%d // 开始初始化", __FILE__, __FUNCTION__, __LINE__);
    // 其余初始化
    handle_sockets_recv_running.store(true);
    handle_sockets_send_running.store(true);
    analyze_recv_data_running.store(true);
    // epoll初始化
    init_epoll();

    // 信号处理
    // signal(SIGINT, event_loop_running_signal_handler);
    // signal(SIGTERM, event_loop_running_signal_handler);

    // 网络io模块初始化
    networkio = std::make_unique<NetworkIo>();
    networkio->init(LISTEN_PORT);
    // 监听套接字加入epoll
    add_socketfd_to_epoll(networkio->get_listenfd(), EPOLLIN | EPOLLET);

    //socketManager初始化
    // socket_manager = std::make_unique<socketManager>();

        // 消息分析初始化
    msg_analysis_fsm = std::make_unique<msgAnalysisFSM>();
    msg_analysis_fsm->init();
    std::function<void(std::string,std::shared_ptr<std::vector<uint8_t>>)> lambda4 = [this](std::string username,std::shared_ptr<std::vector<uint8_t>> data){
        this->enqueue_send_message(username , data);
    };
    msg_analysis_fsm->register_event(ENQUEUE_SEND_DATA,std::any(lambda4));
    std::function<void(std::function<void()>)> lambda5 = [this](std::function<void()> lambda){
        this->task_commit(lambda);
    };
    msg_analysis_fsm->register_event(TASK_COMMIT,std::any(lambda5));

    auto lambda1 = [this]()
    { this->handle_sockets_recv(); };
    thread_pool->commit(lambda1); // 读线程，将就绪事件套接字中的数据读取到对应无锁队列中

    auto lambda2 = [this]()
    { this->handle_sockets_send(); };
    thread_pool->commit(lambda2); // 写线程，将就绪事件套接字中的数据发送

    auto lambda3 = [this]()
    {
        this->analyze_recv_data();
    };
    thread_pool->commit(lambda3); // 分析线程，将就绪事件套接字中的数据分析

    // 序列化类初始化
    // serializationMethod = std::make_unique<serializationMethodV1>();


    
    //心跳初始化
    heartbeat_running.store(true);
    auto lambda6 = [this](){
        this->heartbeat();
    };
    thread_pool->commit(lambda6);

    load_all_userid();// 加载所有用户id

    // LOG_WRITE(logLevel::LOG_LEVEL_INFO, "%s:%s:%d // 初始化完成", __FILE__, __FUNCTION__, __LINE__);
    LOG_INFO("%s:%s:%d // 初始化完成", __FILE__, __FUNCTION__, __LINE__);
}

void ReactorEventHandler::event_loop()
{
    struct epoll_event events[MAX_EPOLL_EVENTS];
    // int listen_fd = networkio->get_listenfd();
    int nfds = 0;
    // std::queue<int> ready_sockets_que;
    LOG_INFO("%s:%s:%d // 开始事件循环", __FILE__, __FUNCTION__, __LINE__);
    // while (event_loop_running)
    while(true)
    {
        nfds = epoll_wait(epoll_fd, events, MAX_EPOLL_EVENTS, -1);
        if (nfds == -1)
        {
            LOG_ERROR("%s:%s:%d // epoll_wait返回-1", __FILE__, __FUNCTION__, __LINE__);
            continue;
            // deleter();
            // exit(EXIT_FAILURE);
        }
        else
        {
            LOG_INFO("%s:%s:%d // 事件循环，就绪事件数量为：%d", __FILE__, __FUNCTION__, __LINE__, nfds);
            // 专门让一个读线程处理ready_sockets
            std::lock_guard<std::mutex> lock(ready_sockets_mutex);
            for (int i = 0; i < nfds; ++i)
            {
                // ready_sockets.enqueue(events[i].data.fd);
                // ready_sockets_vec.push_back(events[i].data.fd);
                // ready_sockets_que.push(events[i].data.fd);
                // ready_sockets.push(events[i].data.fd);
                ready_sockets.push_back(events[i].data.fd);
            }
            ready_sockets_cv.notify_one();
            // ready_sockets.enqueue_bulk(ready_sockets_vec.begin(), ready_sockets_vec.size());

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
    // deleter();
}

void ReactorEventHandler::init_epoll()
{
    this->epoll_fd = epoll_create(1);
    if (epoll_fd == -1)
    {
        LOG_ERROR("%s:%s:%d // epoll_create返回-1", __FILE__, __FUNCTION__, __LINE__);
        deleter();
        exit(EXIT_FAILURE);
    }
}

void ReactorEventHandler::deleter()
{
    networkio->deleter();
    close(epoll_fd);
    handle_sockets_recv_running.store(false);
    handle_sockets_send_running.store(false);
    analyze_recv_data_running.store(false);
    heartbeat_running.store(false);
    thread_pool->deleter();

}

void ReactorEventHandler::add_socketfd_to_epoll(int socketfd, uint32_t events)
{
    struct epoll_event event;
    event.data.fd = socketfd;
    event.events = events;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socketfd, &event) == -1)
    {
        LOG_ERROR("%s:%s:%d // epoll_ctl返回-1", __FILE__, __FUNCTION__, __LINE__);
        deleter();
        exit(EXIT_FAILURE);
    }
}

// void event_loop_running_signal_handler(int signal)
// {
//     if (signal == SIGINT || signal == SIGTERM)
//     {
//         event_loop_running = false;
//     }
// }

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
    LOG_INFO("%s:%s:%d // 循环处理新连接", __FILE__, __FUNCTION__, __LINE__);
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
                LOG_ERROR("%s:%s:%d // accept返回-1且errno不为eagain ewouldblock", __FILE__, __FUNCTION__, __LINE__);
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
    // int socketfd = 0;
    LOG_INFO("%s:%s:%d // 处理套接字接收的线程开始运作", __FILE__, __FUNCTION__, __LINE__);
    int listen_fd = networkio->get_listenfd();
    // std::shared_ptr<std::vector<int>> ready_sockets_vec = std::make_shared<std::vector<int>>();
    // std::queue<int> ready_sockets_temp;
    std::vector<int> ready_sockets_temp;
    LOG_INFO("%s:%s:%d // 循环处理就绪连接", __FILE__, __FUNCTION__, __LINE__);
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
        // ready_sockets_vec = data_from_concurrentQueue(ready_sockets);
        std::unique_lock<std::mutex> lock(ready_sockets_mutex);
        while (ready_sockets.empty())
        {
            ready_sockets_cv.wait(lock);
        }
        // ready_sockets_temp = std::move(ready_sockets);
        // ready_sockets_temp.swap(ready_sockets);
        // ready_sockets_temp = std::move(ready_sockets);
        ready_sockets_temp.swap(ready_sockets);
        lock.unlock();

        for (auto socketfd : ready_sockets_temp)
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
                    LOG_WARING("%s:%s:%d // 客户端断开连接", __FILE__, __FUNCTION__, __LINE__);
                    close(socketfd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socketfd, nullptr);
                }
                else
                {
                    // sockets_recv_data[socketfd].enqueue_bulk(recvmsg->data(), recvmsg->size());
                    // socket_manager->enqueue_recv_data(socketfd, recvmsg);
                    socketManager::getInstance().enqueue_recv_data(socketfd,recvmsg);
                }
            }
        }
        ready_sockets_temp.clear();
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
    // int socket = 0;
    LOG_INFO("%s:%s:%d // 处理套接字发送的线程开始运作", __FILE__, __FUNCTION__, __LINE__);
    auto data = std::make_shared<std::vector<uint8_t>>();
    std::shared_ptr<std::vector<int>> willbesend_sockets;
    while (handle_sockets_send_running)
    {
        // willbesend_sockets = socket_manager->get_updated_socket_send_vec();
        willbesend_sockets = socketManager::getInstance().get_updated_socket_send_vec();
        for(auto &socket : *willbesend_sockets)
        {
            // data = socket_manager->dequeue_send_data(socket);
            data = socketManager::getInstance().dequeue_send_data(socket);
            if(data == nullptr || data->empty())
            {
                continue;
            }
            else
            {
                if(false == networkio->send_data(socket,data))
                {
                    //发送失败
                    LOG_ERROR("%s:%s:%d // 发送失败",__FILE__,__FUNCTION__,__LINE__);
                    close(socket);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket, nullptr);
                    continue;
                }
                else
                {
                    //todo 发送成功
                }
            }
        }
        // for (auto &it : sockets_send_data)
        // {
        //     socket = it.first;
        //     data = data_from_concurrentQueue(it.second);

        //     if (data->size() > 0)
        //     {
        //         // 发送数据
        //         if (false == networkio->send_data(socket, data))
        //         {
        //             // todo 发送失败
        //             LOG_ERROR("%s:%s:%d // 向客户端发送消息发送失败", __FILE__, __FUNCTION__, __LINE__);
        //             close(socket);
        //             epoll_ctl(epoll_fd, EPOLL_CTL_DEL, socket, nullptr);
        //             continue;
        //         }
        //         else
        //         {
        //             // todo 发送成功
        //         }
        //     }
        // }
    }
}

void ReactorEventHandler::analyze_recv_data()
{
    LOG_INFO("%s:%s:%d // 开始分析接收到的数据", __FILE__, __FUNCTION__, __LINE__);
    auto data = std::make_shared<std::vector<uint8_t>>();
    std::shared_ptr<std::vector<int>> ready_sockets;
    // int socket = 0;
    // int offset = 0;
    // int data_size = 0;
    // std::shared_ptr<std::vector<uint8_t>> msg;
    // std::shared_ptr<message> msg_ptr;
    // LOG_INFO("%s:%s:%d // 开始分析接收到的数据", __FILE__, __FUNCTION__, __LINE__);
    // todo 这里忙等待是否可以优化？
    // 还是回归条件变量+共享状态的方式
    while (analyze_recv_data_running)
    {
        // ready_sockets = socket_manager->get_updated_socket_recv_vec();
        ready_sockets = socketManager::getInstance().get_updated_socket_recv_vec();
        // 处理接收到的数据
        for (auto &socket : *ready_sockets)
        {
            // 处理接收到的数据
            // data = socket_manager->dequeue_recv_data(socket);
            data = socketManager::getInstance().dequeue_recv_data(socket);
            if( nullptr == data || data->empty())
            {
                continue;
            }
            else
            {
                // 处理接收到的数据
                msg_analysis_fsm->process_data(data,socket);
            }
        }


        // for (auto &it : sockets_recv_data)
        // {
        //     socket = it.first;
        //     // offset = 0;
        //     data = data_from_concurrentQueue(it.second);
        //     if (data->empty())
        //     {
        //         continue;
        //     }
        //     msg_analysis_fsm->process_data(data);
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
        // }
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

// uint32_t ReactorEventHandler::calculateCRC32(const uint8_t *data, size_t length)
// {
//     uint32_t crc = crc32(0L, Z_NULL, 0); // 初始化CRC32
//     crc = crc32(crc, data, length);      // 计算CRC32
//     return crc;
// }

// int ReactorEventHandler::get_socket_from_username(const std::string &name)
// {
//     return 0;
// }

int ReactorEventHandler::get_socket_from_userid(const std::string &id)
{
    if(!is_userid_exist(id))
    {
        return USER_NOT_EXIST;
    }
    return socketManager::getInstance().get_socket_by_userid(id);
}

// void ReactorEventHandler::enqueue_send_message(std::string username , std::shared_ptr<std::vector<uint8_t>> data)
void ReactorEventHandler::enqueue_send_message(const std::string &userid, std::shared_ptr<std::vector<uint8_t>> data)
{
    // int socket = get_socket_from_username(username);
    int socket = get_socket_from_userid(userid);
    if (socket == USER_NOT_EXIST)
    {
        // // todo 没找到对应用户
        LOG_WARING("%s:%s:%d // 没找到对应用户", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    if(socket == USER_OFFLINE)
    {
        // 用户未上线
        // socket_manager->enqueue_willsend_data(username,data);

        //用户未上线的处理就是，将消息加入到redis和mysql数据库中，等到用户上线时，根据userid和is_delivered来获取离线消息并发送
        // socketManager::getInstance().enqueue_offline_data(username,data);
        
    }
    
    // socket_manager->enqueue_send_data(socket,data);
    socketManager::getInstance().enqueue_send_data(socket,data);
    
}

// // void ReactorEventHandler::enqueue_send_message(std::shared_ptr<message> data)
// {
//     std::string username;
//     auto msg = serializationMethod->serialize_message(data);

//     try
//     {
//         username = data->getHeader().getReceiverName().value();
//     }
//     catch (const std::bad_optional_access &e)
//     {
//         std::perror("get_socket_from_username");
//         return;
//     }

//     int socket = get_socket_from_username(username);
//     if (socket == FIND_USERNAME_FAILED)
//     {
//         // // todo 没找到对应用户
//         // std::perror("get_socket_from_username");
//         // return;
//     }
//     else if (socket == FIND_USER_SOCKET_FAILED)
//     {
//         // 用户未上线
//         // username_send_data[username].enqueue_bulk(msg->data(), msg->size());
//         socket_manager->enqueue_willsend_data(username,msg);
//         return;
//         // return;
//     }
//     // sockets_send_data[socket].enqueue_bulk(msg->data(), msg->size());
//     socket_manager->enqueue_send_data(socket,msg);
//     return;
// }

void ReactorEventHandler::task_commit(std::function<void()> task)
{
    thread_pool->commit(task);
}

void ReactorEventHandler::heartbeat()
{
    LOG_INFO("%s:%s:%d // 心跳包线程开始运作", __FILE__, __FUNCTION__, __LINE__);
    std::shared_ptr<std::vector<uint8_t>> data;
    int socket = 0;
    while(heartbeat_running.load())
    {
        // socket = socket_manager->get_tobesend_heartbeat_socketfd();
        socket = socketManager::getInstance().get_tobesend_heartbeat_socketfd();
        if(socket == -1)
        {
            // //套接字set为空，为啥？
            // continue;

            //没有过期的套接字
            std::this_thread::sleep_for(std::chrono::seconds(TIME_OUT/2));
        }
        // else if (socket == -2)
        // {
        //     //set中还没有过期的socket
        //     std::this_thread::sleep_for(std::chrono::seconds(TIME_OUT/2));
        // }
        else
        {
            //发送心跳包
            data = get_heartbeat_data(socket);
            if(data == nullptr)
            {
                LOG_ERROR("%s:%s:%d // 序列化心跳包失败", __FILE__, __FUNCTION__, __LINE__);
                continue;
            }
            else
            {
                // socket_manager->enqueue_send_data(socket,data);
                // socket_manager->update_socket_interaction_time(socket);
                socketManager::getInstance().enqueue_send_data(socket,data);
                socketManager::getInstance().update_socket_interaction_time(socket);
            }
        }
    }
}

std::shared_ptr<std::vector<uint8_t>> ReactorEventHandler::get_heartbeat_data(int socketfd)
{
    auto temp_msg = std::make_shared<message>();
    // temp_msg->getHeader().setType(messageType::Notice);
    // temp_msg->getHeader().setAction(messageAction::HEARTBEAT);
    dataHeader d;
    d.setType(messageType::Notice);
    d.setAction(messageAction::HEARTBEAT);
    // const std::string &username = socket_manager->get_username(socketfd);
    const std::string& username = socketManager::getInstance().get_username(socketfd);
    if(username.empty())
    {
        LOG_ERROR("%s:%s:%d // 没找到对应用户", __FILE__, __FUNCTION__, __LINE__);
        return nullptr;
    }
    else
    {
        d.setReceiverName(username);
    }
    temp_msg->setHeader(d);
    return msg_analysis_fsm->serializa_msg(temp_msg);
}

void ReactorEventHandler::run()
{
    // init();
    // event_loop();
    // deleter();

    try
    {
        init();
        event_loop();
    }
    catch(const std::exception& e)
    {
        std::cerr <<"异常："<< e.what() << '\n';
    }
}

ReactorEventHandler::~ReactorEventHandler()
{
    deleter();
}

void ReactorEventHandler::load_all_userid()
{
    auto alluserid_vec = database::get_instance().get_alluserid_from_db();
    if(alluserid_vec == nullptr || alluserid_vec->empty())
    {
        LOG_ERROR("%s:%s:%d // 数据库中没有用户", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    std::lock_guard<std::mutex> lock(all_userid_mutex);
    for(const auto & userid : *alluserid_vec)
    {
        all_userid.insert(userid);
    }
}

void ReactorEventHandler::add_new_userid_to_all_userid(const std::string &userid)
{
    std::lock_guard<std::mutex> lock(all_userid_mutex);
    all_userid.insert(userid);
}

void ReactorEventHandler::delete_userid_from_all_userid(const std::string &userid)
{
    std::lock_guard<std::mutex> lock(all_userid_mutex);
    all_userid.erase(userid);
}

bool ReactorEventHandler::is_userid_exist(const std::string &userid)
{
    std::lock_guard<std::mutex> lock(all_userid_mutex);
    return all_userid.find(userid) != all_userid.end();
}

// bool ReactorEventHandler::is_user_online(const std::string& userid)
// {
//     return socketManager::getInstance()
// }