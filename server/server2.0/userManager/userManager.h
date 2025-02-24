// #include "socketVec.h"
// #include "socketInfo.h"
// #include "socketData.h"
#include "userInfo.h"
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
#include <algorithm>
// #include "../requirement/moodycamel/concurrentqueue.h"

//升级为单例模式吧，不然策略类访问不到socketmanager。也不能采用依赖注入。

//互动时间更新时机:该套接字ready


//好烦，哇真的好烦啊。关于userinfo和usermanager和datamanager和correspondence的组合。烦死了。问题就是，首先，关于通信方式，
//我希望可以扩展通信方式，现有的有socket和redis stream。然后就是user和usermanager上出了问题。当扩展通信方式时，user也是要扩展的。
//例如，user类有基础的id name等信息，然后扩展了socket通信后，就要增加用户的socket信息，扩展redis stream通信后，就要增加redis stream的信息。
//如果采用继承的方式，那么岂不是就必须继承所有的前者的扩展，但如果我根本就没有使用到前者的通信呢？我的意思是，我会动态的选择使用哪些通信类，而user类也会根据
//我使用了哪些通信类，而动态的扩展user类需要包括的关于这些通信相关的信息例如如果通信使用了socket，user类中就要加上用户的socket信息。
//还有，如何设计usermanager。首先，这个类中一定有一个总的容器，存储所有登录了的用户的user。usermanager需要提供方法连接通信类和user类。那么
//就用针对每一个通信类搞一个usermanager？然后datamanager又要提供一个总的方法来管理数据，那么成员属性就要有各种usermanager。可是这样处理用户登录和用户的消息时，
//应该选择哪个usermanager呢？好烦人，想了这些问题想了老半天了，我是真不喜欢软件架构啊。目前打算不管扩展和开闭原则了。



inline const bool USERMANAGER_SOCKET_ISBOOLMAP_USING = false;

enum class UserConnectionType
{
    SOCKET,
    REDIS_STREAM,
    ALL,
};

class userManager
{
private:
    std::unique_ptr<std::vector<std::shared_ptr<User>>> users;
    std::mutex mutex_users;

    std::unique_ptr<std::unordered_map<std::string,std::shared_ptr<User>>> userId_to_user_map;
    std::mutex mutex_userId_to_user_map;
    
    //以下的这些都是保存不同的指针，但指向同一个对象
    std::unique_ptr<std::unordered_map<int,std::shared_ptr<User>>> fd_to_user_map;
    std::mutex mutex_fd_to_user_map;

    std::unique_ptr<std::unordered_map<std::string,std::shared_ptr<User>>> userStream_to_user_map;
    std::mutex mutex_userStream_user_map;
    
    std::unique_ptr<std::multiset<std::shared_ptr<User>,compareUserSocketInteractionTimeASC>> socket_users_isTimeOut_set;
    std::mutex mutex_socket_users_isTimeOut_set;
    
    std::unique_ptr<std::multiset<std::shared_ptr<User>,compareUserRedisStreamInteractionTimeASC>> redisStream_users_isTimeOut_set;
    std::mutex mutex_redisStream_users_isTimeOut_set;

    //关于socket通信方法的用户收发数据相关的容器
    //1.如果一个用户有recv的数据，那么将这个用户入队。send同理。这里是记录有数据用户的shared_ptr<User>
    std::unique_ptr<std::unordered_set<std::shared_ptr<User>>> socket_haveRecvData_user_set;
    std::mutex mutex_socket_haveRecvData_user_set;
    std::condition_variable cv_socket_haveRecvData_user_set;
    std::unique_ptr<std::unordered_set<std::shared_ptr<User>>> socket_haveSendData_user_set;
    std::mutex mutex_socket_haveSendData_user_set;
    std::condition_variable cv_socket_haveSendData_user_set;
public:
    void push_socket_haveRecvData_user_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> recv_data);
    void push_socket_haveRecvData_user_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> recv_data_map);
    void push_socket_haveSendData_user_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> send_data);
    void push_socket_haveSendData_user_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> send_data_map);

    std::shared_ptr<User> pop_socket_haveRecvData_user_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_haveRecvData_user_vec();
    std::shared_ptr<User> pop_socket_haveSendData_user_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_haveSendData_user_vec();

    //2. 使用set记录有数据的用户的socket。然后通过socket找到用户的std::shared_ptr<User>
    //这个和上一个都是用一个容器记录哪些user有数据。
