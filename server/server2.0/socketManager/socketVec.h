#include <cstdint>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "../logSystem/log.h"
#include <time.h>
#include <atomic>

extern const int TIME_OUT;

class socketVector
{
private:
    int fd;
    std::string username;
    std::string userid;

    std::atomic<time_t> last_interaction_time;
    // std::priority_queue<socketVector*, std::vector<socketVector*>, compareSocketVec> pq;
    
    
    std::shared_ptr<std::vector<uint8_t>> recv_data;
    std::mutex recv_data_mutex;
    // std::condition_variable recv_data_cv;

    std::shared_ptr<std::vector<uint8_t>> send_data;
    std::mutex send_data_mutex;
    // std::condition_variable send_data_cv;
public:
    socketVector(int socketfd, std::string name):fd(socketfd), username(name) , last_interaction_time(time(NULL)),recv_data(new std::vector<uint8_t>),send_data(new std::vector<uint8_t>){}
    //要不要将互斥量和条件变量放这？
    time_t get_last_interaction_time() const;
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
    // bool operator<(const socketVector& other) const
    // {
    //     return this->last_interaction_time.load() < other.last_interaction_time.load();
    // }
    void update_last_interaction_time();
    bool is_timeout();
    int get_fd(){return fd;}
    std::string get_userid(){return userid;}
    const std::string& get_username(){return username;}
    // void set_fd(int fd){this->fd = fd;}
};

// struct compareSocketVec
// {
//     bool operator()(const socketVector* a, const socketVector* b) const
//     {
//         return a->get_last_interaction_time() > b->get_last_interaction_time();
//     }
// };
struct compareSocketVec
{
    bool operator()(const std::shared_ptr<socketVector>& a, const std::shared_ptr<socketVector>& b) const
    {
        return a->get_last_interaction_time() < b->get_last_interaction_time();
    }
};