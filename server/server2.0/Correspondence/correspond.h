#include "../dataBase/dataBase.h"

inline const int REDIS_READER_NUM = 3;
inline const int REDIS_WRITER_NUM = 3;

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
    void run()override;
private:
    void thread_read();
    void thread_write();
    std::string read_msg();
    bool write_msg(const std::string& msg);
private:
    std::atomic<int> m_consumer_num;
    // int m_reader_num = 1;
    // int m_writer_num = 1;
};

class corSocket : Icorrespondence
{

};