#pragma once
#include <functional>
#include <queue>
#include <future>
#include <mutex>
#include <memory>
#include "msganalyse.h"
#include "mysqlpool.h"
#include <unordered_map>
#include "user.h"
#include "onlineio.h"


extern const std::string USERNAME;
extern const std::string PASSWORD;
extern const std::string EMAIL;


class retmsg;
class taskpackage
{
    
public:
    using task = std::function<void()>;
    template <typename F, typename... Args>
    auto addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>; 
    task takeTask();                                                         // 取任务
    // bool empty() { return taskqueue.empty(); }
    // void set_msg(abstractmsg* msg){m_msg = msg;}
    auto generate_task(std::unique_ptr<abstractmsg>&& basicmsg)->std::future<std::unique_ptr<retmsg>>;
    // static std::unordered_map<std::string,int>& get_utfmap(){return username_to_fd;}
    // static std::string get_latest_username(){return latest_username;}
    // bool set_latest_username_flag(bool flag = true){latest_username_flag = flag;}
    // bool get_latest_username_flag(){return latest_username_flag;}
    // int get_latest_user_socketfd();
    // void get_latest_username_fd(std::string& username, int& fd);
    // void set_result(std::future<std::unique_ptr<retmsg>>&& future){result = std::move(future);}
    //todo 这两个函数需要在server初始化中被调用
    void set_add_loginuser_from_server(std::function<void(const std::string& ,int)> func){add_loginuser_from_server = func;}
    void set_is_login_from_server(std::function<bool(const std::string&)> func){is_login_from_server = func;}
    void set_get_fd_by_username(std::function<int(const std::string&)> func){get_fd_by_username = func;}
    void add_future_result(std::future<std::unique_ptr<retmsg>>&& future);
    // void set_onlineio_sendmsg(std::function<void(user*)> func){onlineio_sendmsg = func;}
    // void set_get_user_from_socketfd(std::function<user*(int)> func){get_user_from_socketfd = func;}
    void set_send_msg_in_task(std::function<void(std::string&,int)> func){send_msg_in_task = func;}
    void set_read_msg_to_generate_task(std::function<void()> func){read_msg_to_generate_task = func;}

private:
    // static std::unique_ptr<retmsg> login_handle(abstractmsg* msg,MySQLConnectionPool* pool);
    std::unique_ptr<retmsg> login_handle(std::unique_ptr<abstractmsg>&& msg);
    std::unique_ptr<retmsg> register_handle(std::unique_ptr<abstractmsg>&& msg);
    std::unique_ptr<retmsg> chat_handle(std::unique_ptr<abstractmsg>&& msg);
    std::unique_ptr<retmsg> error_handle(std::unique_ptr<abstractmsg>&& msg);
    std::unique_ptr<retmsg> ret_handle(std::unique_ptr<abstractmsg>&& msg);
    std::unique_ptr<retmsg> recv_handle(std::unique_ptr<abstractmsg>&& msg);
    // static std::unique_ptr<retmsg> register_handle(abstractmsg* msg,MySQLConnectionPool* pool);
    // static std::unique_ptr<retmsg> chat_handle(abstractmsg* msg,MySQLConnectionPool* pool);
    // static std::unique_ptr<retmsg> error_handle(abstractmsg* msg,MySQLConnectionPool* pool);
    // static std::unique_ptr<retmsg> ret_handle();
    // static std::unique_ptr<retmsg> send_handle();
    // static void add_username_to_fd(const std::string username,int fd);
    // static int get_fd_by_username(const std::string& username);
    // void login_result(bool flag);
private:
    // static bool latest_username_flag;
    // static std::string latest_username;
    // static int latest_socketfd;
    // static std::unordered_map<std::string,int> username_to_fd;
    // static std::future<std::unique_ptr<retmsg>> result;
    std::queue<task> taskqueue; 
    std::mutex taskQueueMutex; // 互斥锁，防止多个线程同时访问任务队列
    // abstractmsg* m_msg;
    // MySQLConnectionPool* pool;
    // onlineio* connection;

    std::function<void(const std::string&,int)> add_loginuser_from_server;
    std::function<bool(const std::string&)> is_login_from_server;
    std::function<int(const std::string&)> get_fd_by_username;    
    // std::function<void(user*)> onlineio_sendmsg;
    // std::function<user*(int)> get_user_from_socketfd;
    std::function<void(std::string&,int)> send_msg_in_task;
    std::function<void()> read_msg_to_generate_task;
    std::mutex future_result_queue_mutex;
    std::queue<std::future<std::unique_ptr<retmsg>>> future_result_queue;
};

template <typename F, typename... Args> // 可变参数模板，模板必须在头文件定义!!!
auto taskpackage::addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>
{
    using RetType = decltype(f(args...));
    // 获取函数返回值类型
    // 将函数封装为无形参的类型 std::bind(f, std::forward<Args>(args)...)：将参数与函数名绑定
    // packaged_task<RetType()>(std::bind(f, std::forward<Args>(args)...)); 将绑定参数后的函数封装为只有返回值没有形参的任务对象，这样就能使用get_future得到future对象，然后future对象可以通过get方法获取返回值了
    // std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...)); 生成智能指针，离开作用域自动析构
    auto task = std::make_shared<std::packaged_task<RetType()>>(std::bind(f, std::forward<Args>(args)...));
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex); // 插入时上锁，防止多个线程同时插入
    taskqueue.emplace([task](){ (*task)(); });
    return task->get_future();
}

class retmsg
{
public:
    void set_msg(const std::string& msg){m_msg = msg;}
    void set_target_fd(int fd){target_fd = fd;}
    std::string& get_msg(){return m_msg;}
    int get_target_fd(){return target_fd;}

private:
    std::string m_msg;
    int target_fd;
};