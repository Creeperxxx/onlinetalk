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
                dataManager::get_instance().push_data(msg);
            }
        }
    }
}

void corRedisStream::init(const std::string &server_stream = REDIS_STREAM_STREAMNAME_SERVER, const std::string &consumer_group = REDIS_STREAM_CONSUMERNAME_SERVER, int reader_num = REDIS_STREAM_READER_NUM, int writer_num = REDIS_STREAM_WRITER_NUM)
{
    database::get_instance().init_stream_consumer_group(server_stream, consumer_group);
    m_reader_run.store(true);
    m_writer_run.store(true);
    m_reader_num = reader_num;
    m_writer_num = writer_num;
}

void corRedisStream::thread_write()
{
    std::string data;
    std::shared_ptr<std::vector<std::pair<std::string, std::string>>> send_vec;
    nlohmann::json send_json;
    std::string receiver_stream;
    while (m_writer_run.load())
    {
        data = dataManager::get_instance().pop_data();
        if (data.empty() == true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REDIS_STREAM_XACK_FAILED_SLEEPTIME));
            continue;
        }
        // 提供一个反序列化方法
        // 将对应的string消息转化为消息结构体

        // 提供一个方法
        // 能将消息结构体提取出要发送的键值对放入std::vector中

        // 提供一个方法
        // 确定该消息要发送的人
        receiver_stream = get_receiver_stream(data);

        // 提供一个方法
        // 确定要发送的具体信息

        // 对data消息进行处理
        database::get_instance().redis_stream_xadd_msg(receiver_stream, data);
    }
}

std::string corRedisStream::get_receiver_name(const std::string &msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_RECEIVER_NAME);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
    }
}

std::string corRedisStream::get_receiver_id(const std::string &msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_RECEIVER_ID);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
    }
}

std::string corRedisStream::get_sender_name(const std::string &msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_SENDER_NAME);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
    }
}

std::string corRedisStream::get_sender_id(const std::string &msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_SENDER_ID);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
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

std::string corRedisStream::get_receiver_stream(const std::string &msg)
{
    nlohmann::json json_msg = nlohmann::json::parse(msg);
    try
    {
        return json_msg.at(MESSAGE_HEADER).at(MESSAGE_HEADER_RECEIVER_STREAM);
        // return REDIS_STREAM_RECEIVERNAME_PREFIX + ": " + receiver_id;
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json发生错误：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
    }
}

void corSocket::init(int listen_port)
{
    m_networkio = std::make_unique<socketNetworkIo>();
    m_networkio->init(listen_port);
    m_event_handler = std::make_unique<ReactorEventHandler>();
    dynamic_cast<ReactorEventHandler *>(m_event_handler.get())->init(m_networkio->get_listenfd());
    m_reader_run.store(true);
    m_writer_run.store(true);
}

void corRedisStream::run()
{
    std::function<void()> lambda;
    for (int i = 0; i < m_reader_num; ++i)
    {
        // threadPool::get_instance().commit(&corRedisStream::thread_read, this);
        lambda = [this]()
        {
            this->thread_read();
        };
        threadPool::get_instance().commit(lambda);
    }
    for (int i = 0; i < m_writer_num; ++i)
    {
        lambda = [this]()
        {
            this->thread_write();
        };
        threadPool::get_instance().commit(lambda);
    }
}

corRedisStream::~corRedisStream()
{
    deleter();
}

void corRedisStream::deleter()
{
    m_reader_run.store(false);
    m_writer_run.store(false);
}

corSocket::~corSocket()
{
    deleter();
}

void corSocket::deleter()
{
}

void corSocket::thread_event_loop()
{
    // dynamic_cast<ReactorEventHandler*>(m_event_handler.get()).run();
    dynamic_cast<ReactorEventHandler *>(m_event_handler.get())->run();
}

void corSocket::thread_read()
{
    std::shared_ptr<std::vector<int>> ready_sockets;
    std::shared_ptr<std::vector<uint8_t>> recv_data;
    int socket;
    if (CORSOCKET_POP_READYSOCKET_ISSINGLE == true)
    {
        while (m_reader_run.load())
        {
            recv_data_single_queue();
        }
    }
    else
    {
        while (m_reader_run.load())
        {
            recv_data_multiple_queue();
        }
    }
}

