#include <string>
#include <mutex>
#include <queue>
#include <memory>
#include <vector>
#include <condition_variable>
// #include "../socketManager/socketManager.h"
#include "../userManager/userManager.h"

inline const int READY_SOCKET_VEC_ISEMPTY = -1;

class readySocketManager
{
private:
    std::unique_ptr<std::vector<int>> ready_sockets_vec;
    std::mutex mutex_ready_sockets_vec;
    std::condition_variable cv_ready_sockets_vec;

public:
    void push_readySocket_single(int socket);
    void push_readySocket_vec(std::unique_ptr<std::vector<int>> ready_sockets);
    int pop_readySocket_single();
    std::unique_ptr<std::vector<int>> pop_readySocket_vec();
};

// class dataManager
// {
// public:
//     static dataManager &get_instance();
//     dataManager(const dataManager &) = delete;
//     dataManager &operator=(const dataManager &) = delete;
//     void push_data(const std::string &data);
//     std::string pop_data();

//     // void push_ready_socket_single(int socket);
//     void push_readySocket_single(int socket);
//     // void push_ready_socket_vec(const std::shared_ptr<std::vector<int>> socket);
//     void push_readySocket_vec(std::unique_ptr<std::vector<int>> sockets);
//     // int pop_ready_socket_single();
//     int pop_readySocket_single();
//     // std::shared_ptr<std::vector<int>> pop_ready_socket_vec();
//     std::unique_ptr<std::vector<int>> pop_readySocket_vec();

//     // void push_data_to_socket(int socket,socket_data_type type ,std::shared_ptr<std::vector<uint8_t>> data);
//     // std::shared_ptr<std::vector<uint8_t>> pop_data_from_socket(int socket,socket_data_type type);
//     void push_recv_data_to_socket_queue(int socket,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_send_data_to_socket_queue(int socket,std::unique_ptr<std::vector<uint8_t>> data);
//     std::shared_ptr<socketUserInfo> pop_recv_data_from_socket_queue();
//     std::shared_ptr<socketUserInfo> pop_send_data_from_socket_queue();

//     void push_recv_data_to_socket_single_set(int socket,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_send_data_to_socket_single_set(int socket,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_recv_data_to_socket_multiple_set(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>);
//     void push_send_data_to_socket_multiple_set(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>);
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_recv_data_from_socket_multiple_set();
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_send_data_from_socket_multiple_set();
//     std::shared_ptr<socketUserInfo> pop_recv_data_from_socket_single_set();
//     std::shared_ptr<socketUserInfo> pop_send_data_from_socket_single_set();

//     void push_recv_data_to_socket_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_send_data_to_socket_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data);
//     void push_recv_data_to_socket_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>);
//     void push_send_data_to_socket_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>);
//     std::shared_ptr<socketUserInfo> pop_recv_data_from_socket_single_loop();
//     std::shared_ptr<socketUserInfo> pop_send_data_from_socket_single_loop();
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_recv_data_from_socket_multiple_loop();
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> pop_send_data_from_socket_multiple_loop();

//     std::unique_ptr<std::vector<std::string>> pop_all_logined_userid();

//     bool is_user_logined(const std::string& userid);
//     std::string get_userid_by_username(const std::string& username);

//     void push_userid(const std::string& userid);

//     // bool isReadySocketEmpty();
// private:
//     void init();
//     dataManager();
//     ~dataManager();
// private:
//     // std::vector<std::string> data;
//     // std::unique_ptr<std::queue<std::string>> data_queue;
//     // std::mutex data_mutex;
//     // std::condition_variable data_cond;

//     std::unique_ptr<readySocketManager> ready_socket_manager;

//     // std::unique_ptr<std::vector<int>> ready_socket;
//     // std::mutex ready_socket_mutex;
//     // std::condition_variable ready_socket_cond;

//     // std::unique_ptr<oldSocketManager> socket_manager;
//     // std::unique_ptr<socketUserManager> socket_manager;
//     // std::unique_ptr<redisStreamUserManager> redis_manager;
//     // std::unique_ptr<IuserManager> manager;
// };
class dataManager
{
private:
    void init();
    dataManager();
    ~dataManager();

public:
    static dataManager &get_instance();
    dataManager(const dataManager &) = delete;
    dataManager &operator=(const dataManager &) = delete;
    void push_readySocket_single(int socket);
    int pop_readySocket_single();

    void push_socket_recvData(int socket, std::unique_ptr<std::vector<uint8_t>> data);
    void push_socket_sendData(int socket, std::unique_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<User> pop_socket_recvData();
    std::shared_ptr<User> pop_socket_sendData();

    void add_socket(int socket);

private:
    std::unique_ptr<userManager> user_manager;
    std::unique_ptr<readySocketManager> ready_socket_manager;
};
