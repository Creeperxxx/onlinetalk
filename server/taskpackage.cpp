#include "taskpackage.h"

const std::string USERNAME = "username";
const std::string PASSWORD = "password";
const std::string EMAIL = "email";

taskpackage::task taskpackage::takeTask()
{
    task task;
    std::lock_guard<std::mutex> lockGuard(taskQueueMutex); // 上锁
    if (!taskqueue.empty())
    {
        task = std::move(taskqueue.front()); // 取出任务
        taskqueue.pop();                     // 将任务从队列中删除
        return task;
    }
    return nullptr;
}

auto taskpackage::generate_task(std::unique_ptr<abstractmsg> basicmsg)->std::future<std::unique_ptr<retmsg>>
{
    msgtype type = basicmsg->get_msg_type();
    using lambdarettype = std::unique_ptr<retmsg>;
    // using addtaskrettype = std::future<std::unique_ptr<retmsg>>;
    std::function<lambdarettype()> lambda;
    // addtaskrettype task;
    switch(type)
    {
    case LOGIN:
        lambda = [this,ptr = std::move(basicmsg)]()mutable{return this->login_handle(std::move(ptr));};
        break;
    case REGISTER:
        lambda = [this,ptr = std::move(basicmsg)]()mutable{return this->register_handle(std::move(ptr));};
        break;
    case CHAT:
        lambda = [this,ptr = std::move(basicmsg)]()mutable{return this->chat_handle(std::move(ptr));};
        break;
    case ERROR:
        lambda = [this,ptr = std::move(basicmsg)]()mutable{return this->error_handle(std::move(ptr));};
        break;
    case RET:
        lambda = [this,ptr = std::move(basicmsg)]()mutable{return this->ret_handle(std::move(ptr));};
        break;
    case RECV:
        lambda = [this,ptr = std::move(basicmsg)]()mutable{return this->recv_handle(std::move(ptr));};
        break;
    default:
        std::cerr << "taskpackage::generate_task() :error" << std::endl;
        exit(1);
        break;
    }
    return addTask(lambda);
    // if (type == LOGIN)
    // {
    //     return addTask(taskpackage::login_handle, m_msg, MySQLConnectionPool::getInstance());
    // }
    // else if (type == REGISTER)
    // {
    //     return addTask(taskpackage::register_handle, m_msg, MySQLConnectionPool::getInstance());
    // }
    // else if (type == CHAT)
    // {
    //     return addTask(taskpackage::chat_handle, m_msg, MySQLConnectionPool::getInstance());
    // }
    // else if (type == ERROR)
    // {
    //     return addTask(taskpackage::error_handle, m_msg, MySQLConnectionPool::getInstance());
    // }
    // else if (type == RET)
    // {
    //     return addTask(taskpackage::ret_handle);
    // }
}

// std::unique_ptr<retmsg> taskpackage::login_handle(abstractmsg *msg, MySQLConnectionPool *pool)
std::unique_ptr<retmsg> taskpackage::login_handle(std::unique_ptr<abstractmsg> msg)
{

    loginmsg *login_msg = dynamic_cast<loginmsg *>(msg.get());
    MySQLConnectionPool *pool = MySQLConnectionPool::getInstance();
    // 从连接池中获取连接
    auto conn = pool->getConnection();
    sql::Statement *stmt = conn->createStatement();
    // 执行查询
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM users");

    bool login_flag = false;
    while (res->next())
    {
        if (res->getString(USERNAME) == login_msg->get_username() && res->getString(PASSWORD) == login_msg->get_password())
        {
            // 登录成功
            login_flag = true;
            break;
        }
    }
    std::unique_ptr retmessage = std::make_unique<retmsg>();
    retmessage->set_target_fd(login_msg->get_sender_fd());
    std::string temp_msg;
    if (login_flag)
    {
        temp_msg = "login success";//以json格式返回
        add_username_to_fd(login_msg->get_username(), login_msg->get_sender_fd());
    }
    else
    {
        // 登录失败
        temp_msg = "login failed";//以json格式返回
    }
    retmessage->set_msg(temp_msg);
    delete stmt;
    return retmessage;
}

