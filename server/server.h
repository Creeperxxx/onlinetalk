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
#include <mutex>

class Server
{
public:
    Server(int port);
    void start();
    ~Server();
    void add_login_user(std::string& username,int socketfd); //将登录了的用户加入到已登录map中
    bool is_login(std::string& username); //判断该用户名是否已登录
    int get_socketfd_from_username(std::string& username); //通过用户名找到对应的套接字,没找到返回-1

private:
    void read_msg_generate_task();
    void init();// 初始化,初始化所有属性
    void event_loop();//循环调用epoll_wait函数监听就绪事件
    user* get_user_from_socketfd(int socketfd); //找到已连接的套接字对应的user对象，可能登录了也可能未登录
    onlineio* connection; // 网络io模块
    msganalyse* m_analyser; // 消息分析模块
    ThreadPool* pool; // 线程池
    taskpackage* task; // 任务包装模块
    void add_unlogin_user();//将新建立连接的套接字，加入到未登录map中
    //功能需求：将以登录的用户的username和socketfd的映射加入到msganalyse中

    //以连接并已登录用户
    std::unordered_map<int,user*> login_user;
    //已连接但未登录用户
    std::unordered_map<int,user*> unlogin_user;

    int m_port;
    std::mutex socket_queue_mutex;
    std::queue<int> newsocket_queue;
    bool queue_is_empty;//todo 初始化
    // int unlogin_user_num;
};

