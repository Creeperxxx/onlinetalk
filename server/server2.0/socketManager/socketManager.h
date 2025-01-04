// #include "socketVec.h"
// #include "socketInfo.h"
#include "socketData.h"
#include <condition_variable>
#include <unordered_set>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <set>
#include "../eventHandler/ehConstants.h"
#include <fcntl.h>
#include <string.h>
#include <queue>
#include <future>
// #include "../requirement/moodycamel/concurrentqueue.h"

//升级为单例模式吧，不然策略类访问不到socketmanager。也不能采用依赖注入。

//互动时间更新时机:该套接字ready

enum class socket_data_type
{
    RECV,
    SEND
};

class socketManager
{
public:
    // static socketManager& get_instance();
    // socketManager(const socketManager&) = delete;
    // socketManager& operator=(const socketManager& a) = delete;
    void add_socket(int socket);
    void remove_socket(int socket);

    //recv and send
    // void push_data_to_socket(int socket,socket_data_type type , std::shared_ptr<std::vector<uint8_t>> data);
    // std::shared_ptr<std::vector<uint8_t>> pop_data_from_socket(int socket,socket_data_type type);

    //ready socket
    void push_ready_socket_single(int socket);
    void push_ready_socket_multiple(const std::shared_ptr<std::vector<int>> vec);
    int pop_ready_socket_single();
    std::shared_ptr<std::vector<int>> pop_ready_socket_multiple();
    
    //1中socket中的recv和send
    void push_recv_data_queue(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data_queue(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<socketData> pop_recv_socketdata_queue();
    std::shared_ptr<socketData> pop_send_socketdata_queue();

    //2中socket中的recv和send
    void push_recv_data_single_map(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data_single_map(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_recv_data_multiple_map(std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>);
    void push_send_data_multiple_map(std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>);
    // std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> pop_recv_data_map_map();
    // std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> pop_send_data_map_map();
    std::unique_ptr<std::vector<std::shared_ptr<socketData>>> pop_recv_data_multiple_map();
    std::unique_ptr<std::vector<std::shared_ptr<socketData>>> pop_send_data_multiple_map();
    std::shared_ptr<socketData> pop_recv_data_single_map();
    std::shared_ptr<socketData> pop_send_data_single_map();

    //3循环遍历
    void push_recv_data_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_recv_data_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data);
    void push_send_data_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data);
    std::shared_ptr<socketData> pop_recv_data_single_loop();
    std::shared_ptr<socketData> pop_send_data_single_loop();
    std::unique_ptr<std::vector<std::shared_ptr<socketData>>> pop_recv_data_multiple_loop();
    std::unique_ptr<std::vector<std::shared_ptr<socketData>>> pop_send_data_multiple_loop();

    socketManager();
    ~socketManager();
private:
    bool is_socket_exist(int socket);
    void init();
    void deleter();
    // socketManager();
    // ~socketManager();
private:
    // std::unordered_map<int,std::unique_ptr<socketVector>> socket_map;
    // std::unordered_map<int,std::shared_ptr<socketInfo>> socket_map;

    // std::unordered_map<int,std::unique_ptr<socketData>> socket_map;
    // std::mutex mutex_socket_map;

    // std::multiset<std::shared_ptr<socketInfo>> interaction_time_socketvec_set;
    // std::mutex mutex_interact_time_set;
    std::multiset<std::shared_ptr<socketData>> interaction_time_socketdata_set;
    std::mutex mutex_interact_time_set;

    std::unique_ptr<std::vector<int>> ready_socket_vec;
    std::mutex mutex_ready_socket_vec;
    std::condition_variable cond_ready_socket_vec;

    //1. 单独一个send队列和一个recv队列
    // std::unique_ptr<std::queue<std::shared_ptr<socketInfo>>> send_queue;
    // std::unique_ptr<std::queue<std::unique_ptr<socketData>>> send_queue;

    //这里为啥要用map替代queue我给忘了啊
    // std::unique_ptr<std::unordered_map<int,std::unique_ptr<socketData>>> send_data_map;
    // std::mutex mutex_send_map;
    // std::condition_variable cond_send_map;

    // std::unique_ptr<std::unordered_map<int,std::unique_ptr<socketData>>> recv_data_map;
    // std::mutex mutex_recv_map;
    // std::condition_variable cond_recv_map;
    
    std::unique_ptr<std::unordered_map<int,std::shared_ptr<socketData>>> socket_map;
    std::mutex mutex_socket_map;

    //1. 用queue实现了
    // std::unique_ptr<std::queue<std::unique_ptr<socketData>>> send_data_queue;
    std::unique_ptr<std::queue<std::shared_ptr<socketData>>> send_data_queue;
    std::mutex mutex_send_queue;
    std::condition_variable cond_send_queue;

    // std::unique_ptr<std::queue<std::unique_ptr<socketData>>> recv_data_queue;
    std::unique_ptr<std::queue<std::shared_ptr<socketData>>> recv_data_queue;
    std::mutex mutex_recv_queue;
    std::condition_variable cond_recv_queue;

    //2. 维护一个set记录哪些有数据
    // std::unique_ptr<std::unordered_map<int,std::shared_ptr<socketData>>> socket_map;
    
    std::unique_ptr<std::set<int>> recv_notempty_socket_set;
    std::mutex mutex_recv_notempty_socket_set;
    std::condition_variable cond_recv_notempty_socket_set;

    std::unique_ptr<std::set<int>> send_notempty_socket_set;
    std::mutex mutex_send_notempty_socket_set;
    std::condition_variable cond_send_notempty_socket_set;

    //3. socketVec+循环遍历所有socket
    //实现了

    //4. 使用promise+future+socketVec,循环遍历所有future
    //这里到底要不要提供promise+future的方案啊，ai说这个方案不好，得使用条件变量才行。哎烦人
    std::unique_ptr<std::unordered_map<int,std::promise<void>>> recv_promise_map;
    std::unique_ptr<std::unordered_map<int,std::promise<void>>> send_promise_map;
    std::unique_ptr<std::unordered_map<int,std::future<void>>> recv_future_map;
    std::unique_ptr<std::unordered_map<int,std::future<void>>> send_future_map;

    // std::unordered_map<int , std::pair<std::promise<int>,std::promise<int>>> promise_socket_map;
    // std::mutex mutex_promise_socket_map;

    // std::unordered_map<int, std::pair<std::future<int>, std::future<int>>> future_socket_map;
    // std::mutex mutex_future_socket_map;
};


class oldSocketManager
{
private:
    std::unordered_map<int,std::shared_ptr<socketInfo>> socket_map;
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
    // std::multiset<std::shared_ptr<socketVector> , compareSocketVec> interaction_time_socketvec_set;
    std::multiset<std::shared_ptr<socketInfo>> interaction_time_socketvec_set;
    std::mutex mutex_interact_time_set;

    // static std::string error_username;

    static std::unique_ptr<oldSocketManager> instance;
public:
    static oldSocketManager& getInstance();
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
    ~oldSocketManager(){}
    oldSocketManager(const oldSocketManager&) = delete;
    oldSocketManager& operator=(const oldSocketManager&) = delete;
    static oldSocketManager& getInstance();
private:
    oldSocketManager(){}
    // std::shared_ptr<socketInfo> get_socket_vec(int socketfd);
    // bool is_username_exist(const std::string& username);
    // bool is_userid_exist(const std::string& userid);
    // void set_socket_isblocking(int socketfd,bool is_block);
};


