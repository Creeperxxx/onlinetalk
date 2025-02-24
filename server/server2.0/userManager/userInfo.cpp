// #include "socketVec.h"

// #include "socketInfo.h"
// #include "socketData.h"
#include "userInfo.h"
// const int TIME_OUT = 10;

// void socketVector::enqueue_recv_data(std::shared_ptr<std::vector<uint8_t>> data)
// {
//     std::lock_guard<std::mutex> lock(recv_data_mutex);
//     // recv_data.insert(recv_data.end(), data->begin(), data->end());

//     recv_data->insert(recv_data->end(), data->begin(), data->end());
//     // recv_data_cv.notify_one();
// }

// void socketVector::enqueue_send_data(std::shared_ptr<std::vector<uint8_t>> data)
// {
//     std::lock_guard<std::mutex> lock(send_data_mutex);
//     send_data->insert(send_data->end(), data->begin(), data->end());
//     // send_data_cv.notify_one();
// }

// std::shared_ptr<std::vector<uint8_t>> socketVector::dequeue_recv_data()
// {
//     std::unique_lock<std::mutex> lock(recv_data_mutex);
//     // recv_data_cv.wait(lock,[this](){return !recv_data->empty();});
//     // return std::make_shared<std::vector<uint8_t>>(std::move(*recv_data));
//     auto ret = std::make_shared<std::vector<uint8_t>>(recv_data->begin(),recv_data->end());
//     recv_data->clear();
//     return ret;
// }

// std::shared_ptr<std::vector<uint8_t>> socketVector::dequeue_send_data()
// {
//     // std::lock_guard<std::mutex> lock(send_data_mutex);
//     // return std::make_shared<std::vector<uint8_t>>(std::move(*send_data));
//     std::unique_lock<std::mutex> lock(send_data_mutex);
//     // send_data_cv.wait(lock,[this]{return !send_data->empty();});
//     auto ret = std::make_shared<std::vector<uint8_t>>(send_data->begin(),send_data->end());
//     send_data->clear();
//     return ret;
// }

// void socketQueues::recv_notify()
// {
//     recv_data_cv.notify_one();
// }

// void socketQueues::send_notify()
// {
//     send_data_cv.notify_one();
// }

/* std::mutex& socketVector::get_recv_data_mutex()
{
    return recv_data_mutex;
} */
/*
std::mutex& socketVector::get_send_data_mutex()
{
    return send_data_mutex;
}
 */
// std::condition_variable& socketVector::get_recv_data_cv()
// {
//     return recv_data_cv;
// }
// std::condition_variable& socketVector::get_send_data_cv()
// {
//     return send_data_cv;
// }

// bool socketVector::is_recv_data_empty()
// {
//     std::lock_guard<std::mutex> lock(recv_data_mutex);
//     return recv_data->empty();
// }

// bool socketVector::is_send_data_empty()
// {
//     std::lock_guard<std::mutex> lock(send_data_mutex);
//     return send_data->empty();
// }

// time_t socketVector::get_last_interaction_time() const
// {
//     return last_interaction_time.load();
// }

// void socketInfo::update_last_interaction_time()
// {
//     last_interaction_time.store(std::chrono::steady_clock::now());
// }

// bool socketInfo::is_timeout()
// {
//     auto now = std::chrono::steady_clock::now();
//     auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_interaction_time.load());
//     return elapsed.count() > TIME_OUT;
// }

// std::unique_ptr<std::vector<uint8_t>> socketUserInfo::pop_recv_data()
// {
//     return std::move(recv_data);
// }

// std::unique_ptr<std::vector<uint8_t>> socketUserInfo::pop_send_data()
// {
//     return std::move(send_data);
// }

// void socketUserInfo::push_recv_data(std::unique_ptr<std::vector<uint8_t>> data)
// {
//     std::lock_guard<std::mutex> lock(mutex_recv_data);
//     recv_data->insert(recv_data->end(), data->begin(), data->end());
// }

// void socketUserInfo::push_send_data(std::unique_ptr<std::vector<uint8_t>> data)
// {
//     std::lock_guard<std::mutex> lock(mutex_send_data);
//     send_data->insert(send_data->end(), data->begin(), data->end());
// }

// // void socketUserInfo::update_interaction_time()
// void IuserInfo::update_interaction_time()
// {
//     last_interaction_time.store(std::chrono::steady_clock::now());
// }

// bool socketUserInfo::is_recv_data_empty()
// {
//     std::lock_guard<std::mutex> lock(mutex_recv_data);
//     return recv_data->empty();
// }

// bool socketUserInfo::is_send_data_empty()
// {
//     std::lock_guard<std::mutex> lock(mutex_send_data);
//     return send_data->empty();
// }

// bool IuserInfo::is_timeout()
// {
//     auto now = std::chrono::steady_clock::now();
//     auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_interaction_time.load());
//     return elapsed.count() > TIME_OUT;
// }

