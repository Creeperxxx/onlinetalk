#include "dataManager.h"

void dataManager::init()
{
    data_queue = std::make_unique<std::queue<std::string>>();
    // ready_socket = std::make_unique<std::vector<int>>();
    // socket_manager = std::make_unique<oldSocketManager>();
    socket_manager = std::make_unique<socketManager>();
}

void dataManager::push_data(const std::string& data)
{
    std::lock_guard<std::mutex> lock(data_mutex);
    data_queue->push(data);
}

std::string dataManager::pop_data()
{
    std::lock_guard<std::mutex> lock(data_mutex);
    if (data_queue->empty()) {
        return "";
    }
    std::string data = data_queue->front();
    data_queue->pop();
    return data;
}

dataManager& dataManager::get_instance()
{
    static dataManager instance;
    return instance;
}

dataManager::dataManager()
{
    init();
}

void dataManager::push_ready_socket_single(int socket)
{
    // std::lock_guard<std::mutex> lock(ready_socket_mutex);
    // ready_socket->push_back(socket);
    // ready_socket_cond.notify_one();
    socket_manager->push_ready_socket_single(socket);
}

void dataManager::push_ready_socket_vec(const std::shared_ptr<std::vector<int>> socket)
{
    // std::lock_guard<std::mutex> lock(ready_socket_mutex);
    // ready_socket->insert(ready_socket->end(), socket->begin(), socket->end());
    // ready_socket_cond.notify_one();
    socket_manager->push_ready_socket_multiple(socket);
}

int dataManager::pop_ready_socket_single()
{
    // if (ready_socket->empty() == true) {
        // return READY_SOCKET_VEC_ISEMPTY;
    // }
    // std::unique_lock<std::mutex> lock(ready_socket_mutex);
    // ready_socket_cond.wait(lock, [this]() { return!ready_socket->empty(); });
    // int socket = ready_socket->front();
    // ready_socket->erase(ready_socket->begin());
    // return socket;
    return socket_manager->pop_ready_socket_single();
}

std::shared_ptr<std::vector<int>> dataManager::pop_ready_socket_vec()
{
    // if (ready_socket->empty() == true) {
        // return nullptr;
    // }
    // std::unique_lock<std::mutex> lock(ready_socket_mutex);
    // ready_socket_cond.wait(lock, [this]() { return!ready_socket->empty(); });
    // std::shared_ptr<std::vector<int>> socket = std::make_shared<std::vector<int>>(ready_socket->begin(), ready_socket->end());
    // ready_socket->clear();
    // return socket;
    return socket_manager->pop_ready_socket_multiple();
}

// bool dataManager::isReadySocketEmpty()
// {
//     std::lock_guard<std::mutex> lock(ready_socket_mutex);
//     return ready_socket->empty();
// }

// void dataManager::push_data_to_socket(int socket,socket_data_type type ,std::shared_ptr<std::vector<uint8_t>> data)
// {
//     socket_manager->push_data_to_socket(socket,type,data);
// }

// std::shared_ptr<std::vector<uint8_t>> dataManager::pop_data_from_socket(int socket,socket_data_type type)
// {
//     return socket_manager->pop_data_from_socket(socket,type);
// }

void dataManager::push_recv_data_to_socket_queue(int socket,std::unique_ptr<std::vector<uint8_t>> data)
{
    socket_manager->push_recv_data_queue(socket,std::move(data));
}

void dataManager::push_send_data_to_socket_queue(int socket,std::unique_ptr<std::vector<uint8_t>> data)
{
    socket_manager->push_send_data_queue(socket,std::move(data));
}

std::shared_ptr<socketData> dataManager::pop_recv_data_from_socket_queue()
{
    return socket_manager->pop_recv_socketdata_queue();
}

std::shared_ptr<socketData> dataManager::pop_send_data_from_socket_queue()
{
    return socket_manager->pop_send_socketdata_queue();
}

void dataManager::push_recv_data_to_socket_single_set(int socket,std::unique_ptr<std::vector<uint8_t>> data)
{
    socket_manager->push_recv_data_single_set(socket,std::move(data));
}

void dataManager::push_send_data_to_socket_single_set(int socket,std::unique_ptr<std::vector<uint8_t>> data)
{
    socket_manager->push_send_data_single_set(socket,std::move(data));
}

void dataManager::push_recv_data_to_socket_multiple_set(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data)
{
    socket_manager->push_recv_data_multiple_set(std::move(data));
}

void dataManager::push_send_data_to_socket_multiple_set(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data)
{
    socket_manager->push_send_data_multiple_set(std::move(data));
}

std::unique_ptr<std::vector<std::shared_ptr<socketData>>> dataManager::pop_recv_data_from_socket_multiple_set()
{
    return std::move(socket_manager->pop_recv_data_multiple_set());
}

std::unique_ptr<std::vector<std::shared_ptr<socketData>>> dataManager::pop_send_data_from_socket_multiple_set()
{
    return std::move(socket_manager->pop_send_data_multiple_set());
}

std::shared_ptr<socketData> dataManager::pop_recv_data_from_socket_single_set()
{
    return socket_manager->pop_recv_data_single_set();
}

std::shared_ptr<socketData> dataManager::pop_send_data_from_socket_single_set()
{
    return socket_manager->pop_send_data_single_set();
}

void dataManager::push_recv_data_to_socket_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data)
{
    socket_manager->push_recv_data_single_loop(socket,std::move(data));
}

void dataManager::push_send_data_to_socket_single_loop(int socket,std::unique_ptr<std::vector<uint8_t>> data)
{
    socket_manager->push_send_data_single_loop(socket,std::move(data));
}

void dataManager::push_recv_data_to_socket_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data)
{
    socket_manager->push_recv_data_multiple_loop(std::move(data));
}

void dataManager::push_send_data_to_socket_multiple_loop(std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data)
{
    socket_manager->push_send_data_multiple_loop(std::move(data));
}

std::shared_ptr<socketData> dataManager::pop_recv_data_from_socket_single_loop()
{
    return socket_manager->pop_recv_data_single_loop();
}

std::shared_ptr<socketData> dataManager::pop_send_data_from_socket_single_loop()
{
    return socket_manager->pop_send_data_single_loop();
}

std::unique_ptr<std::vector<std::shared_ptr<socketData>>> dataManager::pop_recv_data_from_socket_multiple_loop()
{
    return std::move(socket_manager->pop_recv_data_multiple_loop());
}

std::unique_ptr<std::vector<std::shared_ptr<socketData>>> dataManager::pop_send_data_from_socket_multiple_loop()
{
    return std::move(socket_manager->pop_send_data_multiple_loop());
}