private:
    std::unique_ptr<std::unordered_set<int>> socket_haveRecvData_socket_set;
    std::mutex mutex_socket_haveRecvData_socket_set;
    std::condition_variable cv_socket_haveRecvData_socket_set;
    std::unique_ptr<std::unordered_set<int>> socket_haveSendData_socket_set;
    std::mutex mutex_socket_haveSendData_socket_set;
    std::condition_variable cv_socket_haveSendData_socket_set;
public:
    void push_socket_haveRecvData_socket_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_haveRecvData_socket_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> recv_data_map);
    void push_socket_haveSendData_socket_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> send_data);
    void push_socket_haveSendData_socket_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> send_data_map);

    std::shared_ptr<User> pop_socket_haveRecvData_socket_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_haveRecvData_socket_vec();
    std::shared_ptr<User> pop_socket_haveSendData_socket_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_haveSendData_socket_vec();

    //3. 轮询的方式
    // std::condition_variable cv_fd_user_map;
    void push_socket_haveRecvData_loop_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_haveRecvData_loop_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> recv_data_map);
    void push_socket_haveSendData_loop_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_haveSendData_loop_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> send_data_map);
    std::shared_ptr<User> pop_socket_haveRecvData_loop_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_haveRecvData_loop_vec();
    std::shared_ptr<User> pop_socket_haveSendData_loop_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_haveSendData_loop_vec(); 

    //4.。。剩下的诸如：事件驱动 优先队列或哈希表 
    //先试试哈希表试试。其实问题是追踪一个基础容器的就绪元素。前面的都是新增一个容器记录就绪元素的思路。而现在的
    //哈希表则是变基础容器为哈希表。
private:
    // std::unique_ptr<std::unordered_map<bool,std::unique_ptr<std::vector<std::shared_ptr<User>>>>> socket_is_haveRecvData_map;
    std::unique_ptr<std::unordered_map<bool,std::unique_ptr<std::unordered_map<int,std::shared_ptr<User>>>>> socket_is_haveRecvData_map_map;
    std::mutex mutex_socket_is_haveRecvData_map;
    std::condition_variable cv_socket_is_haveRecvData_map;
    // std::unique_ptr<std::unordered_map<bool,std::unique_ptr<std::vector<std::shared_ptr<User>>>>> socket_is_haveSendData_map;
    std::unique_ptr<std::unordered_map<bool,std::unique_ptr<std::unordered_map<int,std::shared_ptr<User>>>>> socket_is_haveSendData_map_map;
    std::mutex mutex_socket_is_haveSendData_map;
    std::condition_variable cv_socket_is_haveSendData_map;

public:
    void push_socket_is_haveRecvData_map_map_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_is_haveRecvData_map_map_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> recv_data_map);
    void push_socket_is_haveSendData_map_map_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_is_haveSendData_map_map_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> send_data_map);
    std::shared_ptr<User> pop_socket_is_haveRecvData_map_map_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_is_haveRecvData_map_map_vec();
    std::shared_ptr<User> pop_socket_is_haveSendData_map_map_single();
    std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_is_haveSendData_map_map_vec();

    //5. 优先队列。
    //好像用不了啊，怎么从优先队列中找到特定元素并出队呢？好像无法解决
// private:
//     std::unique_ptr<std::priority_queue<std::shared_ptr<User>,std::vector<std::shared_ptr<User>>,compareUserSocketIsRecvDataEmpty>> socket_is_haveRecvData_priorityqueue;
//     std::mutex mutex_socket_is_haveRecvData_priorityqueue;
//     std::unique_ptr<std::priority_queue<std::shared_ptr<User>,std::vector<std::shared_ptr<User>>,compareUserSocketIsSendDataEmpty>> socket_is_haveSendData_priorityqueue;
//     std::mutex mutex_socket_is_haveSendData_priorityqueue;
// public:
//     void push_socket_is_haveRecvData_priorityqueue_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_socket_is_haveRecvData_priorityqueue_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> recv_data_map);
//     void push_socket_is_haveSendData_priorityqueue_single(int socketFd,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_socket_is_haveSendData_priorityqueue_map(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> send_data_map);
//     std::shared_ptr<User> pop_socket_is_haveRecvData_priorityqueue_single();
//     std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_is_haveRecvData_priorityqueue_vec();
//     std::shared_ptr<User> pop_socket_is_haveSendData_priorityqueue_single();
//     std::unique_ptr<std::vector<std::shared_ptr<User>>> pop_socket_is_haveSendData_priorityqueue_vec();

    //userid到离线user的映射
