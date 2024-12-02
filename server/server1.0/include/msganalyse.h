#pragma once

#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
// #include <unordered_map>
#include <queue>
#include <mutex>


using json = nlohmann::json;

enum msgtype
{
    LOGIN,
    REGISTER,
    CHAT,
    ERROR,
    RET,
    RECV
};

class abstractmsg;
class msganalyse
{
public:
    // void init();
    // void set_msg(const std::string& msg){m_msg = msg;}//将消息传入到m_msg
    // void set_sender_fd(int fd){sender_fd = fd;}//将发送者的套接字传入到sender_fd
    //bug 不应该返回
    std::unique_ptr<abstractmsg> generate_msg(std::string& msg,int sender_fd);//根据m_msg和sender_fd生成包装任务所需的基础信息

    // msgtype get_message_type(){return m_type;}
    // void set_msg();
    // abstractmsg* get_final_msg(){return m_msg_ptr;}//返回基础信息
    msgtype get_msg_type(std::string& msg);//获取消息的类型
    //todo server初始化中要调用这个函数
    // void set_get_fd_by_username_func(std::function<int(const std::string&)> func){get_fd_by_username = func;}
private:
    // std::unordered_map<std::string,int> username_to_fd;
    // std::string m_msg;//存放从客户端接收到的json字符串
    // json m_msg_json;//存放从json字符串解析后的json对象
    // abstractmsg* m_msg_ptr;//任务包装模块所需的基础信息
    // abstractmsgfactory* m_msg_factory;//生成基础信息的工厂
    // msgtype m_type;//消息的类型
    // int sender_fd;//发送者的套接字
    // std::mutex queue_mutex;
    // std::queue<std::unique_ptr<abstractmsg>> basicalmsg_queue;
    // std::function<int(const std::string&) > get_fd_by_username;
};

class abstractmsg
{
public:
    msgtype get_msg_type(){return msg_type;}
    void set_sender_fd(int fd){m_send_fd = fd;}
    // void set_receive_fd(int fd){m_receive_fd = fd;}
    int get_sender_fd(){return m_send_fd;}
    // int get_receive_fd(){return m_receive_fd;}
    // void set_receive_fd(int fd){m_receive_fd = fd;}
    virtual void init(const json & msg = NULL) = 0;
protected:
    // std::string msg_type;
    msgtype msg_type;
    int m_send_fd;
    // int m_receive_fd;
};


class loginmsg:public abstractmsg
{
public:
    void set_username(const std::string& username){m_username = username;}
    void set_password(const std::string& password){m_password = password;}
    // void set_sender_fd(int fd){sender_fd = fd;}
    const std::string& get_username(){return m_username;}
    const std::string& get_password(){return m_password;}
    // int get_sender_fd(){return sender_fd;}
    void init(const json& msg = NULL);
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
    void init(const json& msg = NULL);
private:
    std::string m_username;
    std::string m_password;
    std::string m_email;
};

class errormsg:public abstractmsg
{
public:
    void init(const json& msg = NULL);
};

class returnmsg:public abstractmsg
{
public:
    void init(const json& msg = NULL);
};

class receivemsg  : public abstractmsg
{
public:
    void init(const json& msg = NULL);
    // user* m_receiveuser;
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

class returnmsgfactory:public abstractmsgfactory
{
public:
    returnmsg* create_msg()
    {
        return new returnmsg;
    }
};

class recvmsgfactory:public abstractmsgfactory
{
public:
    receivemsg* create_msg()
    {
        return new receivemsg;
    }
};