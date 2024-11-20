#include "taskpackage.h"


taskpackage::task taskpackage::takeTask() 
{
    task task;
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex);  // 上锁
    if (!taskqueue.empty()) {
        task = std::move(taskqueue.front());    // 取出任务
        taskqueue.pop();  // 将任务从队列中删除
        return task;
    }
    return nullptr;
}

auto taskpackage::generate_task()
{
    msgtype type = m_msg->get_msg_type();
    if(type == LOGIN)
    {
        return addTask(taskpackage::login_handle,m_msg,connection_pool::GetInsance());
    }
    else if(type == REGISTER)
    {
        return addTask(taskpackage::register_handle,m_msg,connection_pool::GetInsance());
    }
    else if(type == CHAT)
    {
        return addTask(taskpackage::chat_handle,m_msg,connection_pool::GetInsance());
    }
    else if(type == ERROR)
    {
        return addTask(taskpackage::error_handle,m_msg,connection_pool::GetInsance());
    }
    else if(type == RET)
    {
        return addTask(taskpackage::ret_handle,m_msg,connection_pool::GetInsance());
    }
}

std::unique_ptr<retmsg> taskpackage::login_handle(abstractmsg* msg,connection_pool* pool)
{
    MYSQL** conn;
    connectionRAII connRAII(conn,pool);
    
}

std::unique_ptr<retmsg> taskpackage::register_handle(abstractmsg* msg,connection_pool* pool)
{

}

std::unique_ptr<retmsg> taskpackage::chat_handle(abstractmsg* msg,connection_pool* pool)
{

}

std::unique_ptr<retmsg> taskpackage::error_handle(abstractmsg* msg,connection_pool* pool)
{

}

std::unique_ptr<retmsg> taskpackage::ret_handle(abstractmsg* msg,connection_pool* pool)
{

}


