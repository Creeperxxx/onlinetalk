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