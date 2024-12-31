#include "socketVec.h"
#include <condition_variable>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <set>
#include "../eventHandler/ehConstants.h"
#include <fcntl.h>
#include <string.h>
// #include "../requirement/moodycamel/concurrentqueue.h"

//升级为单例模式吧，不然策略类访问不到socketmanager。也不能采用依赖注入。

class socketManager
{
private:
    std::unordered_map<int,std::shared_ptr<socketVector>> socket_map;
    std::mutex mutex_socket_map;
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

    // static std::string error_username;

    static std::unique_ptr<socketManager> instance;
public:
    static socketManager& getInstance();
    void add_socket(int socketfd);
    bool add_socket_vec(const std::string& userid,const std::string& username,int socketfd);
    bool delete_socket_vec(int socketfd);
    void enqueue_recv_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);   
    void enqueue_send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data(int socketfd);
    std::shared_ptr<std::vector<uint8_t>> dequeue_send_data(int socketfd);
    // std::shared_ptr<std::unordered_set<int>> get_updated_socket_recv_vec();
    // std::shared_ptr<std::unordered_set<int>> get_updated_socket_send_vec();
    std::shared_ptr<std::vector<int>> get_updated_socket_recv_vec();
    std::shared_ptr<std::vector<int>> get_updated_socket_send_vec();
    void enqueue_offline_data(const std::string &userid,std::shared_ptr<std::vector<uint8_t>> data);
    void update_socket_interaction_time(int socketfd);
    int get_tobesend_heartbeat_socketfd();
    const std::string& get_username(int socketfd);
    int get_socket_by_userid(const std::string& userid);
    // void init();

    ~socketManager(){}
    socketManager(const socketManager&) = delete;
    socketManager& operator=(const socketManager&) = delete;
    static socketManager& getInstance();
private:
    socketManager(){}
    
    std::shared_ptr<socketVector> get_socket_vec(int socketfd);
    bool is_username_exist(const std::string& username);
    bool is_userid_exist(const std::string& userid);
    // void set_socket_isblocking(int socketfd,bool is_block);
};