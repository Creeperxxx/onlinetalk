#pragma once
#include "../networkio/networkio.h"
#include <sys/epoll.h>
#include <signal.h>
#include <atomic>
#include <functional>
#include <queue>
#include "../requirement/moodycamel/concurrentqueue.h"
#include "../threadPool/threadPool.h"

extern const int MAX_EPOLL_EVENTS;
extern const int THREAD_NUMS;
extern const int LISTEN_PORT;

extern std::atomic<bool> event_loop_running;
void event_loop_running_signal_handler(int signal);

class IEventHandler
{
public:
    virtual void handle_new_connections() = 0;
    virtual void handle_ready_connections(int socketfd) = 0;
};

class ReactorEventHandler : public IEventHandler
{
public:
    void deleter();
    void init();
    void event_loop();
private:
    void init_epoll();
    void add_socketfd_to_epoll(int socketfd,uint32_t events);
    // void accept_new_connection();
    void handle_new_connections() override;
    void handle_ready_sockets();
    // void handle_ready_connections(int socketfd) override;
    
private:
    int epoll_fd;
    std::atomic<int> handle_ready_socket_running;
    std::shared_ptr<NetworkIo> networkio;
    std::shared_ptr<ThreadPool> threadPool;
    moodycamel::ConcurrentQueue<int> ready_sockets;
};


























class ProactorEventHandler : public IEventHandler
{

};