bool User::is_socket_timeOut()
{
    auto now = std::chrono::steady_clock::now();
    switch (TIMEOUT_USERCONNECTION_SOCKET_TYPE)
    {
    default:
    case TIMEOUT_TYPE::SECOND:
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_socket_lastInteractionTime.load());
        return elapsed.count() > TIMEOUT_USERCONNECTION_SOCKET_SECOND;
    }
    case TIMEOUT_TYPE::MILLISECOND:
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_socket_lastInteractionTime.load());
        return elapsed.count() > TIMEOUT_USERCONNECTION_SOCKET_MILLISECOND;
    }
    }
}

bool User::is_redisStream_timeOut()
{
    auto now = std::chrono::steady_clock::now();
    switch (TIMEOUT_USERCONNECTION_REDISSTREAM_TYPE)
    {
    default:
    case TIMEOUT_TYPE::SECOND:
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - m_redisStream_lastInteractionTime.load());
        return elapsed.count() > TIMEOUT_USERCONNECTION_REDISSTREAM_SECOND;
    }
    case TIMEOUT_TYPE::MILLISECOND:
    {
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_redisStream_lastInteractionTime.load());
        return elapsed.count() > TIMEOUT_USERCONNECTION_REDISSTREAM_MILLISECOND;
    }
    }
}

bool User::is_socket_recvData_empty()
{
    std::lock_guard<std::mutex> lock(mutex_socket_recvData);
    return socket_recvData->empty();
}

bool User::is_socket_sendData_empty()
{
    std::lock_guard<std::mutex> lock(mutex_socket_sendData);
    return socket_sendData->empty();
}

bool User::is_redisStream_recvData_empty()
{
    std::lock_guard<std::mutex> lock(mutex_redisStream_recvData);
    return redisStream_recvData->empty();
}

bool User::is_redisStream_sendData_empty()
{
    std::lock_guard<std::mutex> lock(mutex_redisStream_sendData);
    return redisStream_sendData->empty();
}

void User::push_socket_recvData(std::unique_ptr<std::vector<uint8_t>> data)
{
    std::lock_guard<std::mutex> lock(mutex_socket_recvData);
    socket_recvData->insert(socket_recvData->end(), data->begin(), data->end());
}
void User::push_socket_sendData(std::unique_ptr<std::vector<uint8_t>> data)
{
    std::lock_guard<std::mutex> lock(mutex_socket_sendData);
    socket_sendData->insert(socket_sendData->end(), data->begin(), data->end());
}
std::unique_ptr<std::vector<uint8_t>> User::pop_socket_recvData()
{
    std::lock_guard<std::mutex> lock(mutex_socket_recvData);
    auto ret = std::move(socket_recvData);
    socket_recvData = std::make_unique<std::vector<uint8_t>>();
    return std::move(ret);
}
std::unique_ptr<std::vector<uint8_t>> User::pop_socket_sendData()
{
    std::lock_guard<std::mutex> lock(mutex_socket_sendData);
    auto ret = std::move(socket_sendData);
    socket_sendData = std::make_unique<std::vector<uint8_t>>();
    return std::move(ret);
}

// redisStream
void User::push_redisStream_recvData(const std::string &msg)
{
    std::lock_guard<std::mutex> lock(mutex_redisStream_recvData);
    redisStream_recvData->push_back(msg);
}
void User::push_redisStream_sendData(const std::string &msg)
{
    std::lock_guard<std::mutex> lock(mutex_redisStream_sendData);
    redisStream_sendData->push_back(msg);
}
std::unique_ptr<std::vector<std::string>> User::pop_redisStream_recvData()
{
    std::lock_guard<std::mutex> lock(mutex_redisStream_recvData);
    auto ret = std::move(redisStream_recvData);
    redisStream_recvData = std::make_unique<std::vector<std::string>>();
    return std::move(ret);
}
std::unique_ptr<std::vector<std::string>> User::pop_redisStream_sendData()
{
    std::lock_guard<std::mutex> lock(mutex_redisStream_sendData);
    auto ret = std::move(redisStream_sendData);
    redisStream_sendData = std::make_unique<std::vector<std::string>>();
    return std::move(ret);
}

bool User::is_socket_using()
{
    return m_is_socket_using.load();
}

bool User::is_redisStream_using()
{
    return m_is_redisStream_using.load();
}

User::User()
{
    m_socket_lastInteractionTime = std::chrono::steady_clock::now();
    m_redisStream_lastInteractionTime = std::chrono::steady_clock::now();
    socket_recvData = std::make_unique<std::vector<uint8_t>>();
    socket_sendData = std::make_unique<std::vector<uint8_t>>();
    redisStream_recvData = std::make_unique<std::vector<std::string>>();
    redisStream_sendData = std::make_unique<std::vector<std::string>>();
    
}