#pragma once
#include <iostream>

class user
{
    void login();
    void registeruser();
    void forgetpasswd();
private:
    int m_uid;
    std::string m_name;
    std::string m_passwd;
    bool login_result;
};