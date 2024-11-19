#include "server.h"

Server::Server(int port) : m_port(port), connection(new onlineio(m_port))
{
    m_analyser = new msganalyse;
}

void Server::start()
{
    // connection.Event_listen(m_port);
    init();
    event_loop();
}

void Server::init()
{
    connection->init();
}

Server::~Server()
{
    delete connection;
    delete m_analyser;
}

void Server::event_loop()
{
    struct epoll_event evs[MAX_EVENTS];
    // char buf[1024];
    while (true)
    {
        int nfds = epoll_wait(connection->get_epoll_fd(), evs, MAX_EVENTS, -1);
        if (nfds == -1)
        {
            std::cerr << "Event_listen: epoll_wait error" << std::endl;
        }
        for (int i = 0; i < nfds; ++i)
        {
            if (evs[i].data.fd == connection->get_listen_fd())
            {
                //
                // 处理新连接
                // newconnectiondeal();
                connection->deal_with_newconn();
            }
            else
            { // 处理已连接的读事件
                // recv_msg(unlogin_user[evs[i].data.fd]);
                deal_read_in_connsocketfd(evs[i].data.fd);
                //此时，处理过的消息对象，被放置在m_analyser的m_msg_ptr中
                




                
                // 处理已连接的事件
                //  if(evs[i].events & EPOLLIN)
                //  {
                //      int ret = recv(evs[i].data.fd,buf,sizeof(buf),0);
                //      if(ret == -1)
                //      {
                //          std::cerr<<"recv error"<<std::endl;
                //          close(evs[i].data.fd);
                //          epoll_ctl(epoll_fd,EPOLL_CTL_DEL,evs[i].data.fd,NULL);
                //      }
                //      else if(ret == 0)
                //      {
                //          std::cerr<<"client close"<<std::endl;
                //          close(evs[i].data.fd);
                //          epoll_ctl(epoll_fd,EPOLL_CTL_DEL,evs[i].data.fd,NULL);
                //      }
                //      else
                //      {
                //          buf[ret] = '\0';
                //          std::cout<<"recv message:"<<buf<<std::endl;
                //          send(evs[i].data.fd,buf,strlen(buf),0);
                //      }
                //  }
            }
        }
    }
}

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

void Server::add_unlogin_user()
{
    user *newuser = new user;
    newuser->init(connection->get_new_socketfd(), 0);
    unlogin_user.insert({connection->get_new_socketfd(), newuser});
}

void Server::deal_read_in_connsocketfd(int socketfd)
{
    auto it = unlogin_user.find(socketfd);
    if (it == unlogin_user.end())
    {
        it = login_user.find(socketfd);
        if (it == login_user.end())
        {
            std::cerr << "error: find user error" << std::endl;
            close(socketfd);
            epoll_ctl(connection->get_epoll_fd(), EPOLL_CTL_DEL, socketfd, NULL);
            return;
        }
    }
    connection->recv_msg(it->second);
    m_analyser->set_msg(it->second->get_recv_msg());
    m_analyser->generate_msg();
}
