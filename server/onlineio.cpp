#include "onlineio.h"

const int FULL_CONNECT_LENGTH = 10;
const int MAX_EVENTS = 10;
const int ACCEPT_NEW_CONN = 5;

// void onlineio::Event_loop()
// {
//     struct epoll_event evs[MAX_EVENTS];
//     char buf[1024];
//     while(true)
//     {
//         int nfds = epoll_wait(epoll_fd,evs,MAX_EVENTS,-1);
//         if(nfds == -1)
//         {
//             std::cerr<<"Event_listen: epoll_wait error"<<std::endl;
//         }
//         for(int i = 0;i<nfds;++i)
//         {
//             if(evs[i].data.fd == listen_fd)
//             {
//                 //
//                 //处理新连接
//                 // newconnectiondeal();
//                 deal_with_newconn();
//             }
//             else
//             {// 处理已连接的读事件
//                 recv_msg(unlogin_user[evs[i].data.fd]);
                
//                 //处理已连接的事件
//                 // if(evs[i].events & EPOLLIN)
//                 // {
//                 //     int ret = recv(evs[i].data.fd,buf,sizeof(buf),0);
//                 //     if(ret == -1)
//                 //     {
//                 //         std::cerr<<"recv error"<<std::endl;
//                 //         close(evs[i].data.fd);
//                 //         epoll_ctl(epoll_fd,EPOLL_CTL_DEL,evs[i].data.fd,NULL);
//                 //     }
//                 //     else if(ret == 0)
//                 //     {
//                 //         std::cerr<<"client close"<<std::endl;
//                 //         close(evs[i].data.fd);
//                 //         epoll_ctl(epoll_fd,EPOLL_CTL_DEL,evs[i].data.fd,NULL);
//                 //     }
//                 //     else
//                 //     {
//                 //         buf[ret] = '\0';
//                 //         std::cout<<"recv message:"<<buf<<std::endl;
//                 //         send(evs[i].data.fd,buf,strlen(buf),0);
//                 //     }
//                 // }
//             }
//         }
//     }
// }

void onlineio::set_nonblocking(int socket_fd)
{
    int flags = fcntl(socket_fd, F_GETFL, 0); // 获取当前文件描述符的标志
    if (flags == -1)
    {
        perror("fcntl(F_GETFL)");
    }

    flags |= O_NONBLOCK; // 设置非阻塞标志
    if (fcntl(socket_fd, F_SETFL, flags) == -1) 
    {
        perror("fcntl(F_SETFL)");
    }

}  

// void onlineio::newconnectiondeal()
// {
//     struct sockaddr_in client_addr;
//     socklen_t client_len = sizeof(client_addr);
//     int conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
//     if(conn_fd == -1)
//     {
//         std::cerr<<"newconnectiondeal: accept error"<<std::endl;
//         close(listen_fd);
//         return;
//     }
//     //接下来将这个新的连接加入到epoll中
//     struct epoll_event event;
//     event.data.fd = conn_fd;
//     event.events = EPOLLIN|EPOLLET;
//     set_nonblocking(conn_fd);
//     if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,conn_fd,&event) == -1)
//     {
//         std::cerr<<"newconnectiondeal: epoll_ctl error"<<std::endl;
//         close(listen_fd);
//         close(conn_fd);
//         return;
//     }
// }

void onlineio::init_listen_fd(int port)
{
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_fd == -1)
    {
        std::cerr<<"Event_listen: listen_fd error"<<std::endl;
        exit(1);
    }

    struct sockaddr_in address;
    memset(&address,0,sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY); 
    address.sin_port = htons(port);

    if(bind(listen_fd,(struct sockaddr*)&address,sizeof(address)) == -1)
    {
        std::cerr<<"Event_listen: bind error"<<std::endl;
        close(listen_fd);
        exit(1);
    }

    if(listen(listen_fd,FULL_CONNECT_LENGTH) == -1)
    {
        std::cerr<<"Event_listen: listen error"<<std::endl;
        close(listen_fd);
        exit(1);   
    }
}

void onlineio::init_epoll()
{
    epoll_fd = epoll_create(1);
    if(epoll_fd == -1)
    {
        std::cerr<<"Event_listen: epoll_create error"<<std::endl;
        close(listen_fd);
        exit(1);
    }
}

