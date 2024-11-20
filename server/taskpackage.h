#include <functional>
#include <queue>
#include <future>
#include <mutex>
#include <memory>
#include "msganalyse.h"
#include "sql_connection_pool.h"


class retmsg;
class taskpackage
{
public:
    using task = std::function<void()>;
    template <typename F, typename... Args>
    auto addTask(F &f, Args &&...args) -> std::future<decltype(f(args...))>; 
    task takeTask();                                                         // 取任务
    bool empty() { return taskqueue.empty(); }
    void set_msg(abstractmsg* msg){m_msg = msg;}
    auto generate_task();
private:
    static std::unique_ptr<retmsg> login_handle(abstractmsg* msg,connection_pool* pool);
    static std::unique_ptr<retmsg> register_handle(abstractmsg* msg,connection_pool* pool);
    static std::unique_ptr<retmsg> chat_handle(abstractmsg* msg,connection_pool* pool);
    static std::unique_ptr<retmsg> error_handle(abstractmsg* msg,connection_pool* pool);
    static std::unique_ptr<retmsg> ret_handle(abstractmsg* msg,connection_pool* pool);
private:
    std::queue<task> taskqueue; 
    std::mutex taskQueueMutex; // 互斥锁，防止多个线程同时访问任务队列
    abstractmsg* m_msg;
    connection_pool* pool;
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
    // std::lock_guard<std::mutex> lockGuard(taskQueueMutex); // 插入时上锁，防止多个线程同时插入
    taskqueue.emplace([task](){ (*task)(); });
    return task->get_future();
}

class retmsg
{
public:
    void set_msg(const std::string& msg){m_msg = msg;}
    void set_target_fd(int fd){target_fd = fd;}
    const std::string& get_msg(){return m_msg;}
    int get_target_fd(){return target_fd;}
private:
    std::string m_msg;
    int target_fd;
};