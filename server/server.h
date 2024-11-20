#pragma once
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include "onlineio.h"
#include "user.h"
#include <unordered_map>
#include "msganalyse.h"
#include "threadpool.h"

class Server
{
public:
    Server(int port);
    void start();
    ~Server();
private:
    void init();
    void add_unlogin_user();
    void event_loop();
    void deal_read_in_connsocketfd(int socketfd);
    onlineio* connection;
    msganalyse* m_analyser;
    ThreadPool* pool;
    taskpackage* task;

    //以连接并已登录用户
    std::unordered_map<int,user*> login_user;
    //已连接但未登录用户
    std::unordered_map<int,user*> unlogin_user;

    int m_port;
    // int unlogin_user_num;
};

