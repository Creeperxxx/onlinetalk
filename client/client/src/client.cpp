#include "../include/client.h"

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



void Client::event_loop()
{
    struct epoll_event events[MAX_EVETNS];
    int epoll_fd = m_io->getepollfd();
    while(!shutdown)
    {
        int nfds = epoll_wait(epoll_fd, events, MAX_EVETNS, -1);
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



void Client::showrecvived()
{
    std::cout<<"server:"<<m_io->getrecvbuf()<<std::endl;
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


void Client::getsendmsg(const std::string msg)
{

    // 确保字符串长度不超过缓冲区大小减去1（留出空间给 null 终止符）
    size_t copyLength = (msg.length() < SEND_BUF - 1) ? msg.length() : SEND_BUF - 1;

    // 复制字符串到字符数组

    msg.copy(m_io->getsendbuf(), copyLength);

    // 添加 null 终止符
    m_io->getsendbuf()[copyLength] = '\0';
}

void Client::exit()
{
    shutdown = true;
    delete m_io;
}