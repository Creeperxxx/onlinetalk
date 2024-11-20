/** Created by Jiale on 2022/3/14 10:42.
 *  Decryption:线程池源文件
**/

#include "threadpool.h"

ThreadPool::ThreadPool(int threadnum) : taskqueue(std::make_unique<std::queue<task>>()),shutdown(false), busyThreadNum(0)
{ 
    // taskqueue = std::make_unique<queue<task>>();
    this->threadNum.store(threadnum);
    for (int i = 0; i < this->threadNum; ++i) {
        threadVec.push_back(std::make_shared<std::thread>(&ThreadPool::worker, this)); // 创建线程
        threadVec.back()->detach();    // 创建线程后detach，与主线程脱离
    }
}

ThreadPool::~ThreadPool() {
    shutdown = true;  // 等待线程执行完，就不在去队列取任务
}

void ThreadPool::worker() {
    while (!shutdown) \
    {
        std::unique_lock<std::mutex> uniqueLock(queuemutex);
        notempty.wait(uniqueLock, [this] { return!taskqueue->empty() || shutdown; });
        if(taskqueue->empty())
        {
            continue;
        }
        // haveone.wait(uniqueLock);
        // haveone.wait(uniqueLock, [this] { return havetask || shutdown; });  // 任务队列为空，阻塞在此，当任务队列不是空或者线程池关闭时，向下执行
        //任务队列 关机
        //空且关机 break
        //空且不关机 wait   
        //不空且关机 执行
        //不空且不关机 执行
        // if (shutdown &&!havetask) 
        // {
        //     break;
        // }
        // auto currTask = std::move();  // 取出任务
        auto currtask = std::move(taketask());
        uniqueLock.unlock();
        ++busyThreadNum;
        currtask();  // 执行任务
        --busyThreadNum;
    }
}

void ThreadPool::addTask(task&& task) 
{
    std::unique_lock<std::mutex> uniqueLock(queuemutex);
    taskqueue->push(std::move(task));
    uniqueLock.unlock();
    notempty.notify_one();
    return;
}

// ThreadPool::task ThreadPool::getTask() {
    
//     return m_task;
// }

ThreadPool::task ThreadPool::taketask() {
    task task;
    std::lock_guard<std::mutex> lockGuard(queuemutex);  // 上锁
    if (!taskqueue->empty())
    {
        task = std::move(taskqueue->front());    // 取出任务
        taskqueue->pop();  // 将任务从队列中删除
        return task;
    }
    return nullptr;
}