#include "../dataBase/dataBase.h"
#include "../threadPool/threadPool.h"


class Icorrespondence
{
public:
    virtual void init();
    virtual void run();
private:
    //读取消息功能
    std::string read_msg();
    //发布消息功能
    bool write_msg(const std::string& msg);
    
};

class corRedisStream : Icorrespondence
{
public: 
    void init()override;
    void thread_write();
    void thread_read();
    void run()override;
private:
    std::string read_msg();
    bool write_msg(const std::string& msg);
private:
    std::atomic<int> m_consumer_num;
    std::atomic<bool> m_reader_run;
    std::atomic<bool> m_writer_run;
    // int m_reader_num = 1;
    // int m_writer_num = 1;
};

class corSocket : Icorrespondence
{

};