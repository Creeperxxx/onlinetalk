#include "socketManager.h"

// void socketManager::enqueue_recv_data(int socketfd, std::shared_ptr<std::vector<uint8_t>> data)
// {
//     // auto it = get_socket_vec(socketfd);
//     // if (it == nullptr)
//     // {
//     //     return;
//     // }
//     // else
//     // {
//     //     std::unique_lock<std::mutex> lock(it->get_recv_data_mutex());
//     //     it->enqueue_recv_data(data);
//     //     it->get_recv_data_cv().notify_one();
//     //     return;
//     // }
// }
void socketManager::enqueue_recv_data(int socketfd, std::shared_ptr<std::vector<uint8_t>> data)
{
    auto it = get_socket_vec(socketfd);
    if (it == nullptr)
    {
        return;
    }
    else
    {
        std::unique_lock<std::mutex> lock(it->get_recv_data_mutex());
        it->enqueue_recv_data(data);
    }
    std::unique_lock<std::mutex> lock(mutex_recv_set);
    updated_socket_recv_set.insert(socketfd);
    recv_set_cv.notify_one();
}

void socketManager::enqueue_send_data(int socketfd,std::shared_ptr<std::vector<uint8_t>> data)
{
    auto it = get_socket_vec(socketfd);
    if(it == nullptr)
    {
        return;
    }
    else
    {
        std::unique_lock<std::mutex> lock(it->get_send_data_mutex());
        it->enqueue_send_data(data);
    }
    std::unique_lock<std::mutex> lock(mutex_send_set);
    updated_socket_send_set.insert(socketfd);
    send_set_cv.notify_one();
}


// void socketManager::enqueue_send_data(int socketfd, std::shared_ptr<std::vector<uint8_t>> data)
// {
//     auto it = get_socket_vec(socketfd);
//     if (it == nullptr)
//     {
//         return;
//     }
//     else
//     {
//         std::unique_lock<std::mutex> lock(it->get_send_data_mutex());
//         it->enqueue_send_data(data);
//         it->get_send_data_cv().notify_one();
//     }
// }

std::shared_ptr<std::vector<uint8_t>> socketManager::dequeue_recv_data(int socketfd)
{
    auto it = get_socket_vec(socketfd);
    if (it == nullptr)
    {
        return nullptr;
    }
    else
    {
        std::unique_lock<std::mutex> lock(it->get_recv_data_mutex());
        return it->dequeue_recv_data();
    }



    // auto it = get_socket_vec(socketfd);
    // if (it == nullptr)
    // {
    //     return nullptr;
    // }
    // else
    // {
    //     std::unique_lock<std::mutex> lock(it->get_recv_data_mutex());
    //     it->get_recv_data_cv().wait(lock, [it] { return !it->is_recv_data_empty();  });
    //     return it->dequeue_recv_data();
    // }
}

std::shared_ptr<socketVector> socketManager::get_socket_vec(int socketfd)
{
    auto it = socket_vecs.find(socketfd);
    if(it == socket_vecs.end())
    {
        LOG_ERROR("%s:%s:%d // 未找到套接字对应的socketVector", __FILE__, __func__, __LINE__);
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

std::shared_ptr<std::unordered_set<int>> socketManager::get_updated_socket_recv_set()
{
    std::unique_lock<std::mutex> lock(mutex_recv_set);
    while(updated_socket_recv_set.empty())
    {
        recv_set_cv.wait(lock);
    }
    auto data = std::make_shared<std::unordered_set<int>>(updated_socket_recv_set);
    updated_socket_recv_set.clear();
    return data;
}

std::shared_ptr<std::unordered_set<int>> socketManager::get_updated_socket_send_set()
{
    std::unique_lock<std::mutex> lock(mutex_send_set);
    while(updated_socket_send_set.empty())
    {
        send_set_cv.wait(lock);
    }
    auto data = std::make_shared<std::unordered_set<int>>(updated_socket_send_set);
    updated_socket_send_set.clear();
    return data;
}

void socketManager::add_socket_vec(int socket)
{
    socket_vecs[socket] = std::make_shared<socketVector>();
    //todo
    // int n =sendto_offline_user_data[socket].size_approx();
    // std::shared_ptr<std::vector<uint8_t>> temp = std::make_shared<std::vector<uint8_t>>();
    // if(n!=0)
    // {
    //     if(sendto_offline_user_data[socket].try_dequeue_bulk(temp->end(),n))
    //     {
    //         this->enqueue_send_data(socket,temp);
    //     }
    //     else
    //     {
    //         //失败了
    //         return;
    //     }
    // }
}

void socketManager::enqueue_willsend_data(std::string username,std::shared_ptr<std::vector<uint8_t>> data)
{
    sendto_offline_user_data[username].enqueue_bulk(data->data(),data->size());
}