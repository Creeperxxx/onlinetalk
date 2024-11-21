#include "msganalyse.h"

void msganalyse::generate_msg()
{
    msgtype type = get_msg_type();
    delete m_msg_factory;
    delete m_msg_ptr;
    switch (type)
    {
        case LOGIN:
            m_msg_factory = new loginmsgfactory();
            m_msg_ptr = m_msg_factory->create_msg();
            break;
        case CHAT:
            m_msg_factory = new chatmsgfactory();
            m_msg_ptr = m_msg_factory->create_msg();
            break;
        case REGISTER:
            m_msg_factory = new registermsgfactory();
            m_msg_ptr = m_msg_factory->create_msg();
            break;
        case ERROR:
            m_msg_factory = new errormsgfactory();
            m_msg_ptr = m_msg_factory->create_msg();
            break;
        default:
            break;
    }
    m_msg_ptr->init(m_msg_json);
    m_msg_ptr->set_sender_fd(sender_fd);
}

msgtype msganalyse::get_msg_type()
{
    m_msg_json = json::parse(m_msg);
    std::string type = m_msg_json["type"];
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
    else if (type == "error")
    {
        return ERROR;
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
    // m_username = msg["username"];
    // m_password = msg["password"];
}

void chatmsg::init(const json& msg)
{
    msg_type = LOGIN;
    m_msg = msg["data"]["context"];
    m_reciver = msg["data"]["reciver"];
}

void registermsg::init(const json& msg)
{
    msg_type = REGISTER;
    m_username = msg["data"]["username"];
    m_password = msg["data"]["password"];
    m_email = msg["data"]["email"];
}

void errormsg::init(const json& msg)
{
    msg_type = ERROR;
}

void returnmsg::init(const json& msg)
{
    msg_type = RET;
}