void onlineio::add_to_epoll(int fd,uint32_t events)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = events;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,fd,&event) == -1)
    {
        std::cerr<<"Event_listen: epoll_ctl error"<<std::endl;
        close(listen_fd);
        close(epoll_fd);
        exit(1);
    }
}

void onlineio::init()
{
    init_listen_fd(m_port);
    init_epoll();
    add_to_epoll(listen_fd,EPOLLIN);
}

void onlineio::deal_with_newconn()
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int conn_fd = 0;
    for(int i = 0;i<ACCEPT_NEW_CONN;++i)
    {
        conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
        if(conn_fd < 0 )
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                break;
            }
            else
            {
                std::cerr<<"newconnectiondeal: accept error"<<std::endl;
                continue;
            }
        }
        else
        {
            // m_new_socketfd = conn_fd;
            set_newclientconn(conn_fd);
            // add_unlogin_user();
            newsocketfd_pushqueue(conn_fd);
        }  
    }
}

void onlineio::set_newclientconn(int socketfd)
{
    set_nonblocking(socketfd);
    add_to_epoll(socketfd,EPOLLIN|EPOLLET);
    // socket_queue.push(client_fd);
    // m_new_socketfd = client_fd;
}

void onlineio::send_msg(user* user)
{
    const char* buf = user->get_send_msg().c_str();
    int len = strlen(buf);
    int totalbytes = 0;

    int overtime = 5;
    while(true)
    {
        int ret = send(user->get_socketfd(),buf + totalbytes,len - totalbytes,0);
        if(ret == -1)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                //需求：这里说明缓冲区满了，要继续发送，但如果一直进入这个if，就会一直阻塞，所以要设置超时时间，超时就返回，说明发送失败
                overtime--;
                if(overtime == 0)
                {
                    std::cerr<<"send_msg: overtime"<<std::endl;
                    user->set_isalive(0);
                    return;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                std::cerr<<"send_msg: send error"<<std::endl;
                user->set_isalive(0);
                return;
            }
        }
        else if(ret == 0)
        {
            std::cerr<<"client close"<<std::endl;
            user->set_isalive(0);
            return;
        }
        else
        {
            totalbytes += ret;
            if(totalbytes == len)
            {
                return;
            }
        }
    }
}

// void onlineio::set_blocking(int socket_fd)
// {
//     int flags = fcntl(socket_fd, F_GETFL, 0); // 获取当前文件描述符的标志
//     if (flags == -1)
//     {
//         perror("fcntl(F_GETFL)");
//     }
//     flags &= ~O_NONBLOCK; // 清除非阻塞标志
//     if (fcntl(socket_fd, F_SETFL, flags) == -1)
//     {
//         perror("fcntl(F_SETFL)");
//     }
// }

void onlineio::recv_msg(user* user)
{
    int totalbytes = 0;
    int len = user->get_recv_msg().capacity();
    char buf[len];
    while(true)
    {
        int ret = recv(user->get_socketfd(),buf + totalbytes,len - totalbytes,0);
        if(ret == -1)
        {
            if((errno == EAGAIN) || (errno == EWOULDBLOCK))
            {
                return;
            }
            else
            {
                std::cerr<<"recv_msg: recv error"<<std::endl;
                user->set_isalive(0);
                return;
            }
        }
        else if(ret == 0)
        {
            std::cerr<<"client close"<<std::endl;
            user->set_isalive(0);
            return;
        }
        else
        {
            totalbytes += ret;
            if(totalbytes == len)
            {
                user->set_recv_msg(buf);
                return;
            }
        }
    }
}

// void onlineio::get_newsocketfd(std::queue<int>& socketfd_queue)
// {
//     socketfd_queue = newsocket_queue;
// }


// void onlineio::add_unlogin_user()
// {
//     user* newuser = new user;
//     // int id = generate_unlogin_uid();
//     newuser->init(m_new_socketfd,0);
//     unlogin_user.insert({m_new_socketfd,newuser});
//     m_new_socketfd = 0;
// }

// int onlineio::generate_unlogin_uid()
// {
//     ++unlogin_user_num;
//     return unlogin_user_num;
// }

// void onlineio::sent_return_msg(retmsg* msg)
// {

// }

void onlineio::deleter()
{
    close(epoll_fd);
    close(listen_fd);
}