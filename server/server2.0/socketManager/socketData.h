#include <cstdint>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include "../logSystem/log.h"
// #include <time.h>
#include <atomic>
#include <chrono>
#include <optional>
// #include <string>
// #include <unordered_map>

inline const int TIME_OUT = 60; // second

class socketInfo
{
private:
    int fd;
    std::string username;
    std::string userid;

    // std::atomic<time_t> last_interaction_time{std::chrono::steady_clock::now()};
    std::atomic<std::chrono::time_point<std::chrono::steady_clock>> last_interaction_time;

    // std::priority_queue<socketVector*, std::vector<socketVector*>, compareSocketVec> pq;

    // std::shared_ptr<std::vector<uint8_t>> recv_data;
    // std::mutex recv_data_mutex;
    // std::condition_variable recv_data_cv;
    // std::condition_variable recv_data_cv;

    // std::shared_ptr<std::vector<uint8_t>> send_data;
    // std::mutex send_data_mutex;
    // std::condition_variable send_data_cv;
    // std::condition_variable send_data_cv;
public:
    // socketVector(int socketfd, std::string name):fd(socketfd), username(name) , last_interaction_time(time(NULL)),recv_data(new std::vector<uint8_t>),send_data(new std::vector<uint8_t>){}
    // socketVector(int socketfd):fd(socketfd) , last_interaction_time(std::chrono::steady_clock::now()),recv_data(new std::vector<uint8_t>),send_data(new std::vector<uint8_t>){}
    socketInfo(int socketfd) : fd(socketfd), last_interaction_time(std::chrono::steady_clock::now()) {}
    // 要不要将互斥量和条件变量放这？
    //  time_t get_last_interaction_time() const;
    //  void enqueue_recv_data(std::shared_ptr<std::vector<uint8_t>> data);
    //  void enqueue_send_data(std::shared_ptr<std::vector<uint8_t>> data);
    //  std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data();
    //  std::shared_ptr<std::vector<uint8_t>> dequeue_send_data();
    //  void recv_notify();
    //  void send_notify();
    //  std::mutex& get_recv_data_mutex();
    //  std::mutex& get_send_data_mutex();
    //  std::condition_variable& get_recv_data_cv();
    //  std::condition_variable& get_send_data_cv();
    //  bool is_recv_data_empty();
    //  bool is_send_data_empty();

    void update_last_interaction_time();
    bool is_timeout();
    int get_fd() { return fd; }
    std::string get_userid() { return userid; }
    const std::string &get_username() { return username; }
    bool operator<(const socketInfo &other) const
    {
        return this->last_interaction_time.load() < other.last_interaction_time.load();
    }
    // void set_fd(int fd){this->fd = fd;}
};

// struct compareSocketVec
// {
//     bool operator()(const socketVector* a, const socketVector* b) const
//     {
//         return a->get_last_interaction_time() > b->get_last_interaction_time();
//     }
// };

// struct compareSocketVec
// {
//     bool operator()(const std::shared_ptr<socketVector>& a, const std::shared_ptr<socketVector>& b) const
//     {
//         return a->get_last_interaction_time() < b->get_last_interaction_time();
//     }
// };

class socketInfoDataVec : public socketInfo
{
public:
    void push_recv_data(std::shared_ptr<std::vector<uint8_t>> data);
    void push_send_data(std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<std::vector<uint8_t>> pop_recv_data();
    std::shared_ptr<std::vector<uint8_t>> pop_send_data();

private:
    std::shared_ptr<std::vector<uint8_t>> recv_data;
    std::mutex mutex_recv_data;
    std::shared_ptr<std::vector<uint8_t>> send_data;
    std::mutex mutex_send_data;
};

class socketData
{
private:
    int fd;
    std::string userid;
    std::string username;
    std::atomic<std::chrono::time_point<std::chrono::steady_clock>> last_interaction_time;

    std::unique_ptr<std::vector<uint8_t>> recv_data;
    std::mutex mutex_recv_data;
    
    std::unique_ptr<std::vector<uint8_t>> send_data;
    std::mutex mutex_send_data;

public:
    socketData(int socketfd) : fd(socketfd) {};
    // socketData(int socketfd, std::unique_ptr<std::vector<uint8_t>> send, std::unique_ptr<std::vector<uint8_t>> recv)
    //     : fd(socketfd), send_data(std::move(send)), recv_data(std::move(recv)) {}
    socketData(int socketfd,
               std::optional<std::unique_ptr<std::vector<uint8_t>>> send = std::nullopt,
               std::optional<std::unique_ptr<std::vector<uint8_t>>> recv = std::nullopt)
        : fd(socketfd), send_data(send.has_value() ? std::move(send.value()) : nullptr), recv_data(recv.has_value() ? std::move(recv.value()) : nullptr) {}
    std::unique_ptr<std::vector<uint8_t>> pop_recv_data();
    std::unique_ptr<std::vector<uint8_t>> pop_send_data();
    void update_interaction_time();
    
    void push_recv_data(std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data(std::unique_ptr<std::vector<uint8_t>> data);
    bool is_recv_data_empty();
    bool is_send_data_empty();
    int get_fd() { return fd; }
    
};