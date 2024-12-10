#include "socketVec.h"

void socketVector::enqueue_recv_data(std::shared_ptr<std::vector<uint8_t>> data)
{
    recv_data.insert(recv_data.end(), data->begin(), data->end());
}

void socketVector::enqueue_send_data(std::shared_ptr<std::vector<uint8_t>> data)
{
    send_data.insert(send_data.end(), data->begin(), data->end());
}

std::shared_ptr<std::vector<uint8_t>> socketVector::dequeue_recv_data()
{
    auto data = std::move(recv_data);
    return std::make_shared<std::vector<uint8_t>>(data);
}

std::shared_ptr<std::vector<uint8_t>> socketVector::dequeue_send_data()
{
    auto data = std::move(send_data);
    return std::make_shared<std::vector<uint8_t>>(data);
}

// void socketQueues::recv_notify()
// {
//     recv_data_cv.notify_one();
// }

// void socketQueues::send_notify()
// {
//     send_data_cv.notify_one();
// }

std::mutex& socketVector::get_recv_data_mutex()
{
    return recv_data_mutex;
}

std::mutex& socketVector::get_send_data_mutex()
{
    return send_data_mutex;
}

// std::condition_variable& socketVector::get_recv_data_cv()
// {
//     return recv_data_cv;
// }
// std::condition_variable& socketVector::get_send_data_cv()
// {
//     return send_data_cv;
// }

bool socketVector::is_recv_data_empty()
{
    return recv_data.empty();
}

bool socketVector::is_send_data_empty()
{
    return send_data.empty();
}

