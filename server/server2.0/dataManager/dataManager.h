#include <string>
#include <mutex>
#include <queue>
#include <memory>
#include <vector>
#include <condition_variable>
#include "../socketManager/socketManager.h"

inline const int READY_SOCKET_VEC_ISEMPTY = -1;

class dataManager
{
public:
    static dataManager &get_instance();
    dataManager(const dataManager &) = delete;
    dataManager &operator=(const dataManager &) = delete;
    void push_data(const std::string &data);
    std::string pop_data();

    void push_ready_socket_single(int socket);
    void push_ready_socket_vec(const std::shared_ptr<std::vector<int>> socket);
    int pop_ready_socket_single();
    std::shared_ptr<std::vector<int>> pop_ready_socket_vec();

    // void push_data_to_socket(int socket,socket_data_type type ,std::shared_ptr<std::vector<uint8_t>> data);
    // std::shared_ptr<std::vector<uint8_t>> pop_data_from_socket(int socket,socket_data_type type);
    void push_recv_data_to_socket(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    void push_send_data_to_socket(int socket,std::unique_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<socketData> pop_recv_data_from_socket();
    std::shared_ptr<socketData> pop_send_data_from_socket();

    // bool isReadySocketEmpty();
private:
    void init();
    dataManager();
    ~dataManager();
private:
    // std::vector<std::string> data;
    std::unique_ptr<std::queue<std::string>> data_queue;
    std::mutex data_mutex;
    std::condition_variable data_cond;

    // std::unique_ptr<std::vector<int>> ready_socket;
    // std::mutex ready_socket_mutex;
    // std::condition_variable ready_socket_cond;

    // std::unique_ptr<oldSocketManager> socket_manager;
    std::unique_ptr<socketManager> socket_manager;
};
