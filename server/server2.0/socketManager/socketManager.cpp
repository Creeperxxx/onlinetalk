#include "socketManager.h"

// std::string socketManager::error_username = "errorusername";

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
        it->enqueue_recv_data(data);
    }
    std::unique_lock<std::mutex> lock(mutex_recv_vec);
    updated_socket_recv_vec.push_back(socketfd);
    recv_vec_cv.notify_one();
}

void socketManager::enqueue_send_data(int socketfd, std::shared_ptr<std::vector<uint8_t>> data)
{
    auto it = get_socket_vec(socketfd);
    if (it == nullptr)
    {
        return;
    }
    else
    {
        it->enqueue_send_data(data);
    }
    std::unique_lock<std::mutex> lock(mutex_send_vec);
    // updated_socket_send_set.insert(socketfd);
    updated_socket_send_vec.push_back(socketfd);
    send_vec_cv.notify_one();
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
std::shared_ptr<std::vector<uint8_t>> socketManager::dequeue_send_data(int socketfd)
{
    auto it = get_socket_vec(socketfd);
    if (it == nullptr)
    {
        return nullptr;
    }
    else
    {
        return it->dequeue_send_data();
    }
}

std::shared_ptr<socketVector> socketManager::get_socket_vec(int socketfd)
{
    std::lock_guard<std::mutex> lock(mutex_socket_map);
    auto it = socket_map.find(socketfd);
    if (it == socket_map.end())
    {
        LOG_ERROR("%s:%s:%d // 未找到套接字对应的socketVector", __FILE__, __func__, __LINE__);
        return nullptr;
    }
    else
    {
        return it->second;
    }
}

// std::shared_ptr<std::unordered_set<int>> socketManager::get_updated_socket_recv_vec()
std::shared_ptr<std::vector<int>> socketManager::get_updated_socket_recv_vec()
{
    std::unique_lock<std::mutex> lock(mutex_recv_vec);
    // while(updated_socket_recv_set.empty())
    while (updated_socket_recv_vec.empty())
    {
        recv_vec_cv.wait(lock);
    }
    // return std::make_shared<std::unordered_set<int>>(std::move(updated_socket_recv_set));
    return std::make_shared<std::vector<int>>(std::move(updated_socket_recv_vec));
}

// std::shared_ptr<std::unordered_set<int>> socketManager::get_updated_socket_send_vec()
std::shared_ptr<std::vector<int>> socketManager::get_updated_socket_send_vec()
{
    std::unique_lock<std::mutex> lock(mutex_send_vec);
    // while(updated_socket_send_set.empty())
    while (updated_socket_send_vec.empty())
    {
        send_vec_cv.wait(lock);
    }
    // return std::make_shared<std::unordered_set<int>>(std::move(updated_socket_send_set));
    return std::make_shared<std::vector<int>>(std::move(updated_socket_send_vec));
}

// bool socketManager::add_socket_vec(const std::string &username, int socket)
bool socketManager::add_socket_vec(const std::string& userid,const std::string& username,int socketfd)
{
    {
        if( is_username_exist(username))
        {
            LOG_INFO("%s:%s:%d // 该用户已登录，不可再登录", __FILE__, __FUNCTION__, __LINE__);
            return false;
        }
        else
        {
            std::lock_guard<std::mutex> lock(mutex_socket_map);
            socket_map[socketfd] = std::make_shared<socketVector>(socketfd, username);
        }
        // socket_vecs[socket] = std::make_shared<socketVector>(socket, username);
    }
    {
        std::lock_guard<std::mutex> lock(mutex_interact_time_set);
        interaction_time_socketvec_set.insert(socket_map[socketfd]);
    }

    // interaction_time_socketvec_set.insert(socket_vecs[socket]);
    // pq.push(socket_vecs[socket]);
    // username_socketfd[username] = socket;
    std::shared_ptr<std::vector<uint8_t>> temp;
    {
        std::lock_guard<std::mutex> lock(mutex_offline_user_data);
        temp = std::make_shared<std::vector<uint8_t>>(std::move(*sendto_offline_user_data[username]));
        sendto_offline_user_data.erase(username);
    }
    enqueue_send_data(socketfd, temp);

    // auto temp = sendto_offline_user_data[username]
    // enqueue_send_data(socket,sendto_offline_user_data[username]);
    // sendto_offline_user_data.erase(username);

    // int n = sendto_offline_user_data[username].size_approx();
    // if( 0 != n)
    // {
    //     std::shared_ptr<std::vector<uint8_t>> temp = std::make_shared<std::vector<uint8_t>>();
    //     if(sendto_offline_user_data[username].try_dequeue_bulk(temp->end(),n))
    //     {
    //         this->enqueue_send_data(socket,temp);
    //     }
    // }
    // todo
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

void socketManager::enqueue_offline_data(const std::string &userid, std::shared_ptr<std::vector<uint8_t>> data)
{
    // sendto_offline_user_data[username].enqueue_bulk(data->data(),data->size());
    std::lock_guard<std::mutex> lock(mutex_offline_user_data);
    sendto_offline_user_data[userid]->insert(sendto_offline_user_data[userid]->end(), data->begin(), data->end());
}

bool socketManager::delete_socket_vec(int socket)
{
    auto it = get_socket_vec(socket);
    if (it == nullptr)
    {
        LOG_WARING("%s:%s:%d // 未找到套接字对应的socketVector", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    else
    {
        if (it->is_recv_data_empty() && it->is_send_data_empty())
        {
            std::lock_guard<std::mutex> lock(mutex_socket_map);
            socket_map.erase(socket);
            return true;
        }
        else
        {
            // todo 等待socketVector清空后再删除，如何实现？
            LOG_WARING("%s:%s:%d // 套接字对应的socketVector未清空", __FILE__, __FUNCTION__, __LINE__);
            return false;
        }
    }
}

// void socketManager::init()
// {

// }

void socketManager::update_socket_interaction_time(int socketfd)
{
    auto vec = get_socket_vec(socketfd);
    if (vec == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 更新套接字的latesttime时找不到套接字的vec", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    else
    {
        // if(interaction_time_socketvec_set.find(it) == interaction_time_socketvec_set.end())
        auto it = interaction_time_socketvec_set.find(vec);
        if (it == interaction_time_socketvec_set.end())
        {
            LOG_ERROR("%s:%s:%d // 更新套接字的latesttime时找不到套接字的vec", __FILE__, __FUNCTION__, __LINE__);
            return;
        }
        else
        {
            std::lock_guard<std::mutex> lock(mutex_interact_time_set);
            interaction_time_socketvec_set.erase(it);
            (*it)->update_last_interaction_time();
            interaction_time_socketvec_set.insert(vec);
        }
    }
}

int socketManager::get_tobesend_heartbeat_socketfd()
{
    while (true)
    {
        {
            // 获取锁并检查集合是否为空
            std::lock_guard<std::mutex> lock(mutex_interact_time_set);

            if (!interaction_time_socketvec_set.empty())
            {
                // 如果集合不为空，退出循环
                break;
            }
        } // 锁在这里被释放

        // 集合为空，释放锁并休眠一段时间
        std::this_thread::sleep_for(std::chrono::seconds(TIME_OUT)); 
    }

    // 再次获取锁以确保线程安全地访问集合
    std::lock_guard<std::mutex> lock(mutex_interact_time_set);

    // 检查第一个元素是否过期
    if (!interaction_time_socketvec_set.empty() && (*interaction_time_socketvec_set.begin())->is_timeout())
    {
        return (*interaction_time_socketvec_set.begin())->get_fd();
    }
    else
    {
        return -1; // 如果没有过期或集合为空（理论上这里不会为空）
    }

// std::lock_guard<std::mutex> lock(mutex_interact_time_set);

// if(interaction_time_socketvec_set.empty())
// {
//     LOG_WARING("%s:%s:%d // 套接字set中没有套接字", __FILE__, __FUNCTION__, __LINE__);
//     return -1;
// }
// else
// {
//     if((*interaction_time_socketvec_set.begin())->is_timeout())
//     {
//         return (*interaction_time_socketvec_set.begin())->get_fd();
//     }
//     else
//     {
//         return -2;
//     }
// }
}

const std::string &socketManager::get_username(int socketfd)
{
    auto it = get_socket_vec(socketfd);
    if (it == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 未找到套接字对应的socketVector", __FILE__, __FUNCTION__, __LINE__);
        return "";
    }
    else
    {
        return it->get_username();
    }
}

socketManager& socketManager::getInstance()
{
    static socketManager instance;
    return instance;
}

bool socketManager::is_username_exist(const std::string& username)
{
    std::lock_guard<std::mutex> lock(mutex_socket_map);
    for (auto it : socket_map)
    {
        if (it.second->get_username() == username)
        {
            return true;
        }
    }
    return false;
}

int socketManager::get_socket_by_userid(const std::string& userid)
{
    std::lock_guard<std::mutex> lock(mutex_socket_map);
    for(auto it : socket_map)
    {
        if(it.second->get_userid() == userid)
        {
            return it.first;
        }
    }
    return USER_OFFLINE;
}

// void socketManager::set_socket_isblocking(int socket_fd, bool blocking)
// {
//     int flags = fcntl(socket_fd, F_GETFL, 0);
//     if (flags == -1) {
//         // std::cerr << "Error getting file status flags: " << strerror(errno) << std::endl;
//         LOG_ERROR("%s:%s:%d // 错误获取文件状态标志: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
//     }

//     if (blocking) {
//         // 设置为阻塞模式
//         if (fcntl(socket_fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
//             // std::cerr << "Error setting socket to blocking mode: " << strerror(errno) << std::endl;
//             LOG_ERROR("%s:%s:%d // 错误设置套接字为阻塞模式: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
//         }
//     } else {
//         // 设置为非阻塞模式
//         if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
//             // std::cerr << "Error setting socket to non-blocking mode: " << strerror(errno) << std::endl;
//             LOG_ERROR("%s:%s:%d // 错误设置套接字为非阻塞模式: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
//         }
//     }
// }

void socketManager::add_socket(int socketfd)
{
    std::lock_guard<std::mutex> lock(mutex_socket_map);
    socket_map[socketfd] = std::make_shared<socketVector>(socketfd);
    // set_socket_isblocking(socketfd,false);
    // return true;
}