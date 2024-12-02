//此版以废弃

#pragma once
#include <sys/socket.h>
#include <string.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include "user.h"
// #include <unordered_map>
#include <queue>
#include "taskpackage.h"
// #include <queue>

extern const int FULL_CONNECT_LENGTH;//全连接队列的长度
extern const int MAX_EVENTS; // 最大事件数量
extern const int ACCEPT_NEW_CONN; // 一次接收的最大连接数量

class onlineio
{
public:
    void deleter();
    onlineio(int port):m_port(port){} 
    ~onlineio(){deleter();}
    void send_msg(std::shared_ptr<user> user); //将user中存储的send消息发送出去 
    void recv_msg(std::shared_ptr<user> user); //将套接字中的消息接收并放在user中 
    // int get_new_socketfd(){return m_new_socketfd;}//返回新套接字
    int get_epoll_fd(){return epoll_fd;}
    int get_listen_fd(){return listen_fd;}
    void deal_with_newconn();//调用accept函数接收指定数量的连接，并对其进行设置，然后存储起来
    // void Event_loop();
    void init();
    // void get_newsocketfd(std::queue<int>& socket_queue);//将存放新套接字的队列复制给server中的队列
    void set_newsocketfd_pushqueue(std::function<void(int)> func){newsocketfd_pushqueue = func;}
private:
    // void newconnectiondeal();
    void set_blocking_mode(int socket_fd,bool blocking);// 对套接字进行设置，设置为非阻塞
    // void set_blocking(int socket_fd);
    void init_listen_fd();// 初始化监听套接字
    void init_epoll();// 初始化epoll
    void add_to_epoll(int fd,uint32_t events);// 向epoll中添加一个套接字
    void set_newclientconn(int socketfd);// 初始化新连接的套接字,
    // void add_newsocketfd_to_queue(int socketfd);
    // void add_unlogin_user(); 

    // int generate_unlogin_uid();
    // void send_return_msg(retmsg* msg);

private:
    int listen_fd;
    int epoll_fd;
    int m_port;
    // int m_new_socketfd; // 新连接的socketfd
    // std::queue<int> newsocket_queue;// 存储新连接的套接字
    std::function<void(int)> newsocketfd_pushqueue;
    // std::unordered_map<int,user*> unlogin_user;
    // int unlogin_user_num;
    // std::queue<int> socket_queue;

// public:
    // class Accessor 
    // {
    //     const onlineio& a;

    // public:
    //     explicit Accessor(const onlineio& a) : a(a) {}

    //     // 只公开特定的私有成员函数
    //     void 
    // };
};