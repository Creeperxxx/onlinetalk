#include "../dataBase/dataBase.h"
#include "../threadPool/threadPool.h"
#include "../dataManager/dataManager.h"
#include "../networkio/networkio.h"
#include <memory>


class Icorrespondence
{
public:
    virtual void init();
    // virtual void run();
    virtual void thread_write();
    virtual void thread_read();
private:
    //读取消息功能
    //发布消息功能
protected:
    std::atomic<bool> m_reader_run;
    std::atomic<bool> m_writer_run;

};

class corRedisStream : public Icorrespondence
{
public: 
    void init()override;
    void thread_write() override;
    void thread_read() override;
    // void run()override;
private:
    std::string get_receiver_name(const std::string& msg);
    std::string get_receiver_id(const std::string& msg);
    std::string get_sender_name(const std::string& msg);
    std::string get_sender_id(const std::string& msg);
    // std::string get_register_receiver_stream(const std::string& msg); //感觉这个应该再注册消息处理中获取到接收方的redisstream
    std::string get_receiver_stream(const std::string& msg);
private:
    std::atomic<int> m_consumer_num;
    // std::atomic<bool> m_reader_run;
    // std::atomic<bool> m_writer_run;
    // int m_reader_num = 1;
    // int m_writer_num = 1;
};

class corSocket : public Icorrespondence
{
public:
    void init()override;
    // void run()override;
    void thread_write() override;
    void thread_read() override;
    void thread_event_loop();
private:
    std::unique_ptr<NetworkIo> m_networkio;
};



