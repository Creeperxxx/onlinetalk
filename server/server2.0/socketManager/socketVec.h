#include <cstdint>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "../logSystem/log.h"
class socketVector
{
private:
    std::vector<uint8_t> recv_data;
    std::mutex recv_data_mutex;
    // std::condition_variable recv_data_cv;

    std::vector<uint8_t> send_data;
    std::mutex send_data_mutex;
    // std::condition_variable send_data_cv;
public:
    //要不要将互斥量和条件变量放这？
    void enqueue_recv_data(std::shared_ptr<std::vector<uint8_t>> data);
    void enqueue_send_data(std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data();
    std::shared_ptr<std::vector<uint8_t>> dequeue_send_data();
    // void recv_notify();
    // void send_notify();
    std::mutex& get_recv_data_mutex();
    std::mutex& get_send_data_mutex();
    // std::condition_variable& get_recv_data_cv();
    // std::condition_variable& get_send_data_cv();
    bool is_recv_data_empty();
    bool is_send_data_empty();
};