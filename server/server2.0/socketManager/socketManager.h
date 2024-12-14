#include "socketVec.h"
#include <condition_variable>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <set>
// #include "../requirement/moodycamel/concurrentqueue.h"

class socketManager
{
private:
    std::unordered_map<int,std::shared_ptr<socketVector>> socket_vecs;
    std::mutex mutex_socket_vecs;
    // std::condition_variable socket_vecs_cv;
    // std::unordered_set<int> updated_socket_recv_set; // 这里为啥要用集合来着
    std::vector<int> updated_socket_recv_vec;
    std::mutex mutex_recv_vec;
    std::condition_variable recv_vec_cv;
    
    // std::atomic<bool> is_updated; 

    // std::unordered_set<int> updated_socket_send_set;
    std::vector<int> updated_socket_send_vec;
    std::mutex mutex_send_vec;
    std::condition_variable send_vec_cv;

    // std::unordered_map<std::string,moodycamel::ConcurrentQueue<uint8_t>> sendto_offline_user_data;
    std::unordered_map<std::string,std::unique_ptr<std::vector<uint8_t>>> sendto_offline_user_data;
    std::mutex mutex_offline_user_data;
    // std::unordered_map<std::string,int> username_socketfd;

    // std::priority_queue<std::shared_ptr<socketVector>, std::vector<std::shared_ptr<socketVector>>, compareSocketVec> pq;
    // std::multiset<std::shared_ptr<socketVector>,compareSocketVec> interaction_time_socketvec_set;
    // std::multiset<std::shared_ptr<socketVector>> interaction_time_socketvec_set;
    std::multiset<std::shared_ptr<socketVector> , compareSocketVec> interaction_time_socketvec_set;
    std::mutex mutex_interact_time_set;

    static std::string error_username;
public:
    void add_socket_vec(const std::string& username,int socketfd);
    bool delete_socket_vec(int socketfd);
    void enqueue_recv_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);   
    void enqueue_send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data(int socketfd);
    std::shared_ptr<std::vector<uint8_t>> dequeue_send_data(int socketfd);
    // std::shared_ptr<std::unordered_set<int>> get_updated_socket_recv_vec();
    // std::shared_ptr<std::unordered_set<int>> get_updated_socket_send_vec();
    std::shared_ptr<std::vector<int>> get_updated_socket_recv_vec();
    std::shared_ptr<std::vector<int>> get_updated_socket_send_vec();
    void enqueue_willsend_data(std::string username,std::shared_ptr<std::vector<uint8_t>> data);
    void update_socket_interaction_time(int socketfd);
    int get_tobesend_heartbeat_socketfd();
    const std::string& get_username(int socketfd);
    // void init();
private:
    std::shared_ptr<socketVector> get_socket_vec(int socketfd);
};