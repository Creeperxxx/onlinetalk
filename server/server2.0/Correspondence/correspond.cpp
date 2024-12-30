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
                dataManager::get_instance().pushData(msg);
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
    std::string data;
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> send_vec;
    nlohmann::json send_json;
    std::string receiver_stream;
    while(m_writer_run.load())
    {   
        data = dataManager::get_instance().popData();
        if(data.empty() == true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REDIS_STREAM_XACK_FAILED_SLEEPTIME));
            continue;
        }
        //提供一个反序列化方法
        //将对应的string消息转化为消息结构体
        
        //提供一个方法
        //能将消息结构体提取出要发送的键值对放入std::vector中
        
        //提供一个方法
        //确定该消息要发送的人
        receiver_stream = get_receiver_stream(data);

        //提供一个方法
        //确定要发送的具体信息

        //对data消息进行处理
        database::get_instance().redis_stream_xadd_msg(receiver_stream,data);
    }
}

std::string corRedisStream::get_receiver_name(const std::string& msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_RECEIVER_NAME);
    } catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
    } 
}

std::string corRedisStream::get_receiver_id(const std::string& msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_RECEIVER_ID);
    } catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
    }
}

std::string corRedisStream::get_sender_name(const std::string& msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_SENDER_NAME);
    } catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
    }
}

std::string corRedisStream::get_sender_id(const std::string& msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_SENDER_ID);
    } catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
    }
}

// std::string corRedisStream::get_register_receiver_stream(const std::string& msg)
// {
//     nlohmann::json json_msg = nlohmann::json::parse(msg);
//     try
//     {
//         std::string token = json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_REGISTER_TOKEN);
//         return REDIS_STREAM_REGISTERNAME_PREFIX + ": " + token;
//     } catch(const std::exception& e)
//     {
//         LOG_ERROR("%s:%s:%d // json发生错误：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
//     }
// }

std::string corRedisStream::get_receiver_stream(const std::string& msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_RECEIVER_STREAM);
        // return REDIS_STREAM_RECEIVERNAME_PREFIX + ": " + receiver_id;
    } catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
    }
}