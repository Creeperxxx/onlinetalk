#pragma once 
#include <iostream>
#include <string>
#include <unistd.h>

class user
{
public:
    void init(int socketfd,std::string username = "");
    void set_username(const std::string& username){this->username = username;}
    void set_recv_msg(const std::string& msg){recv_msg = msg;}
    void set_send_msg(const std::string& msg){send_msg = msg;}
    void set_isalive(int flag){is_alive = flag;}
    std::string& get_recv_msg(){return recv_msg;}
    std::string& get_username(){return username;}
    std::string& get_send_msg(){return send_msg;}
    int get_socketfd(){return socketfd;}
    void clear();
private:
    // int uid;
    int socketfd;
    std::string recv_msg;
    std::string send_msg; 
    int is_alive;
    std::string username;
};