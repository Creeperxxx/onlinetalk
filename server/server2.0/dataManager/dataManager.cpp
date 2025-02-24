#include "dataManager.h"

// void dataManager::init()
// {
//     data_queue = std::make_unique<std::queue<std::string>>();
//     // ready_socket = std::make_unique<std::vector<int>>();
//     // socket_manager = std::make_unique<oldSocketManager>();
//     // socket_manager = std::make_unique<socketUserManager>();
// }

// void dataManager::push_data(const std::string &data)
// {
//     std::lock_guard<std::mutex> lock(data_mutex);
//     data_queue->push(data);
// }

// std::string dataManager::pop_data()
// {
//     std::lock_guard<std::mutex> lock(data_mutex);
//     if (data_queue->empty())
//     {
//         return "";
//     }
//     std::string data = data_queue->front();
//     data_queue->pop();
//     return data;
// }

// dataManager &dataManager::get_instance()
// {
//     static dataManager instance;
//     return instance;
// }

// dataManager::dataManager()
// {
//     init();
// }

// void dataManager::push_readySocket_single(int socket)
// {
//     // std::lock_guard<std::mutex> lock(ready_socket_mutex);
//     // ready_socket->push_back(socket);
//     // ready_socket_cond.notify_one();
//     // socket_manager->push_ready_socket_single(socket);
//     ready_socket_manager->push_readySocket_single(socket);
// }

// void dataManager::push_readySocket_vec(std::unique_ptr<std::vector<int>> sockets)
// {
//     // std::lock_guard<std::mutex> lock(ready_socket_mutex);
//     // ready_socket->insert(ready_socket->end(), socket->begin(), socket->end());
//     // ready_socket_cond.notify_one();
//     // socket_manager->push_ready_socket_multiple(socket);
//     ready_socket_manager->push_readySocket_vec(std::move(sockets));
// }

// int dataManager::pop_readySocket_single()
// {
//     // if (ready_socket->empty() == true) {
//     // return READY_SOCKET_VEC_ISEMPTY;
//     // }
//     // std::unique_lock<std::mutex> lock(ready_socket_mutex);
//     // ready_socket_cond.wait(lock, [this]() { return!ready_socket->empty(); });
//     // int socket = ready_socket->front();
//     // ready_socket->erase(ready_socket->begin());
//     // return socket;
//     // return socket_manager->pop_ready_socket_single();
//     return ready_socket_manager->pop_readySocket_single();
// }

// // std::shared_ptr<std::vector<int>> dataManager::pop_ready_socket_vec()
// std::unique_ptr<std::vector<int>> dataManager::pop_readySocket_vec()
// {
//     // if (ready_socket->empty() == true) {
//     // return nullptr;
//     // }
//     // std::unique_lock<std::mutex> lock(ready_socket_mutex);
//     // ready_socket_cond.wait(lock, [this]() { return!ready_socket->empty(); });
//     // std::shared_ptr<std::vector<int>> socket = std::make_shared<std::vector<int>>(ready_socket->begin(), ready_socket->end());
//     // ready_socket->clear();
//     // return socket;
//     // return socket_manager->pop_ready_socket_multiple();
//     return std::move(ready_socket_manager->pop_readySocket_vec());
// }

// // bool dataManager::isReadySocketEmpty()
// // {
// //     std::lock_guard<std::mutex> lock(ready_socket_mutex);
// //     return ready_socket->empty();
// // }

// // void dataManager::push_data_to_socket(int socket,socket_data_type type ,std::shared_ptr<std::vector<uint8_t>> data)
// // {
// //     socket_manager->push_data_to_socket(socket,type,data);
// // }

// // std::shared_ptr<std::vector<uint8_t>> dataManager::pop_data_from_socket(int socket,socket_data_type type)
// // {
// //     return socket_manager->pop_data_from_socket(socket,type);
// // }

// void dataManager::push_recv_data_to_socket_queue(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_recv_data_queue(socket, std::move(data));
// }

// void dataManager::push_send_data_to_socket_queue(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_send_data_queue(socket, std::move(data));
// }

// std::shared_ptr<socketUserInfo> dataManager::pop_recv_data_from_socket_queue()
// {
//     return socket_manager->pop_recv_socketdata_queue();
// }

// std::shared_ptr<socketUserInfo> dataManager::pop_send_data_from_socket_queue()
// {
//     return socket_manager->pop_send_socketdata_queue();
// }

// void dataManager::push_recv_data_to_socket_single_set(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_recv_data_single_set(socket, std::move(data));
// }

// void dataManager::push_send_data_to_socket_single_set(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_send_data_single_set(socket, std::move(data));
// }

// void dataManager::push_recv_data_to_socket_multiple_set(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     socket_manager->push_recv_data_multiple_set(std::move(data));
// }

// void dataManager::push_send_data_to_socket_multiple_set(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     socket_manager->push_send_data_multiple_set(std::move(data));
// }

// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> dataManager::pop_recv_data_from_socket_multiple_set()
// {
//     return std::move(socket_manager->pop_recv_data_multiple_set());
// }

// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> dataManager::pop_send_data_from_socket_multiple_set()
// {
//     return std::move(socket_manager->pop_send_data_multiple_set());
// }

// std::shared_ptr<socketUserInfo> dataManager::pop_recv_data_from_socket_single_set()
// {
//     return socket_manager->pop_recv_data_single_set();
// }

// std::shared_ptr<socketUserInfo> dataManager::pop_send_data_from_socket_single_set()
// {
//     return socket_manager->pop_send_data_single_set();
// }

// void dataManager::push_recv_data_to_socket_single_loop(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_recv_data_single_loop(socket, std::move(data));
// }

// void dataManager::push_send_data_to_socket_single_loop(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_send_data_single_loop(socket, std::move(data));
// }

// void dataManager::push_recv_data_to_socket_multiple_loop(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     socket_manager->push_recv_data_multiple_loop(std::move(data));
// }

// void dataManager::push_send_data_to_socket_multiple_loop(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     socket_manager->push_send_data_multiple_loop(std::move(data));
// }

// std::shared_ptr<socketUserInfo> dataManager::pop_recv_data_from_socket_single_loop()
// {
//     return socket_manager->pop_recv_data_single_loop();
// }

// std::shared_ptr<socketUserInfo> dataManager::pop_send_data_from_socket_single_loop()
// {
//     return socket_manager->pop_send_data_single_loop();
// }

// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> dataManager::pop_recv_data_from_socket_multiple_loop()
// {
//     return std::move(socket_manager->pop_recv_data_multiple_loop());
// }

// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> dataManager::pop_send_data_from_socket_multiple_loop()
// {
//     return std::move(socket_manager->pop_send_data_multiple_loop());
// }

// std::unique_ptr<std::vector<std::string>> dataManager::pop_all_logined_userid()
// {
//     return std::move(socket_manager->get_all_logined_userid());
// }

// bool dataManager::is_user_logined(const std::string &userid)
// {
//     return socket_manager->is_userid_exist(userid) || redis_manager->is_userid_exist(userid);
// }

// std::string dataManager::get_userid_by_username(const std::string &username)
// {
// }

// void dataManager::push_userid(const std::string &userid)
// {
//     // socket_manager
// }

void readySocketManager::push_readySocket_single(int socket)
{
    std::lock_guard<std::mutex> lock(mutex_ready_sockets_vec);
    ready_sockets_vec->push_back(socket);
    cv_ready_sockets_vec.notify_one();
}

void readySocketManager::push_readySocket_vec(std::unique_ptr<std::vector<int>> ready_sockets)
{
    std::lock_guard<std::mutex> lock(mutex_ready_sockets_vec);
    ready_sockets_vec->insert(ready_sockets_vec->end(), ready_sockets->begin(), ready_sockets->end());
    cv_ready_sockets_vec.notify_one();
}

int readySocketManager::pop_readySocket_single()
{
    std::unique_lock<std::mutex> lock(mutex_ready_sockets_vec);
    cv_ready_sockets_vec.wait(lock, [&]
                              { return !ready_sockets_vec->empty(); });
    int socket = ready_sockets_vec->back();
    ready_sockets_vec->pop_back();
    return socket;
}

std::unique_ptr<std::vector<int>> readySocketManager::pop_readySocket_vec()
{
    std::unique_lock<std::mutex> lock(mutex_ready_sockets_vec);
    cv_ready_sockets_vec.wait(lock, [&]
                              { return !ready_sockets_vec->empty(); });

    std::unique_ptr<std::vector<int>> ready_sockets_vec_tmp = std::make_unique<std::vector<int>>(*ready_sockets_vec);
    return std::move(ready_sockets_vec_tmp);
}

dataManager& dataManager::get_instance()
{
    static dataManager instance;
    return instance;
}

void dataManager::push_readySocket_single(int socket)
{
    ready_socket_manager->push_readySocket_single(socket);
}

int dataManager::pop_readySocket_single()
{
    return ready_socket_manager->pop_readySocket_single();
}

void dataManager::push_socket_recvData(int socket, std::unique_ptr<std::vector<uint8_t>> data)
{
    user_manager->push_socket_haveRecvData_user_single(socket, std::move(data));
}

void dataManager::push_socket_sendData(int socket, std::unique_ptr<std::vector<uint8_t>> data)
{
    user_manager->push_socket_haveSendData_user_single(socket, std::move(data));
}

std::shared_ptr<User> dataManager::pop_socket_recvData()
{
    return user_manager->pop_socket_haveRecvData_user_single();
}

std::shared_ptr<User> dataManager::pop_socket_sendData()
{
    return user_manager->pop_socket_haveSendData_user_single();
}

void dataManager::add_socket(int socket)
{
    user_manager->add_user_socket(socket);
}



