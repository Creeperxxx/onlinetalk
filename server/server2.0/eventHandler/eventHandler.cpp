#include "eventHandler.h"

const int MAX_EPOLL_EVENTS = 1024;
const int THREAD_NUMS = 10;
const int LISTEN_PORT = 8060;
std::atomic<bool> event_loop_running(true);

void ReactorEventHandler::init()
{
    //其余初始化
    handle_ready_socket_running = true;
    //epoll初始化
    init_epoll();

    //信号处理
    signal(SIGINT,event_loop_running_signal_handler);
    signal(SIGTERM,event_loop_running_signal_handler);

    //网络io模块初始化
    networkio = std::make_shared<NetworkIo>();
    networkio->init(LISTEN_PORT);
    //监听套接字加入epoll
    add_socketfd_to_epoll(networkio->get_listenfd(),EPOLLIN|EPOLLET);

    //线程池初始化
    threadPool = std::make_shared<ThreadPool>(THREAD_NUMS);
    
}

void ReactorEventHandler::event_loop()
{
    struct epoll_event events[MAX_EPOLL_EVENTS];
    int listen_fd = networkio->get_listenfd();
    int nfds = 0;
    while(event_loop_running)
    {
        nfds = epoll_wait(epoll_fd,events,MAX_EPOLL_EVENTS,-1);
        if(nfds == -1)
        {
            perror("epoll_wait");
            deleter();
            exit(EXIT_FAILURE);
        }
        else
        {
            //专门让一个读线程处理ready_sockets
            for(int i = 0;i<nfds;++i)
            {
                ready_sockets.enqueue(i);
            }
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
    if(epoll_fd == -1)
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

void ReactorEventHandler::add_socketfd_to_epoll(int socketfd,uint32_t events)
{
    struct epoll_event event;
    event.data.fd = socketfd;
    event.events = events;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,socketfd,&event) == -1)
    {
        perror("epoll_ctl");
        deleter();
        exit(EXIT_FAILURE);
    }
}

void event_loop_running_signal_handler(int signal)
{
    if (signal == SIGINT || signal == SIGTERM) {
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
    while(true)
    {
        client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_addr_len);
        if(client_fd == -1)
        {
            if(errno == EAGAIN || errno == EWOULDBLOCK)
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
            //将新连接加入epoll
            networkio->set_blocking_mode(client_fd,false);
            add_socketfd_to_epoll(client_fd,EPOLLIN|EPOLLET);
        }
    }
}

void ReactorEventHandler::handle_ready_sockets()
{
    int socketfd = 0;
    int listen_fd = networkio->get_listenfd();
    while(handle_ready_socket_running)
    {
        if(ready_sockets.try_dequeue(socketfd))
        {
            //判断套接字类型
            if(socketfd == listen_fd)
            {
                //处理新连接
                handle_new_connections();
            }
            else
            {
                //读取就绪套接字的数据并放入队列中
            }
        }
        else
        {
            std::this_thread::yield();//队列为空，让出时间片
        }
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