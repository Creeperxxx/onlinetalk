#include "dataManager.h"

void dataManager::init()
{
    data_queue = std::make_unique<std::queue<std::string>>();
}

void dataManager::pushData(const std::string& data)
{
    std::lock_guard<std::mutex> lock(data_mutex);
    data_queue->push(data);
}

std::string dataManager::popData()
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

void dataManager::pushReadySocketSingle(int socket)
{
    std::lock_guard<std::mutex> lock(ready_socket_mutex);
    ready_socket->push_back(socket);
    ready_socket_cond.notify_one();
}

void dataManager::pushReadySocketVec(const std::shared_ptr<std::vector<int>>& socket)
{
    std::lock_guard<std::mutex> lock(ready_socket_mutex);
    ready_socket->insert(ready_socket->end(), socket->begin(), socket->end());
    ready_socket_cond.notify_one();
}

int dataManager::popReadySocketSingle()
{
    // if (ready_socket->empty() == true) {
        // return READY_SOCKET_VEC_ISEMPTY;
    // }
    std::unique_lock<std::mutex> lock(ready_socket_mutex);
    ready_socket_cond.wait(lock, [this]() { return!ready_socket->empty(); });
    int socket = ready_socket->front();
    ready_socket->erase(ready_socket->begin());
    return socket;
}

std::shared_ptr<std::vector<int>> dataManager::popReadySocketVec()
{
    // if (ready_socket->empty() == true) {
        // return nullptr;
    // }
    std::unique_lock<std::mutex> lock(ready_socket_mutex);
    ready_socket_cond.wait(lock, [this]() { return!ready_socket->empty(); });
    std::shared_ptr<std::vector<int>> socket = std::make_shared<std::vector<int>>(ready_socket->begin(), ready_socket->end());
    ready_socket->clear();
    return socket;
}

// bool dataManager::isReadySocketEmpty()
// {
//     std::lock_guard<std::mutex> lock(ready_socket_mutex);
//     return ready_socket->empty();
// }