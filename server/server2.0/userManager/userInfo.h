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

// inline const int TIME_OUT = 60; // second
// constexpr int USERCONNECY_TIMEOUT = 60 * 1000;

enum class TIMEOUT_TYPE
{
    SECOND,
    MILLISECOND
};

constexpr int TIMEOUT_USERCONNECTION_SOCKET_SECOND = 60;
constexpr int TIMEOUT_USERCONNECTION_SOCKET_MILLISECOND = 60 * 1000;
// constexpr bool TIMEOUT_USERCONNECTION_SOCKET_ISSECOND = true;
constexpr TIMEOUT_TYPE TIMEOUT_USERCONNECTION_SOCKET_TYPE = TIMEOUT_TYPE::SECOND;
constexpr int TIMEOUT_USERCONNECTION_REDISSTREAM_SECOND = 60;
constexpr int TIMEOUT_USERCONNECTION_REDISSTREAM_MILLISECOND = 60 * 1000;
// constexpr bool TIMEOUT_USERCONNECTION_REDISSTREAM_ISSECOND = true;
constexpr TIMEOUT_TYPE TIMEOUT_USERCONNECTION_REDISSTREAM_TYPE = TIMEOUT_TYPE::SECOND;

class User
{
private:
    std::string m_userId;
    std::string m_userName;
    int m_socketFd;
    std::string m_redisStream;
    std::atomic<std::chrono::time_point<std::chrono::steady_clock>> m_socket_lastInteractionTime;
    std::atomic<std::chrono::time_point<std::chrono::steady_clock>> m_redisStream_lastInteractionTime;

    // socket
    std::unique_ptr<std::vector<uint8_t>> socket_recvData;
    std::mutex mutex_socket_recvData;
    std::unique_ptr<std::vector<uint8_t>> socket_sendData;
    std::mutex mutex_socket_sendData;

    // redis stream
    std::unique_ptr<std::vector<std::string>> redisStream_recvData;
    std::mutex mutex_redisStream_recvData;
    std::unique_ptr<std::vector<std::string>> redisStream_sendData;
    std::mutex mutex_redisStream_sendData;

    std::atomic<bool> is_socket_active;
    std::atomic<bool> is_redisStream_active;

    std::atomic<bool> m_is_socket_using;
    std::atomic<bool> m_is_redisStream_using;

public:
    void set_userId(const std::string &userid) { m_userId = userid; }
    std::string get_userId() { return m_userId; }
    void set_userName(const std::string &userName) { m_userName = userName; }
    std::string get_userName(const std::string &userName) { return m_userName; }
    void set_socketFd(const int &fd) { m_socketFd = fd; }
    int get_socketFd() { return m_socketFd; }
    void set_redisStream(const std::string &redisStream) { m_redisStream = redisStream; }
    std::string get_redisSteam() { return m_redisStream; }

    // 更新 判断是否过期 返回
    void update_socket_interactionTime() { m_socket_lastInteractionTime.store(std::chrono::steady_clock::now()); }
    void update_redisStream_interactionTime() { m_redisStream_lastInteractionTime.store(std::chrono::steady_clock::now()); }
    bool is_socket_timeOut();
    bool is_redisStream_timeOut();
    std::chrono::time_point<std::chrono::steady_clock> get_socket_lastInteractionTime() { return m_socket_lastInteractionTime.load(); }
    std::chrono::time_point<std::chrono::steady_clock> get_redisStream_lastInteractionTime() { return m_redisStream_lastInteractionTime.load(); }

