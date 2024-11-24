#include "server.h"

const int THREAD_NUM = 10;
const std::string SQLURL = "tcp://127.0.0.1:3306";
const std::string SQLUSER = "root";
const std::string SQLPASSWD = "";
const std::string SQLDATABASE = "onlinechat";
const ssize_t POOLSIZE = 10;

Server::Server(int port) : m_port(port), connection(new onlineio(m_port))
{
    init();
}

void Server::start()
{
    // connection.Event_listen(m_port);
    event_loop();
}

void Server::init()
{
    connection->init();
    auto temp = bind(&Server::newsocketfd_queuepush,this,std::placeholders::_1);
    connection->set_newsocketfd_pushqueue(temp);

    // msganalyse的初始化
    m_analyser = new msganalyse;
    std::function<int(const std::string &)> temp1 = bind(&Server::get_socketfd_from_username, this, std::placeholders::_1);
    m_analyser->set_get_fd_by_username_func(temp1);

    // taskpackage的初始化
    task = new taskpackage;
    std::function<void(const std::string&,int)> temp2 = bind(&Server::add_login_user, this, std::placeholders::_1, std::placeholders::_2);
    task->set_add_loginuser_from_server(temp2);

    std::function<bool(const std::string &)> temp3 = bind(&Server::is_login, this, std::placeholders::_1);
    task->set_is_login_from_server(temp3);

    // std::function<int(const std::string &)> temp4 = bind(&Server::get_socketfd_from_username, this, std::placeholders::_1);
    task->set_get_fd_by_username(temp1);

    std::function<void(std::string &, int)> temp5 = bind(&Server::send_msg, this, std::placeholders::_1, std::placeholders::_2);
    task->set_send_msg_in_task(temp5);

    std::function<void()> temp6 = [this](){this->read_msg_generate_task();};//mark 不知道为什么无法使用bind
    task->set_read_msg_to_generate_task(temp6);
    
    //线程库初始化
    pool = new ThreadPool(THREAD_NUM);

    //数据库初始化
    MySQLPool = MySQLConnectionPool::getInstance();
    MySQLPool->initializePool(SQLURL,SQLUSER,SQLPASSWD,SQLDATABASE,POOLSIZE);

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
        //测试
        std::cout<<"服务器正在进行事件循环"<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(2));
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
                // 此时，处理完了新连接，将新连接的套接字，加入到未登录的map中
                add_unlogin_user();
            }
            else
            {
                std::lock_guard<std::mutex> lockGuard(inorder_socket_mutex);
                inorder_socket.push(evs[i].data.fd);
                // std::lock_guard<std::mutex> lockGuard(socket_queue_mutex);
                // newsocket_queue.push(evs[i].data.fd);
                // 处理已连接的读事件
                // recv_msg(unlogin_user[evs[i].data.fd]);
                // user *temp_user = get_user_from_socketfd(evs[i].data.fd);
                // std::string temp_msg = R"({"type":"recv"})";
                // m_analyser->set_msg(temp_msg);
                // m_analyser->set_sender_fd(evs[i].data.fd);
                // m_analyser->generate_msg();
                // // 此时，处理过的消息对象，被放置在m_analyser的m_msg_ptr中
                // task->set_msg(m_analyser->get_final_msg());

                // auto ret = task->generate_task();

                // connection->recv_msg(temp_user);

                // m_analyser->set_msg(temp_user->get_recv_msg());
                // m_analyser->set_sender_fd(evs[i].data.fd);
                // m_analyser->generate_msg();
                // // 此时，处理过的消息对象，被放置在m_analyser的m_msg_ptr中
                // task->set_msg(m_analyser->get_final_msg());
                // auto ret = task->generate_task();
                // pool->addTask(std::move(task->takeTask()));

                // 将等待结果包装成一个任务
                // 1. 设置task的msg的type为ret
                // 2. 设置ret_handle，参数有ret，
                // 3. 调用generate函数，调用pool的addtask函数，将任务添加到线程池
                // abstractmsg *ret_msg = new returnmsg;
                // ret_msg->init();
                // task->set_msg(ret_msg);
                // task->set_result(std::move(ret));
                // task->generate_task();

                // task->set_msg()

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
                // newsocket_queue.push(evs[i].data.fd);
            }
        }
        // todo 读任务，将消息读取出来，解析出来，包装成任务，添加到线程池。把以上包装成一个任务放入线程池中
        std::string temp_recv_msg = R"({"type":"recv"})";
        // 生成一个recv任务，该任务将读取所有消息，解析出来，生成任务，添加到线程池中
        auto basicmsg = m_analyser->generate_msg(temp_recv_msg, 0);
        task->generate_task(std::move(basicmsg));
        pool->addTask(std::move(task->takeTask()));

        // 根据future生成ret任务
        std::string temp_ret_msg = R"({"type":"ret"})";
        auto ret_basicmsg = m_analyser->generate_msg(temp_ret_msg, 0);
        task->generate_task(std::move(ret_basicmsg));
        pool->addTask(std::move(task->takeTask()));
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
    // std::queue<int> queue;
    user *add_user = NULL;
    // std::lock_guard<std::mutex> lockGuard(socket_queue_mutex);
    while (!newsocket_queue.empty())
    {
        int socketfd = newsocket_queue.front();
        newsocket_queue.pop();

        add_user = new user;
        add_user->init(socketfd);

        std::lock_guard<std::mutex> lockGuard(unlogin_user_mutex);
        unlogin_user.insert({socketfd, add_user});
    }
    return;
}