void corSocket::thread_write()
{
    // std::shared_ptr<std::vector<uint8_t>> send_data;
    switch (CORSOCKET_WRITE_TYPE)
    {
    case corSocketWriteType::QUEUE:
        while (m_writer_run.load())
        {
            send_data_single_queue();
        }
        break;
    case corSocketWriteType::SET:
        if (CORSOCKET_WRITE_SET_ISSINGLE == true)
        {
            while (m_writer_run.load())
            {
                send_data_single_set();
            }
        }
        else
        {
            while (m_writer_run.load())
            {
                send_data_multiple_set();
            }
        }
        break;
    case corSocketWriteType::LOOP:
        if (CORSOCKET_WRITE_LOOP_ISSINGLE == true)
        {
            while (m_writer_run.load())
            {
                send_data_single_loop();
            }
        }
        else
        {
            while (m_writer_run.load())
            {
                send_data_multiple_loop();
            }
        }
        break;
    }
}

void corSocket::send_data_single_queue()
{
    auto send_data = dataManager::get_instance().pop_send_data_from_socket_queue();
    if (send_data->is_send_data_empty() == false)
    {
        m_networkio->send_data(send_data->get_fd(), reinterpret_cast<const unsigned char*>(send_data->pop_send_data()->data()), send_data->pop_send_data()->size());
    }
}

void corSocket::recv_data_single_queue()
{
    auto ready_socket = dataManager::get_instance().pop_ready_socket_single();
    if (ready_socket != -1)
    {
        auto recv_data = m_networkio->recv_data(ready_socket);
        if (recv_data->size() > 0)
        {
            dataManager::get_instance().push_recv_data_to_socket_queue(ready_socket, std::move(recv_data));
        }
    }
}

void corSocket::recv_data_multiple_queue()
{
    auto ready_sockets = dataManager::get_instance().pop_ready_socket_vec();
    std::unique_ptr<std::vector<uint8_t>> recv_data;
    for (auto socket : *ready_sockets)
    {
        recv_data = m_networkio->recv_data(socket);
        if (recv_data->size() > 0)
        {
            dataManager::get_instance().push_recv_data_to_socket_queue(socket, std::move(recv_data));
        }
    }
}

void corSocket::run()
{
    auto lambda = [this]()
    {
        this->thread_event_loop();
    };
    threadPool::get_instance().commit(lambda);
    auto lambda1 = [this]()
    {
        this->thread_read();
    };
    threadPool::get_instance().commit(lambda);
    auto lambda2 = [this]()
    {
        this->thread_write();
    };
    threadPool::get_instance().commit(lambda);
}

void corSocket::send_data_single_set()
{
    auto send_data = dataManager::get_instance().pop_send_data_from_socket_single_set();
    if (send_data->is_send_data_empty() == false)
    {
        m_networkio->send_data(send_data->get_fd(), reinterpret_cast<const unsigned char*>(send_data->pop_send_data()->data()), send_data->pop_send_data()->size());
    }
}

void corSocket::send_data_multiple_set()
{
    auto send_datas = dataManager::get_instance().pop_send_data_from_socket_multiple_set();
    for (auto send_data : *send_datas)
    {
        if (send_data->is_send_data_empty() == false)
        {
            m_networkio->send_data(send_data->get_fd(), reinterpret_cast<const unsigned char*>(send_data->pop_send_data()->data()), send_data->pop_send_data()->size());
        }
    }
}

void corSocket::send_data_single_loop()
{
    auto send_data = dataManager::get_instance().pop_send_data_from_socket_single_loop();
    if (send_data->is_send_data_empty() == false)
    {
        m_networkio->send_data(send_data->get_fd(), reinterpret_cast<const unsigned char*>(send_data->pop_send_data()->data()), send_data->pop_send_data()->size());
    }
}

void corSocket::send_data_multiple_loop()
{
    auto send_datas = dataManager::get_instance().pop_send_data_from_socket_multiple_loop();
    for (auto send_data : *send_datas)
    {
        if (send_data->is_send_data_empty() == false)
        {
            m_networkio->send_data(send_data->get_fd(), reinterpret_cast<const unsigned char*>(send_data->pop_send_data()->data()), send_data->pop_send_data()->size());
        }
    }
}