// std::unique_ptr<retmsg> taskpackage::register_handle(abstractmsg *msg, MySQLConnectionPool *pool)
std::unique_ptr<retmsg> taskpackage::register_handle(std::unique_ptr<abstractmsg> msg)
{
    registermsg *register_msg = dynamic_cast<registermsg *>(msg.get());
    MySQLConnectionPool *pool = MySQLConnectionPool::getInstance();
    // 从连接池中获取连接
    auto conn = pool->getConnection();
    sql::Statement *stmt = conn->createStatement();
    // 执行查询
    sql::ResultSet *res = stmt->executeQuery("SELECT * FROM users");
    // 邮箱不存在且用户名不存在则可注册，否则失败
    bool register_flag = true;
    while (res->next())
    {
        if (res->getString(EMAIL) == register_msg->get_email() || res->getString(USERNAME) == register_msg->get_username())
        {
            register_flag = false;
            break;
        }
    }
    std::string temp_msg ;
    if(register_flag)
    {
        // 注册成功
        temp_msg = "register success";//以json格式返回
        std::string insert_sql = "INSERT INTO users (username, password, email) VALUES ('" + register_msg->get_username() + "', '" + register_msg->get_password() + "', '" + register_msg->get_email() + "')";
        stmt->execute(insert_sql);
    }
    else
    {
        // 注册失败
        temp_msg = "register failed";//以json格式返回
    }
    delete stmt;
    std::unique_ptr retmessage = std::make_unique<retmsg>();
    retmessage->set_target_fd(register_msg->get_sender_fd());
    retmessage->set_msg(temp_msg);
    return retmessage;
}

// std::unique_ptr<retmsg> taskpackage::chat_handle(abstractmsg *msg, MySQLConnectionPool *pool)
std::unique_ptr<retmsg> taskpackage::chat_handle(std::unique_ptr<abstractmsg> msg)
{
    chatmsg *chat_msg = dynamic_cast<chatmsg *>(msg.get());
    
    chat_msg->set_sender_fd(m_msg->get_sender_fd());
    std::unique_ptr retmessage = std::make_unique<retmsg>();
    int receiver_fd = 0;
    std::string recviver_name = chat_msg->get_reciver();
    //根据recviver找到receiver_fd
    if (username_to_fd.find(recviver_name)!= username_to_fd.end())
    {
        receiver_fd = username_to_fd[recviver_name];
        retmessage->set_target_fd(receiver_fd);
        retmessage->set_msg(chat_msg->get_msg());
    }
    else
    {
        // 发送失败
        retmessage->set_msg("send failed");//需要以json格式返回
        retmessage->set_target_fd(chat_msg->get_sender_fd());
    }
    return retmessage;
}

// std::unique_ptr<retmsg> taskpackage::error_handle(abstractmsg *msg, MySQLConnectionPool *pool)
std::unique_ptr<retmsg> taskpackage::error_handle(std::unique_ptr<abstractmsg> msg)
{
    errormsg *error_msg = dynamic_cast<errormsg *>(msg.get());
    std::unique_ptr retmessage = std::make_unique<retmsg>();
    retmessage->set_msg("error");//需要以json格式返回
    retmessage->set_target_fd(error_msg->get_sender_fd());
    return retmessage;
}

std::unique_ptr<retmsg> taskpackage::ret_handle(std::unique_ptr<abstractmsg> msg)
{
    // std::unique_ptr retmessage = std::make_unique<retmsg>();
    std::unique_ptr retmsg = result.get();
    //调用网络io模块对retmsg进行发送
    //专门写一个static函数来发送服务器返回消息，记住使用代理类来控制权限，只允许只用这个静态函数
    // user* temp_user = new user;
    // temp_user->init(retmsg->get_target_fd(),0);
    // temp_user->set_send_msg(retmsg->get_msg());
    // connection->send_msg(temp_user);
    // delete temp_user;
    return NULL;
}

std::unique_ptr<retmsg> taskpackage::recv_handle(std::unique_ptr<abstractmsg> msg)
{
    // connection->recv_msg()
}

void taskpackage::add_username_to_fd(const std::string username, int fd)
{
    username_to_fd.insert({username, fd});
    latest_username = username;
    latest_socketfd = fd;
    latest_username_flag = true;
}

void taskpackage::get_latest_username_fd(std::string &username, int &fd)
{
    username = latest_username;
    fd = latest_socketfd;
}

