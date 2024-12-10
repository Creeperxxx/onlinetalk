#include "socketVec.h"
#include <condition_variable>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include "../requirement/moodycamel/concurrentqueue.h"

class socketManager
{
private:
    std::unordered_map<int,std::shared_ptr<socketVector>> socket_vecs;
    // std::condition_variable socket_vecs_cv;
    std::unordered_set<int> updated_socket_recv_set;
    std::mutex mutex_recv_set;
    // std::atomic<bool> is_updated; 
    std::condition_variable recv_set_cv;

    std::unordered_set<int> updated_socket_send_set;
    std::mutex mutex_send_set;
    std::condition_variable send_set_cv;

    std::unordered_map<std::string,moodycamel::ConcurrentQueue<uint8_t>> sendto_offline_user_data;
public:
    void add_socket_vec(int socketfd);
    bool delete_socket_vec(int socketfd);
    void enqueue_recv_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);   
    void enqueue_send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data(int socketfd);
    std::shared_ptr<std::vector<uint8_t>> dequeue_send_data(int socketfd);
    std::shared_ptr<std::unordered_set<int>> get_updated_socket_recv_set();
    std::shared_ptr<std::unordered_set<int>> get_updated_socket_send_set();
    void enqueue_willsend_data(std::string username,std::shared_ptr<std::vector<uint8_t>> data);
private:
    std::shared_ptr<socketVector> get_socket_vec(int socketfd);
};