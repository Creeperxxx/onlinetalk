#include "../dataBase/dataBase.h"

class Icorrespondence
{
public:
    //读取消息功能
    std::string read_msg();
    //发布消息功能
    bool write_msg(const std::string& msg);
    
};

class corRedisStream : Icorrespondence
{

};

class corSocket : Icorrespondence
{

};