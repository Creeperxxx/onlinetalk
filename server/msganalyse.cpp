#include "msganalyse.h"

void msganalyse::generate_msg()
{
    msgtype type = get_msg_type();
    abstractmsg* msg = NULL;
    abstractmsgfactory* fac = NULL;
    switch (type)
    {
        case LOGIN:
            fac = new loginmsgfactory();
            msg = fac->create_msg();
            break;
        case CHAT:
            fac = new chatmsgfactory();
            msg = fac->create_msg();
            break;
        case REGISTER:
            fac = new registermsgfactory();
            msg = fac->create_msg();
            break;
        case ERROR:
            fac = new errormsgfactory();
            msg = fac->create_msg();
            break;
        default:
            break;
    }
    msg->init(m_msg_json);
    m_msg_ptr = msg;
    msg = NULL;
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
    else
    {
        return ERROR;
    }
}

void loginmsg::init(const json& msg)
{
    msg_type = "login";
    m_username = msg["data"]["username"];
    m_password = msg["data"]["password"];
    // m_username = msg["username"];
    // m_password = msg["password"];
}

void chatmsg::init(const json& msg)
{
    msg_type = "chat";
    m_msg = msg["data"]["context"];
    m_reciver = msg["data"]["reciver"];
}

void registermsg::init(const json& msg)
{
    msg_type = "register";
    m_username = msg["data"]["username"];
    m_password = msg["data"]["password"];
    m_email = msg["data"]["email"];
}

void errormsg::init(const json& msg)
{
    msg_type = "error";
}
