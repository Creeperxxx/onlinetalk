/** Created by Jiale on 2022/3/14 10:42.
 *  Decryption: 线程池头文件
**/
#pragma once

#include <atomic>
#include <thread>
#include <condition_variable>
#include "taskpackage.h"
#include <memory>


class ThreadPool 
{
    using task = std::function<void()>;
    std::atomic<int> threadNum{};  // 最小线程数
    std::atomic<int> busyThreadNum; // 忙线程数
    std::condition_variable notempty;
    // bool havetask;
    std::mutex queuemutex;   // 线程池互斥锁
    bool shutdown;          // 线程池是否启动
    // std::unique_ptr<taskpackage> m_taskpackage;  // 任务队列
    std::unique_ptr<std::queue<task>> taskqueue;
    task m_task;
    std::vector<std::shared_ptr<std::thread>> threadVec;  // 线程池
    // task getTask();
    task taketask();
public:
    explicit ThreadPool(int threadnum = 5);   // 创建线程池
    ~ThreadPool();          // 销毁线程池
    void addTask(task&& task);
    // template <typename F, typename ...Args>
    // auto commit(F &f, Args &&...args) -> decltype(m_taskpackage->addTask(f, std::forward<Args>(args)...)); // 提交一个任务
    void worker();
};

// template <typename F, typename ...Args>  // 可变参数模板
// auto ThreadPool::commit(F &f, Args &&...args) -> decltype(m_taskpackage->addTask(f, std::forward<Args>(args)...)){
//     // 这个目的就是把接收的参数直接转发给我们上面写的addTask方法，这样，就可以对使用者隐藏TaskQueue的细节，只向用户暴露ThreadPool就行
//     auto ret = m_taskpackage->addTask(f, std::forward<Args>(args)...);
//     notEmptyCondVar.notify_one();
//     return ret;
// }


