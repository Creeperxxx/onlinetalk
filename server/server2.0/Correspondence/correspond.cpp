#include "correspond.h"

void corRedisStream::thread_read()
{
    int consumer_seq = m_consumer_num++;
    std::string consumer_name = REDIS_STREAM_CONSUMERNAME_SERVER + std::to_string(consumer_seq);
    auto receive_vec = database::get_instance().redis_stream_xreadgroup(REDIS_STREAM_STREAMNAME_SERVER, REDIS_STREAM_GROUPNAME_SERVER, consumer_name, REDIS_STREAM_XREADGROUP_BLOCK, REDIS_STREAM_XREADGROUP_COUNT);
    if(receive_vec == nullptr || receive_vec->empty())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(REDIS_STREAM_XREADGROUP_SLEEPTIME));
    }
    else
    {
        for(auto& msg : *receive_vec)
        {
            
        }
    }
}

void corRedisStream::init()
{
    database::get_instance().init_stream_consumer_group(REDIS_STREAM_STREAMNAME_SERVER, REDIS_STREAM_GROUPNAME_SERVER);
}