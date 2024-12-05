#pragma once
#include "../networkio/networkio.h"
#include <sys/epoll.h>
#include <signal.h>
#include <atomic>
#include <functional>
#include <queue>
#include "../requirement/moodycamel/concurrentqueue.h"
#include "../serializationMethod/serializationMethod.h"
#include "../threadPool/threadPool.h"
#include <unordered_map>
#include <zlib.h>
#include "../msgAnalysis/msgAnalysis.h"

extern const int MAX_EPOLL_EVENTS;
extern const int THREAD_NUMS;
extern const int LISTEN_PORT;
extern const int MAX_DEQUEUE_NUMS;

extern const int FIND_USERNAME_FAILED;
extern const int FIND_USER_SOCKET_FAILED;

extern std::atomic<bool> event_loop_running;
void event_loop_running_signal_handler(int signal);


class IEventHandler
{
public:
    virtual void handle_new_connections() = 0;
    // virtual void handle_ready_connections(int socketfd) = 0;
};

class ReactorEventHandler : public IEventHandler
{
public:
    void deleter();
    void init();
    void event_loop();
    void handle_sockets_recv();
    void handle_sockets_send();//todo 同时检查是否有要发给为上线用户的消息。
    void analyze_recv_data();
    void enqueue_send_message(std::shared_ptr<message> data);

private:
    void init_epoll();
    void add_socketfd_to_epoll(int socketfd, uint32_t events);
    // void accept_new_connection();
    void handle_new_connections() override;
    // void handle_ready_connections(int socketfd) override;
    template <typename T>
    std::shared_ptr<std::vector<T>> data_from_concurrentQueue(moodycamel::ConcurrentQueue<T> &queue);
    uint32_t calculateCRC32(const uint8_t *data, size_t length);
    int get_socket_from_username(const std::string& username);
    int get_socket_from_userid(int userid);

private:
    int epoll_fd;
    std::atomic<int> handle_sockets_recv_running;
    std::atomic<int> handle_sockets_send_running;
    std::atomic<int> analyze_recv_data_running;
    std::shared_ptr<NetworkIo> networkio;
    std::shared_ptr<ThreadPool> threadPool;
    std::shared_ptr<msg_analysis> msgAnalysis;
    std::shared_ptr<IserializationMethod> serializationMethod;
    moodycamel::ConcurrentQueue<int> ready_sockets;
    


    // 这里有两种方案设计，一种是无锁队列存放二进制，另一种是无锁队列存放vector，每个vector存放一个消息的二进制
    //  std::unordered_map<int,moodycamel::ConcurrentQueue<std::vector<uint8_t>>> sockets_recv_data;
    //  std::unordered_map<int,moodycamel::ConcurrentQueue<std::vector<uint8_t>>> sockets_send_data;
    std::unordered_map<int, moodycamel::ConcurrentQueue<uint8_t>> sockets_recv_data;
    std::unordered_map<int, moodycamel::ConcurrentQueue<uint8_t>> sockets_send_data;
    std::unordered_map<std::string,moodycamel::ConcurrentQueue<uint8_t>> username_send_data;
};

template <typename T>
std::shared_ptr<std::vector<T>> data_from_concurrentQueue(moodycamel::ConcurrentQueue<T> &queue)
{
    int n = 0;
    int dequeued_count = MAX_DEQUEUE_NUMS;
    std::shared_ptr<std::vector<T>> data = std::make_shared<std::vector<T>>();
    do
    {
        n = queue.size_approx();
        if (queue.try_dequeue_bulk(data->end(), n))
        {
            --dequeued_count;
            continue;
        }
    } while (n > 0 && dequeued_count > 0);
    return data;
}

class ProactorEventHandler : public IEventHandler
{

};