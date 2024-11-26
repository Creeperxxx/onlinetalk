#include "msganalyse.h"

std::unique_ptr<abstractmsg> msganalyse::generate_msg(std::string& msg,int sender_fd)
{
    msgtype type = get_msg_type(msg);
    abstractmsgfactory* msg_factory = NULL;
    std::unique_ptr<abstractmsg> basicalmsg_ptr = NULL;
    switch (type)
    {
        case LOGIN:
            msg_factory = new loginmsgfactory();
            basicalmsg_ptr = std::make_unique<loginmsg>(*(dynamic_cast<loginmsg*>(msg_factory->create_msg())));
            break;
        case CHAT:
            msg_factory = new chatmsgfactory();
            basicalmsg_ptr = std::make_unique<chatmsg>(*(dynamic_cast<chatmsg*>(msg_factory->create_msg())));
            break;
        case REGISTER:
            msg_factory = new registermsgfactory();
            basicalmsg_ptr = std::make_unique<registermsg>(*(dynamic_cast<registermsg*>(msg_factory->create_msg())));
            break;
        case ERROR:
            msg_factory = new errormsgfactory();
            basicalmsg_ptr = std::make_unique<errormsg>(*(dynamic_cast<errormsg*>(msg_factory->create_msg())));
            break;
        case RET:
            msg_factory = new returnmsgfactory();
            basicalmsg_ptr = std::make_unique<returnmsg>(*(dynamic_cast<returnmsg*>(msg_factory->create_msg())));
            break;
        case RECV:
            msg_factory = new recvmsgfactory();
            basicalmsg_ptr = std::make_unique<receivemsg>(*(dynamic_cast<receivemsg*>(msg_factory->create_msg())));
            break;
        default:
            break;
    }
    basicalmsg_ptr->set_sender_fd(sender_fd);
    basicalmsg_ptr->init(json::parse(msg));
    if(type == CHAT)
    {
        chatmsg* chat_msg = dynamic_cast<chatmsg*>(basicalmsg_ptr.get());
        int receiver_fd = get_fd_by_username(chat_msg->get_reciver());
        chat_msg->set_receive_fd(receiver_fd);
    }
    return std::move(basicalmsg_ptr);
    // std::lock_guard<std::mutex> lock(queue_mutex);
    // basicalmsg_queue.push(std::move(basicalmsg_ptr));
}

msgtype msganalyse::get_msg_type(std::string& msg)
{
    json msg_json = json::parse(msg);
    std::string type = msg_json["type"];
    if(type == "login")
    {
        return LOGIN;
    }
    else if (type == "chat")
    {
        return CHAT;
    }
    else if (type == "register")
    {
        return REGISTER;
    }
    else if (type == "ret")
    {
        return RET;
    }
    else if(type == "recv")
    {
        return RECV;
    }
    else
    {
        return ERROR;
    }
}

void loginmsg::init(const json& msg)
{
    msg_type = LOGIN;
    m_username = msg["data"]["username"];
    m_password = msg["data"]["password"];
    m_receive_fd = m_send_fd;
    // m_username = msg["username"];
    // m_password = msg["password"];
}

void chatmsg::init(const json& msg)
{
    msg_type = CHAT;
    m_msg = msg["data"]["context"];
    m_reciver = msg["data"]["reciver"];
    // m_receive_fd = get_fd_by_username(m_reciver);
}

void registermsg::init(const json& msg)
{
    msg_type = REGISTER;
    m_username = msg["data"]["username"];
    m_password = msg["data"]["password"];
    m_email = msg["data"]["email"];
    m_receive_fd = m_send_fd;
}

void errormsg::init(const json& msg)
{
    msg_type = ERROR;
    m_receive_fd = m_send_fd;
}

void returnmsg::init(const json& msg)
{
    msg_type = RET;
    // m_receive_fd = m_send_fd;
}

void receivemsg::init(const json& msg)
{
    msg_type = RECV;
}

// void msganalyse::init()
// {

// }