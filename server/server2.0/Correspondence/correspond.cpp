#include "correspond.h"

void corRedisStream::thread_read()
{
    int consumer_seq = m_consumer_num++;
    std::string consumer_name = REDIS_STREAM_CONSUMERNAME_SERVER + std::to_string(consumer_seq);
    std::shared_ptr<std::vector<std::string>> receive_vec;
    while (m_reader_run.load())
    {
        receive_vec = database::get_instance().redis_stream_xreadgroup(REDIS_STREAM_STREAMNAME_SERVER, REDIS_STREAM_GROUPNAME_SERVER, consumer_name, REDIS_STREAM_XREADGROUP_BLOCK, REDIS_STREAM_XREADGROUP_COUNT);
        if (receive_vec == nullptr || receive_vec->empty())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REDIS_STREAM_XREADGROUP_SLEEPTIME));
        }
        else
        {
            for (auto &msg : *receive_vec)
            {
                // 对读取到的消息进行处理（放到某个容器内？）
            }
        }
    }
}

void corRedisStream::init()
{
    database::get_instance().init_stream_consumer_group(REDIS_STREAM_STREAMNAME_SERVER, REDIS_STREAM_GROUPNAME_SERVER);
    m_reader_run.store(true);
    m_writer_run.store(true);
    std::function<void()> lambda;
    for (int i = 0; i < REDIS_STREAM_READER_NUM; ++i)
    {
        // threadPool::get_instance().commit(&corRedisStream::thread_read, this);
        lambda = [this](){
            this->thread_read();
        };
        threadPool::get_instance().commit(lambda);
    }
    for (int i = 0; i< REDOS_STREAM_WRITER_NUM; ++i)
    {
        lambda = [this](){
            this->thread_write();
        };
        threadPool::get_instance().commit(lambda);
    }
}

void corRedisStream::thread_write()
{
    
}