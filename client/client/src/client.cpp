#include "../include/client.h"

void clientonlineio::connectserver()
{
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(m_port);
    if(inet_pton(AF_INET,m_serverip,&address.sin_addr.s_addr) <= 0 )  //服务器ip转化为网络字节序配置address
    {
        std::cerr<<"connectserver: inet_pton error"<<std::endl;
        close(m_socketfd);
        exit(1);
    }
    if(connect(m_socketfd,(struct sockaddr*)&address,sizeof(address)) == -1)
    {
        
        std::cerr<<"connectserver: connect error:"<<errno<<std::endl;
        close(m_socketfd);
        exit(1);
    }
}

void clientonlineio::sendmessage()
{
    // int ret = 0;
    // int sum = 0;
    // while(sum < strlen(send_buffer))
    // {
    //     ret = send(m_socketfd,send_buffer + sum,strlen(send_buffer) - sum,0);
    //     if(ret > 0 )
    //     {
    //         sum += ret;
    //     }
    //     else if (ret = -1)
    //     {
    //         if(errno == EAGAIN || errno == EWOULDBLOCK)
    //         {
    //             std::cerr<<"recvmessage: recv error"<<std::endl;
    //             break;
    //         }
    //         else
    //         {
    //             std::cerr<<"recvmessage: recv error"<<std::endl;
    //             close(m_socketfd);
    //             close(epoll_fd);
    //             exit(1);
    //         }
    //     }
    // }
    // if(sum >= strlen(send_buffer))
    // {
    //     send_all = true;
    // }
    // else
    // {
    //     send_all = false;

    // }
    if(strlen(send_buffer)>0)
    {
        set_blocking(m_socketfd);
        int ret = send(m_socketfd,send_buffer,strlen(send_buffer),0);
        if(ret == -1)
        {
            // std::cerr<<"sendmessage: send error"<<std::endl;
            // close(m_socketfd);
            // close(epoll_fd);
            // exit(1);
             std::cerr<<"sendmessage: send error"<<std::endl;

            std::cerr<<"errno:"<<errno<<std::endl;
            close(m_socketfd);
            close(epoll_fd);
            exit(1);
        }
        set_nonblocking(m_socketfd);
    }
}

void clientonlineio::recvmessage()
{
    set_nonblocking(m_socketfd);
    int sum = 0;
    int ret = 0;
    while(true)
    {
        ret = recv(m_socketfd,recv_buffer + sum,RECV_BUF,0);
        if(ret > 0)
        {
            sum += ret;
        }
        else if (ret == 0)
        {
            std::cerr<<"server close the connection"<<std::endl;
            close(m_socketfd);
            close(epoll_fd);
            exit(1);
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            recv_buffer[sum] = '\0';
            break;
        }
        else
        {
            std::cerr<<"recvmessage: recv error"<<std::endl;
            close(m_socketfd);
            close(epoll_fd);
            exit(1);
        }
    }
    set_blocking(m_socketfd);
}

Client::~Client()
{
    delete m_io;
}

void Client::send()
{
    m_io->sendmessage();
    // m_io->getsendall() = true;
    m_io->clearbuf(m_io->getsendbuf(),SEND_BUF);
}

void Client::recv()
{
    m_io->recvmessage();
    showrecvived();
    m_io->clearbuf(m_io->getrecvbuf(),RECV_BUF);
}

void clientonlineio::epoll_setup()
{
    epoll_fd = epoll_create(1);
    if(epoll_fd == -1)
    {
        std::cerr<<"epoll_setup: epoll_create error"<<std::endl;
        return;
    }
    add_to_epoll(m_socketfd,EPOLLIN|EPOLLET);
    // add_to_epoll(STDIN_FILENO,EPOLLIN|EPOLLET);
}

void clientonlineio::add_to_epoll(int fd,uint32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1) 
    {
        std::cerr<<"add_to_epoll: epoll_ctl error"<<std::endl;
        close(m_socketfd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }
}

void Client::event_loop()
{
    struct epoll_event events[MAX_EVETNS];

    while(true)
    {
        int nfds = epoll_wait(m_io->getepollfd(), events, MAX_EVETNS, -1);
        if(nfds == -1)
        {
            if(errno == EINTR )
            {
                continue;
            }
            else
            {
                std::cerr<<"event_loop: epoll_wait error errno: "<<errno<<std::endl;
                break;
            }
        }
        for(int i = 0;i<nfds;i++)
        {
            if(events[i].data.fd == m_io->getsocketfd())
            {
                if(events[i].events & EPOLLIN)
                {
                    recv();
                }
                // if(events[i].events & EPOLLOUT)
                // {
                //     recv();   
                // }
            }
            // else
            // {
            //     stdin_send();
            // }
        }
    }
}

void Client::run()
{
    m_io->init_fd();
    m_io->connectserver();
    m_io->epoll_setup();
    event_loop();
}

void clientonlineio::set_blocking(int sockfd)
{
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("Fcntl get flags failed");
        exit(EXIT_FAILURE);
    }
    flags &= ~O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        perror("Fcntl set blocking failed");
        exit(EXIT_FAILURE);
    }
}

void Client::showrecvived()
{
    std::cout<<"server:"<<m_io->getrecvbuf()<<std::endl;
}

void clientonlineio::clearbuf(char* buf,int len)
{
    memset(buf,0,len);
}

void clientonlineio::set_nonblocking(int socket_fd)
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

// void Client::stdin_send()
// {
//     if(m_io->getsendall() == false)
//     {
//         return;
//     }
//     int sum = 0;
//     while(true)
//     {
//         ssize_t bytes = read(STDIN_FILENO, m_io->getsendbuf() + sum, SEND_BUF - 1);
//         if(bytes > 0)
//         {
//             sum += bytes;
//         }
//         else if (bytes == 0||errno == EAGAIN||errno == EWOULDBLOCK)
//         {
//             break;
//         }
//         else
//         {
//             std::cerr<<"stdin_send: read error"<<std::endl;
//             close(m_io->getsocketfd());
//             close(m_io->getepollfd());
//             exit(1);
//         }
//     }
//     m_io->getsendbuf()[sum] = '\0';
//     m_io->getsendall() = false;
// }

void clientonlineio::init_fd()
{
    m_socketfd = socket(AF_INET,SOCK_STREAM,0);
    if(m_socketfd == -1)
    {
        std::cerr<<"connectserver: socket_fd error"<<std::endl;
        exit(1);
    }
    // set_nonblocking(STDIN_FILENO);
}

void Client::getsendmsg(const std::string msg)
{

    // 确保字符串长度不超过缓冲区大小减去1（留出空间给 null 终止符）
    size_t copyLength = (msg.length() < SEND_BUF - 1) ? msg.length() : SEND_BUF - 1;

    // 复制字符串到字符数组

    msg.copy(m_io->getsendbuf(), copyLength);

    // 添加 null 终止符
    m_io->getsendbuf()[copyLength] = '\0';
}