user *Server::get_user_from_socketfd(int socketfd)
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
            return NULL;
        }
    }
    return it->second;
}

void Server::add_login_user(const std::string & username,int socketfd)
{
    unlogin_user_mutex.lock();
    auto it = unlogin_user.find(socketfd);
    if (it == unlogin_user.end())
    {
        std::cerr << "error: find user error" << std::endl;
        close(socketfd);
        epoll_ctl(connection->get_epoll_fd(), EPOLL_CTL_DEL, socketfd, NULL);
        unlogin_user_mutex.unlock();
        return;
    }
    unlogin_user.erase(socketfd);
    unlogin_user_mutex.unlock();

    it->second->set_username(username);
    login_user_mutex.lock();
    login_user.insert({socketfd, it->second});
    login_user_mutex.unlock();
    return;
}

bool Server::is_login(const std::string &username)
{
    for (const auto &pair : login_user)
    {
        if (pair.second->get_username() == username)
        {
            return true;
        }
    }
    return false;
}

void Server::read_msg_generate_task()
{
    while (true)
    {
        inorder_socket_mutex.lock();
        if (inorder_socket.empty())
        {
            inorder_socket_mutex.unlock();
            return;
        }
        else
        {
            int socketfd = inorder_socket.front();
            inorder_socket.pop();

            user *temp_user = get_user_from_socketfd(socketfd);
            if(temp_user == NULL)
            {
                continue; 
            }
            connection->recv_msg(temp_user);
            // std::string temp_msg = R"({"type":"recv"})";
            auto basicmsg = m_analyser->generate_msg(temp_user->get_recv_msg(), socketfd);
            // socket_queue_mutex.unlock();
            inorder_socket_mutex.unlock();

            auto result = task->generate_task(std::move(basicmsg));
            pool->addTask(std::move(task->takeTask()));
            // todo future对象的处理
            task->add_future_result(std::move(result));

            // auto lambda = [ret = std::move(result)]()mutable{ return ret.get();};
            // task->addTask(lambda);
            // 这里将recv封装成一个任务，将等待recv的结果封装成一个任务

            // m_analyser->set_msg(temp_msg);
            // m_analyser->set_msg(temp_user->get_recv_msg());
            // m_analyser->set_sender_fd(socketfd);
            // m_analyser->generate_msg();
        }
    }
}

int Server::get_socketfd_from_username(const std::string &username)
{
    for (const auto &pair : login_user)
    {
        if (pair.second->get_username() == username)
        {
            return pair.first;
        }
    }
    return -1;
}

// void Server::add_login_user()
// {
//     bool flag = task->get_latest_username_flag();
//     if (flag)
//     {
//         std::string username = "";
//         int socketfd = 0;
//         task->get_latest_username_fd(username, socketfd);
//         user *add_user = new user;
//         add_user->init(socketfd, 0, username);
//         login_user.insert({socketfd, add_user});
//         task->set_latest_username_flag(false);
//     }
//     return;
// std::string latest_username = taskpackage::get_latest_username();
// int socket_fd = taskpackage::get_utfmap()[latest_username];
// if(login_user.find(socket_fd) != login_user.end())
// {
//     //找到了直接返回
//     return;
// }
// else
// {
//     user* add_user = new user;
//     add_user->init(socket_fd);
//     login_user.insert({socket_fd,add_user});
// }
// }

void Server::send_msg(const std::string &msg, int socketfd)
{
    std::lock_guard<std::mutex> lock(login_user_mutex);
    login_user[socketfd]->set_send_msg(msg);
    connection->send_msg(login_user[socketfd]);
    return;
}

void Server::delete_server()
{
    unlogin_user_mutex.lock();
    for (auto it = unlogin_user.begin(); it!= unlogin_user.end();it++)
    {
        delete it->second;
    }
    unlogin_user_mutex.unlock();

    login_user_mutex.lock();
    for (auto it = login_user.begin(); it!= login_user.end();it++)
    {
        delete it->second;
    }
    login_user_mutex.unlock();

    //网络io
    delete connection;
    //msganalyse
    delete m_analyser;
    //taskpackage
    delete task;
    //mysqlpool
    MySQLPool->destroyInstance();
    //threadpool
    delete pool;
}

void Server::newsocketfd_queuepush(int socketfd)
{
    // std::lock_guard<std::mutex> lock(socket_queue_mutex);
    newsocket_queue.push(socketfd);
    return;
}