    // push和pop
    // socket
    void push_socket_recvData(std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_sendData(std::unique_ptr<std::vector<uint8_t>> data);
    std::unique_ptr<std::vector<uint8_t>> pop_socket_recvData();
    std::unique_ptr<std::vector<uint8_t>> pop_socket_sendData();

    // redisStream
    void push_redisStream_recvData(const std::string &msg);
    void push_redisStream_sendData(const std::string &msg);
    std::unique_ptr<std::vector<std::string>> pop_redisStream_recvData();
    std::unique_ptr<std::vector<std::string>> pop_redisStream_sendData();

    bool is_socket_recvData_empty();
    bool is_socket_sendData_empty();
    bool is_redisStream_recvData_empty();
    bool is_redisStream_sendData_empty();

    bool is_socket_using();
    bool is_redisStream_using();

    void set_socket_using(bool is_using) { m_is_socket_using.store(is_using); }
    void set_redisStream_using(bool is_using) { m_is_redisStream_using.store(is_using); }

    User();
};

struct compareUserSocketInteractionTimeASC
{
    bool operator()(const std::shared_ptr<User> &lhs, const std::shared_ptr<User> &rhs)
    {
        return lhs->get_socket_lastInteractionTime() < rhs->get_socket_lastInteractionTime();
    }
};

struct compareUserRedisStreamInteractionTimeASC
{
    bool operator()(const std::shared_ptr<User> &lhs, const std::shared_ptr<User> &rhs)
    {
        return lhs->get_redisStream_lastInteractionTime() < rhs->get_redisStream_lastInteractionTime();
    }
};

struct compareUserSocketIsRecvDataEmpty
{
    bool operator()(const std::shared_ptr<User> &luser, const std::shared_ptr<User> &ruser)
    {
        if (luser->is_socket_recvData_empty() == false && ruser->is_socket_recvData_empty() == true)
        {
            return true;
        }
        return false;
    }
};

struct compareUserSocketIsSendDataEmpty
{
    bool operator()(const std::shared_ptr<User> &luser, const std::shared_ptr<User> &ruser)
    {
        if (luser->is_socket_sendData_empty() == false && ruser->is_socket_sendData_empty() == true)
        {
            return true;
        }
        return false;
    }
};

// class socketInfo
// {
// private:
//     int fd;
//     std::string username;
//     std::string userid;

//     // std::atomic<time_t> last_interaction_time{std::chrono::steady_clock::now()};
//     std::atomic<std::chrono::time_point<std::chrono::steady_clock>> last_interaction_time;

//     // std::priority_queue<socketVector*, std::vector<socketVector*>, compareSocketVec> pq;

//     // std::shared_ptr<std::vector<uint8_t>> recv_data;
//     // std::mutex recv_data_mutex;
//     // std::condition_variable recv_data_cv;
//     // std::condition_variable recv_data_cv;

//     // std::shared_ptr<std::vector<uint8_t>> send_data;
//     // std::mutex send_data_mutex;
//     // std::condition_variable send_data_cv;
//     // std::condition_variable send_data_cv;
// public:
//     // socketVector(int socketfd, std::string name):fd(socketfd), username(name) , last_interaction_time(time(NULL)),recv_data(new std::vector<uint8_t>),send_data(new std::vector<uint8_t>){}
//     // socketVector(int socketfd):fd(socketfd) , last_interaction_time(std::chrono::steady_clock::now()),recv_data(new std::vector<uint8_t>),send_data(new std::vector<uint8_t>){}
//     socketInfo(int socketfd) : fd(socketfd), last_interaction_time(std::chrono::steady_clock::now()) {}
//     // 要不要将互斥量和条件变量放这？
//     //  time_t get_last_interaction_time() const;
//     //  void enqueue_recv_data(std::shared_ptr<std::vector<uint8_t>> data);
//     //  void enqueue_send_data(std::shared_ptr<std::vector<uint8_t>> data);
//     //  std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data();
//     //  std::shared_ptr<std::vector<uint8_t>> dequeue_send_data();
//     //  void recv_notify();
//     //  void send_notify();
//     //  std::mutex& get_recv_data_mutex();
//     //  std::mutex& get_send_data_mutex();
//     //  std::condition_variable& get_recv_data_cv();
//     //  std::condition_variable& get_send_data_cv();
//     //  bool is_recv_data_empty();
//     //  bool is_send_data_empty();

//     void update_last_interaction_time();
//     bool is_timeout();
//     int get_fd() { return fd; }
//     std::string get_userid() { return userid; }
//     const std::string &get_username() { return username; }
//     // bool operator<(const socketInfo &other) const
//     // {
//     //     return this->last_interaction_time.load() < other.last_interaction_time.load();
//     // }
//     // void set_fd(int fd){this->fd = fd;}
// };

// // struct compareSocketVec
// // {
// //     bool operator()(const socketVector* a, const socketVector* b) const
// //     {
// //         return a->get_last_interaction_time() > b->get_last_interaction_time();
// //     }
// // };

// // struct compareSocketVec
// // {
// //     bool operator()(const std::shared_ptr<socketVector>& a, const std::shared_ptr<socketVector>& b) const
// //     {
// //         return a->get_last_interaction_time() < b->get_last_interaction_time();
// //     }
// // };

// class socketInfoDataVec : public socketInfo
// {
// public:
//     void push_recv_data(std::shared_ptr<std::vector<uint8_t>> data);
//     void push_send_data(std::shared_ptr<std::vector<uint8_t>> data);
//     std::shared_ptr<std::vector<uint8_t>> pop_recv_data();
//     std::shared_ptr<std::vector<uint8_t>> pop_send_data();

// private:
//     std::shared_ptr<std::vector<uint8_t>> recv_data;
//     std::mutex mutex_recv_data;
//     std::shared_ptr<std::vector<uint8_t>> send_data;
//     std::mutex mutex_send_data;
// };

// class IuserInfo
// {
// public:
//     // void update_interaction_time();
//     // bool is_timeout();
//     // std::chrono::time_point<std::chrono::steady_clock> get_last_interaction_time(){return last_interaction_time;}
//     void set_user_id(const std::string &user_id) { m_user_id = user_id; }
//     void set_user_name(const std::string &user_name) { m_user_name = user_name; }
//     std::string get_user_id() { return m_user_id; }
//     std::string get_user_name() { return m_user_name; }
//     // void set_is_online(bool flag){is_online = flag;}
//     // bool get_is_online(){return is_online;}
// protected:
//     std::string m_user_id;
//     std::string m_user_name;
//     // bool is_online;
//     // std::atomic<std::chrono::time_point<std::chrono::steady_clock>> last_interaction_time;
// };

// struct CompareUserInteractionTime
// {
//     bool operator()(const std::shared_ptr<IuserInfo> &a, const std::shared_ptr<IuserInfo> &b) const
//     {
//         // return a->get_last_interaction_time() < b->get_last_interaction_time();
//     }
// };

// class socketUserInfo : public IuserInfo
// {
// private:
//     int m_fd;
//     std::atomic<std::chrono::time_point<std::chrono::steady_clock>> socket_last_interaction_time;

//     // std::string userid;
//     // std::string username;

//     std::unique_ptr<std::vector<uint8_t>> recv_data;
//     std::mutex mutex_recv_data;

//     std::unique_ptr<std::vector<uint8_t>> send_data;
//     std::mutex mutex_send_data;

// public:
//     // 重载小于号
//     //  socketUserInfo& operator<(const socketUserInfo& s);
//     socketUserInfo(int socketfd) : m_fd(socketfd) {};
//     // socketData(int socketfd, std::unique_ptr<std::vector<uint8_t>> send, std::unique_ptr<std::vector<uint8_t>> recv)
//     //     : fd(socketfd), send_data(std::move(send)), recv_data(std::move(recv)) {}
//     socketUserInfo(int socketfd,
//                    std::optional<std::unique_ptr<std::vector<uint8_t>>> send = std::nullopt,
//                    std::optional<std::unique_ptr<std::vector<uint8_t>>> recv = std::nullopt)
//         : m_fd(socketfd), send_data(send.has_value() ? std::move(send.value()) : nullptr), recv_data(recv.has_value() ? std::move(recv.value()) : nullptr) {}

//     std::unique_ptr<std::vector<uint8_t>> pop_recv_data();
//     std::unique_ptr<std::vector<uint8_t>> pop_send_data();
//     // void update_interaction_time();

//     void push_recv_data(std::unique_ptr<std::vector<uint8_t>> data);
//     void push_send_data(std::unique_ptr<std::vector<uint8_t>> data);
//     bool is_recv_data_empty();
//     bool is_send_data_empty();
//     int get_fd() { return m_fd; }
//     void set_fd(int fd) { m_fd = fd; }
// };

// class redisStreamUserInfo : public socketUserInfo
// {
// private:
//     std::string user_stream;
//     std::unique_ptr<std::vector<std::string>> recv_strs;
//     std::mutex mutex_recv;
//     std::unique_ptr<std::vector<std::string>> send_strs;
//     std::mutex mutex_send;

// public:
//     void push_recv_str(const std::string &str);
//     void push_send_str(const std::string &str);
//     std::string pop_recv_str();
//     std::string pop_send_str();
//     bool is_recv_empty();
//     bool is_send_empty();
// };
