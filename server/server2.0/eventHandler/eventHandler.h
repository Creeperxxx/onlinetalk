#pragma once
// #include "../networkio/networkio.h"
#include "../socketNetworkIo/socketNetworkIo.h"
#include <sys/epoll.h>
#include <signal.h>
#include <atomic>
#include <functional>
#include <queue>
// #include "../requirement/moodycamel/concurrentqueue.h"
#include "../serializationMethod/serializationMethod.h"
#include "../threadPool/threadPool.h"
#include <unordered_map>
#include "../msgAnalysis/msgAnalysisFSM.h"
#include "../logSystem/log.h"
#include <string>
// #include "../socketManager/socketManager.h"
#include "../userManager/userManager.h"
#include "ehConstants.h"
#include "../dataManager/dataManager.h"
#include <fcntl.h>

inline const bool REACTOR_IS_PUSH_READY_SOCKET_SINGLE = true;
inline const bool REACTOR_IS_EPOLL_WAIT_TIMEOUT = false;
inline const int REACTOR_EPOLL_WAIT_TIMEOUT_SECOND = 10;
inline const int REACTOR_EPOLL_MAX_EVENTS = 10;

// extern const int MAX_EPOLL_EVENTS;
// extern const int THREAD_NUMS;
// extern const int LISTEN_PORT;
// extern const int MAX_DEQUEUE_NUMS;


// extern std::atomic<bool> event_loop_running;
// void event_loop_running_signal_handler(int signal);

class IEventHandler
{
public:
    // virtual void init() = 0;
    // virtual void handle_new_connections() = 0;
    // virtual void run() = 0;
    // virtual ~IEventHandler() = default;
    // virtual void handle_ready_connections(int socketfd) = 0;
    virtual ~IEventHandler() = default;
};

class ReactorEventHandler : public IEventHandler
{
public:
    void init(int listen_fd);
    void run();
    ~ReactorEventHandler() override;
private:
    void init_epoll();
    void event_loop();
    void accept_new_connections();
    void add_socket_to_epoll(int socketfd, uint32_t events);
    void set_socket_isblocking(int socketfd,bool isblocking);
    void deleter();
private:
    std::atomic<bool> m_event_loop_running;
    std::atomic<bool> m_event_loop_isend;
    int epoll_fd;
    int listen_fd;
};







// class ReactorEventHandlerV1 : public IEventHandler
// {
// public:
//     ~ReactorEventHandlerV1();
//     void init();
//     void new_init();
//     void run();
//     void old_1_event_loop();
//     void new_event_loop();
//     void handle_sockets_recv();
//     void handle_sockets_send();//todo 同时检查是否有要发给为上线用户的消息。
//     void analyze_recv_data();
//     void heartbeat();
//     // void enqueue_send_message(std::shared_ptr<message> data);
//     // void enqueue_send_message(std::string username , std::shared_ptr<std::vector<uint8_t>> data);
//     void enqueue_send_message(const std::string& userid,std::shared_ptr<std::vector<uint8_t>> data);
//     void task_commit(std::function<void()> task);
//     void add_new_userid_to_all_userid(const std::string& userid);
//     void delete_userid_from_all_userid(const std::string& userid);
    
    

// private:
//     void deleter();
//     void init_epoll();
//     void add_socketfd_to_epoll(int socketfd, uint32_t events);
//     // void accept_new_connection();
//     void handle_new_connections();
//     // void handle_ready_connections(int socketfd) override;
//     // template <typename T>
//     // std::shared_ptr<std::vector<T>> data_from_concurrentQueue(moodycamel::ConcurrentQueue<T> &queue);
//     // uint32_t calculateCRC32(const uint8_t *data, size_t length);
//     // int get_socket_from_username(const std::string& username);
//     int get_socket_from_userid(const std::string& userid);
//     // int get_socket_from_userid(int userid);
//     std::shared_ptr<std::vector<uint8_t>> get_heartbeat_data(int socketfd);
//     void load_all_userid();
//     bool is_userid_exist(const std::string& userid);
//     // bool is_user_online(const std::string& userid);

// private:
//     int epoll_fd;
//     std::atomic<int> handle_sockets_recv_running;
//     std::atomic<int> handle_sockets_send_running;
//     std::atomic<int> analyze_recv_data_running;
//     std::atomic<int> heartbeat_running;
//     std::unique_ptr<socketNetworkIo> networkio;
//     std::unique_ptr<threadPool> thread_pool;
//     // std::shared_ptr<msgAnalysisFSM> msg_analysis_fsm;
//     std::unique_ptr<msgAnalysisFSM> msg_analysis_fsm;
//     // std::shared_ptr<msgAnalysis> msgAnalysis;
    
//     // std::unique_ptr<IserializationMethod> serializationMethod;

//     // moodycamel::ConcurrentQueue<int> ready_sockets;
//     // std::queue<int> ready_sockets;
//     std::vector<int> ready_sockets;
//     std::mutex ready_sockets_mutex;
//     std::condition_variable ready_sockets_cv;

//     std::unordered_set<std::string> all_userid;
//     std::mutex all_userid_mutex;

//     std::atomic<bool> m_event_loop_running;

//     // std::shared_ptr<socketManager> socket_manager;
//     // std::unique_ptr<socketManager> socket_manager;
    
//     // 这里有两种方案设计，一种是无锁队列存放二进制，另一种是无锁队列存放vector，每个vector存放一个消息的二进制
//     //  std::unordered_map<int,moodycamel::ConcurrentQueue<std::vector<uint8_t>>> sockets_recv_data;
//     //  std::unordered_map<int,moodycamel::ConcurrentQueue<std::vector<uint8_t>>> sockets_send_data;
//     // std::unordered_map<int, moodycamel::ConcurrentQueue<uint8_t>> sockets_recv_data;
//     // std::unordered_map<int, moodycamel::ConcurrentQueue<uint8_t>> sockets_send_data;
//     // std::unordered_map<std::string,moodycamel::ConcurrentQueue<uint8_t>> username_send_data;//这里真的有必要用无锁队列吗，还是用条件变量呢？
// };

// template <typename T>
// std::shared_ptr<std::vector<T>> data_from_concurrentQueue(moodycamel::ConcurrentQueue<T> &queue)
// {
//     int n = 0;
//     int dequeued_count = MAX_DEQUEUE_NUMS;
//     std::shared_ptr<std::vector<T>> data = std::make_shared<std::vector<T>>();
//     do
//     {
//         n = queue.size_approx();
//         if (queue.try_dequeue_bulk(data->end(), n))
//         {
//             --dequeued_count;
//             continue;
//         }
//     } while (n > 0 && dequeued_count > 0);
//     return data;
// }

class ProactorEventHandler : public IEventHandler
{

};

