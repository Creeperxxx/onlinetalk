#include "threadPool.h"

threadPool::threadPool(int threadnum) : taskQueue(std::make_unique<TaskQueue>()), shutdown(false), busyThreadNum(0) { 
    this->threadNum.store(threadnum);
    for (int i = 0; i < this->threadNum; ++i) {
        threadVec.push_back(std::make_shared<std::thread>(&threadPool::worker, this)); // 创建线程
        threadVec.back()->detach();    // 创建线程后detach，与主线程脱离
    }
}

// ThreadPool::~ThreadPool() {
//     shutdown = true;  // 等待线程执行完，就不在去队列取任务
// }

void threadPool::worker() {
    while (!shutdown) {
        std::unique_lock<std::mutex> uniqueLock(threadPoolMutex);
        notEmptyCondVar.wait(uniqueLock, [this] { return !taskQueue->empty() || shutdown; });  // 任务队列为空，阻塞在此，当任务队列不是空或者线程池关闭时，向下执行
        auto currTask = std::move(taskQueue->takeTask());  // 取出任务
        uniqueLock.unlock();
        ++busyThreadNum;
        currTask();  // 执行任务
        --busyThreadNum;
    }
}

void threadPool::deleter()
{
    shutdown = true;
    notEmptyCondVar.notify_all();
}

threadPool& threadPool::get_instance()
{
    static threadPool instance;
    return instance;
}