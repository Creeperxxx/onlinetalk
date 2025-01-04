// #include "socketVec.h"

// #include "socketInfo.h"
#include "socketData.h"
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

void socketInfo::update_last_interaction_time()
{
    last_interaction_time.store(std::chrono::steady_clock::now());
}

bool socketInfo::is_timeout()
{
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - last_interaction_time.load());
    return elapsed.count() > TIME_OUT;
}

std::unique_ptr<std::vector<uint8_t>> socketData::pop_recv_data()
{
    return std::move(recv_data);
}

std::unique_ptr<std::vector<uint8_t>> socketData::pop_send_data()
{
    return std::move(send_data);
}

void socketData::push_recv_data(std::unique_ptr<std::vector<uint8_t>> data)
{
    std::lock_guard<std::mutex> lock(mutex_recv_data);
    recv_data->insert(recv_data->end(), data->begin(), data->end());
}

void socketData::push_send_data(std::unique_ptr<std::vector<uint8_t>> data)
{
    std::lock_guard<std::mutex> lock(mutex_send_data);
    send_data->insert(send_data->end(), data->begin(), data->end());
}

void socketData::update_interaction_time()
{
    last_interaction_time.store(std::chrono::steady_clock::now());
}

bool socketData::is_recv_data_empty()
{
    std::lock_guard<std::mutex> lock(mutex_recv_data);
    return recv_data->empty();
}

bool socketData::is_send_data_empty()
{
    std::lock_guard<std::mutex> lock(mutex_send_data);
    return send_data->empty();
}