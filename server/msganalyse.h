#pragma once

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>


using json = nlohmann::json;

enum msgtype
{
    LOGIN,
    REGISTER,
    CHAT,
    ERROR,
    RET
};

class msganalyse
{
public:
    void set_msg(const std::string& msg){m_msg = msg;}
    void generate_msg();
    // msgtype get_message_type(){return m_type;}
    // void set_msg();
    abstractmsg* get_final_msg(){return m_msg_ptr;}
private:
    msgtype get_msg_type();
private:
    std::string m_msg;
    abstractmsg* m_msg_ptr;
    msgtype m_type;
    json m_msg_json;
};

class abstractmsg
{
public:
    msgtype get_msg_type(){return msg_type;}
    virtual void init(const json & msg) = 0;
protected:
    // std::string msg_type;
    msgtype msg_type;
};


class loginmsg:public abstractmsg
{
public:
    void set_username(const std::string& username){m_username = username;}
    void set_password(const std::string& password){m_password = password;}
    const std::string& get_username(){return m_username;}
    const std::string& get_password(){return m_password;}
    void init(const json& msg);
private:
    std::string m_username;
    std::string m_password;
};

class chatmsg:public abstractmsg
{
public:
    void set_msg(const std::string& msg){m_msg = msg;}
    void set_reciver(const std::string& reciver){m_reciver = reciver;}
    const std::string& get_msg(){return m_msg;}
    const std::string& get_reciver(){return m_reciver;}
    void init(const json& msg);
private:
    std::string m_msg;
    std::string m_reciver;
};

class registermsg:public abstractmsg
{
public:
    void set_username(const std::string& username){m_username = username;}
    void set_password(const std::string& password){m_password = password;}
    void set_email(const std::string& email){m_email = email;}
    const std::string& get_username(){return m_username;}
    const std::string& get_password(){return m_password;}
    const std::string& get_email(){return m_email;}
    void init(const json& msg);
private:
    std::string m_username;
    std::string m_password;
    std::string m_email;
};

class errormsg:public abstractmsg
{
public:
    void init(const json& msg);
};

class abstractmsgfactory
{
public:
    virtual abstractmsg* create_msg() = 0;
};

class loginmsgfactory:public abstractmsgfactory
{
public:
    loginmsg* create_msg()
    {
        return new loginmsg;
    }
};

class chatmsgfactory:public abstractmsgfactory
{
public:
    chatmsg* create_msg()
    {
        return new chatmsg;
    }
};

class registermsgfactory:public abstractmsgfactory
{
public:
    registermsg* create_msg()
    {
        return new registermsg;
    }
};

class errormsgfactory:public abstractmsgfactory
{
public:
    errormsg* create_msg()
    {
        return new errormsg;
    }
};