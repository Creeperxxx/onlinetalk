#include "../dataBase/dataBase.h"
#include "../threadPool/threadPool.h"
#include "../dataManager/dataManager.h"
// #include "../networkio/networkio.h"
#include "../socketNetworkIo/socketNetworkIo.h"
#include <memory>
#include "../eventHandler/eventHandler.h"

//corredisstream有两个todo

// inline const bool CORSOCKET_POP_READYSOCKET_ISSINGLE = false;
// // inline const bool CORSOCKET_POP_SENDDATA_ISSINGLE = true;
// enum class corSocketWriteType
// {
//     QUEUE,
//     SET,
//     LOOP
// };
// inline const corSocketWriteType CORSOCKET_WRITE_TYPE = corSocketWriteType::SET;
// inline const bool CORSOCKET_WRITE_SET_ISSINGLE = true;
// inline const bool CORSOCKET_WRITE_LOOP_ISSINGLE = true;

class Icorrespondence
{
public:
    // virtual void init();
    // virtual void run();
    // virtual void thread_write();
    // virtual void thread_read();
    // virtual void run() = 0;
    virtual ~Icorrespondence() = default;

private:
    // 读取消息功能
    // 发布消息功能
};

class corRedisStream : public Icorrespondence
{
public:
    void init(const std::string &server_stream = REDIS_STREAM_STREAMNAME_SERVER, const std::string &consumer_group = REDIS_STREAM_CONSUMERNAME_SERVER, int reader_num = REDIS_STREAM_READER_NUM, int writer_num = REDIS_STREAM_WRITER_NUM); // override;
    // void run()override;
    ~corRedisStream() override;
    void run();// override;
    void thread_write() ;//override;
    void thread_read(); //override;
private:
    void deleter();
    std::string get_receiver_name(const std::string &msg);
    std::string get_receiver_id(const std::string &msg);
    std::string get_sender_name(const std::string &msg);
    std::string get_sender_id(const std::string &msg);
    // std::string get_register_receiver_stream(const std::string& msg); //感觉这个应该再注册消息处理中获取到接收方的redisstream
    std::string get_receiver_stream(const std::string &msg);

private:
    std::atomic<int> m_consumer_num;
    int m_reader_num;
    int m_writer_num;
    std::atomic<bool> m_reader_run;
    std::atomic<bool> m_writer_run;
    // std::atomic<bool> m_reader_run;
    // std::atomic<bool> m_writer_run;
    // int m_reader_num = 1;
    // int m_writer_num = 1;
};

class corSocket : public Icorrespondence
{
public:
    void run();
    void init(int listen_port); // override;
    // void run()override;
    void thread_write() ;//override;
    void thread_read() ;//override;
    void thread_event_loop();
    ~corSocket() override;
private:
    void deleter();
    // void recv_data_single_queue();
    // void recv_data_multiple_queue();

    // void send_data_single_queue();

    // void send_data_single_set();
    // void send_data_multiple_set();

    // void send_data_single_loop();
    // void send_data_multiple_loop();
    

    // void recv_data();
    
    void send_data();
    void recv_data();
private:
    // std::atomic<bool> m_writer_run;
    std::atomic<bool> m_writer_run;
    std::atomic<bool> m_reader_run;
    std::unique_ptr<socketNetworkIo> m_networkio;
    std::unique_ptr<IEventHandler> m_event_handler;
    // std::unique_ptr<readySocketManager> m_ready_socket_manager;
};