private:
    std::unique_ptr<std::unordered_map<std::string,std::shared_ptr<User>>> socket_userId_to_offlineUser_map;
    std::mutex mutex_socket_userId_to_offlineUser_map;
public:
    void push_socket_recvData_to_offlineUser_map(const std::string& userid,std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_sendData_to_offlineUser_map(const std::string& userid,std::unique_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<User> pop_socket_recvData_from_offlineUser_map(const std::string& userid);
    std::shared_ptr<User> pop_socket_sendData_from_offlineUser_map(const std::string& userid);


    //6. 未认领的数据
private:
    std::unique_ptr<std::vector<std::unique_ptr<std::vector<uint8_t>>>> socket_unclaimed_recv_data;
    std::mutex mutex_socket_unclaimed_recv_data;
    std::condition_variable cv_socket_unclaimed_recv_data;
    std::unique_ptr<std::vector<std::unique_ptr<std::vector<uint8_t>>>> socket_unclaimed_send_data;
    std::mutex mutex_socket_unclaimed_send_data;
    std::condition_variable cv_socket_unclaimed_send_data;
public:
    void push_socket_unclaimed_recv_data(std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_unclaimed_send_data(std::unique_ptr<std::vector<uint8_t>> data);
    std::unique_ptr<std::vector<uint8_t>> pop_socket_unclaimed_recv_data();
    std::unique_ptr<std::vector<uint8_t>> pop_socket_unclaimed_send_data();


    
public:
    //更新和弹出一个过期的
    void socket_update_user_interactionTime(const std::string& userid);
    std::shared_ptr<User> pop_socket_timeOut_user();
    
    void redisStream_update_user_interactionTime(const std::string& userid);
    std::shared_ptr<User> pop_redisStream_timeOut_user();

public:
    void add_user(std::shared_ptr<User> user);
    void add_user_socket(int socket);
    void del_user(const std::string& userid,UserConnectionType type);
private:
    void add_user_to_socket_fdUser_Map(std::shared_ptr<User> user);
    void add_user_to_socket_userTimeOut_set(std::shared_ptr<User> user);
    void add_user_to_socket_ishaveData_user_map(std::shared_ptr<User> user);
    void add_user_to_userIdToUser_map(std::shared_ptr<User> user);
    void add_user_to_userIdToOfflineUser_map(std::shared_ptr<User> user);
    // bool is_user_exist_at_socket(int socketFd);

public:
    static userManager& get_instance();
    userManager & operator=(const userManager& a) = delete;
    userManager(const userManager& a) = delete;
private:
    userManager();
    ~userManager(); 
};











/* enum class socket_data_type
{
    RECV,
    SEND
};


class IuserManager
{
public:
    virtual ~IuserManager() = 0;
    static bool is_userid_exist(const std::string& userid);
    static std::unique_ptr<std::vector<std::string>> get_all_logined_userid();
//  database::get_instance().add_logined_userid(userManager.get_all_logined_userid())

protected:
    void push_userid(const std::string& userid);
    void delete_userid(const std::string& userid);
    // virtual void push_all_userid() = 0;
protected:
    static std::unique_ptr<std::unordered_set<std::string>> logined_userid_set; 
    static std::mutex mutex_logined_userid_set;
};


class socketUserManager : public IuserManager
{
protected:
    // void push_all_userid() override;
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
    std::shared_ptr<socketUserInfo> pop_recv_socketdata_queue();
    std::shared_ptr<socketUserInfo> pop_send_socketdata_queue();

    //2中socket中的recv和send
    void push_recv_data_single_set(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data_single_set(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_recv_data_multiple_set(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>);
    void push_send_data_multiple_set(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>);
    // std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> pop_recv_data_map_map();
    // std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> pop_send_data_map_map();
    std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_recv_data_multiple_set();
    std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_send_data_multiple_set();
    std::shared_ptr<socketUserInfo> pop_recv_data_single_set();
    std::shared_ptr<socketUserInfo> pop_send_data_single_set();

    //3循环遍历
    void push_recv_data_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_recv_data_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data);
    void push_send_data_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data);
    std::shared_ptr<socketUserInfo> pop_recv_data_single_loop();
    std::shared_ptr<socketUserInfo> pop_send_data_single_loop();
    std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_recv_data_multiple_loop();
    std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_send_data_multiple_loop();

    socketUserManager();
    ~socketUserManager();
private:
    bool is_socket_exist(int socket);
    bool is_user_exist_userid(const std::string& a);
    bool is_user_exist_username(const std::string& a);
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
    // std::multiset<std::shared_ptr<socketUserInfo>> interaction_time_socketdata_set;
    std::multiset<std::shared_ptr<IuserInfo>,CompareUserInteractionTime> interaction_time_socketdata_set;
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
    
    std::unique_ptr<std::unordered_map<int,std::shared_ptr<socketUserInfo>>> socket_map;
    std::mutex mutex_socket_map;

    //1. 用queue实现了
    // std::unique_ptr<std::queue<std::unique_ptr<socketData>>> send_data_queue;
    std::unique_ptr<std::queue<std::shared_ptr<socketUserInfo>>> send_data_queue;
    std::mutex mutex_send_queue;
    std::condition_variable cond_send_queue;

    // std::unique_ptr<std::queue<std::unique_ptr<socketData>>> recv_data_queue;
    std::unique_ptr<std::queue<std::shared_ptr<socketUserInfo>>> recv_data_queue;
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

class redisStreamUserManager : public IuserManager
{
    
};

 */



// class oldSocketManager
// {
// private:
//     std::unordered_map<int,std::shared_ptr<socketInfo>> socket_map;
//     std::mutex mutex_socket_map;
//     // std::condition_variable socket_vecs_cv;
//     // std::unordered_set<int> updated_socket_recv_set; // 这里为啥要用集合来着
//     std::vector<int> updated_socket_recv_vec;
//     std::mutex mutex_recv_vec;
//     std::condition_variable recv_vec_cv;
    
//     // std::atomic<bool> is_updated; 

//     // std::unordered_set<int> updated_socket_send_set;
//     std::vector<int> updated_socket_send_vec;
//     std::mutex mutex_send_vec;
//     std::condition_variable send_vec_cv;

//     // std::unordered_map<std::string,moodycamel::ConcurrentQueue<uint8_t>> sendto_offline_user_data;
//     std::unordered_map<std::string,std::unique_ptr<std::vector<uint8_t>>> sendto_offline_user_data;
//     std::mutex mutex_offline_user_data;
//     // std::unordered_map<std::string,int> username_socketfd;

//     // std::priority_queue<std::shared_ptr<socketVector>, std::vector<std::shared_ptr<socketVector>>, compareSocketVec> pq;
//     // std::multiset<std::shared_ptr<socketVector>,compareSocketVec> interaction_time_socketvec_set;
//     // std::multiset<std::shared_ptr<socketVector>> interaction_time_socketvec_set;
//     // std::multiset<std::shared_ptr<socketVector> , compareSocketVec> interaction_time_socketvec_set;
//     std::multiset<std::shared_ptr<socketInfo>> interaction_time_socketvec_set;
//     std::mutex mutex_interact_time_set;

//     // static std::string error_username;

//     static std::unique_ptr<oldSocketManager> instance;
// public:
//     static oldSocketManager& getInstance();
//     void add_socket(int socketfd);
//     bool add_socket_vec(const std::string& userid,const std::string& username,int socketfd);
//     bool delete_socket_vec(int socketfd);
//     void enqueue_recv_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);   
//     void enqueue_send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data);
//     std::shared_ptr<std::vector<uint8_t>> dequeue_recv_data(int socketfd);
//     std::shared_ptr<std::vector<uint8_t>> dequeue_send_data(int socketfd);
//     // std::shared_ptr<std::unordered_set<int>> get_updated_socket_recv_vec();
//     // std::shared_ptr<std::unordered_set<int>> get_updated_socket_send_vec();
//     std::shared_ptr<std::vector<int>> get_updated_socket_recv_vec();
//     std::shared_ptr<std::vector<int>> get_updated_socket_send_vec();
//     void enqueue_offline_data(const std::string &userid,std::shared_ptr<std::vector<uint8_t>> data);
//     void update_socket_interaction_time(int socketfd);
//     int get_tobesend_heartbeat_socketfd();
//     const std::string& get_username(int socketfd);
//     int get_socket_by_userid(const std::string& userid);
//     // void init();
//     ~oldSocketManager(){}
//     oldSocketManager(const oldSocketManager&) = delete;
//     oldSocketManager& operator=(const oldSocketManager&) = delete;
//     static oldSocketManager& getInstance();
// private:
//     oldSocketManager(){}
//     // std::shared_ptr<socketInfo> get_socket_vec(int socketfd);
//     // bool is_username_exist(const std::string& username);
//     // bool is_userid_exist(const std::string& userid);
//     // void set_socket_isblocking(int socketfd,bool is_block);
// };


