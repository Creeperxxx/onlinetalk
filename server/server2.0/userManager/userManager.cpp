// #include "socketManager.h"
#include "userManager.h"

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

// void oldSocketManager::enqueue_recv_data(int socketfd, std::shared_ptr<std::vector<uint8_t>> data)
// {
//     auto it = get_socket_vec(socketfd);
//     if (it == nullptr)
//     {
//         return;
//     }
//     else
//     {
//         it->enqueue_recv_data(data);
//     }
//     std::unique_lock<std::mutex> lock(mutex_recv_vec);
//     updated_socket_recv_vec.push_back(socketfd);
//     recv_vec_cv.notify_one();
// }

// void oldSocketManager::enqueue_send_data(int socketfd, std::shared_ptr<std::vector<uint8_t>> data)
// {
//     auto it = get_socket_vec(socketfd);
//     if (it == nullptr)
//     {
//         return;
//     }
//     else
//     {
//         it->enqueue_send_data(data);
//     }
//     std::unique_lock<std::mutex> lock(mutex_send_vec);
//     // updated_socket_send_set.insert(socketfd);
//     updated_socket_send_vec.push_back(socketfd);
//     send_vec_cv.notify_one();
// }

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

// std::shared_ptr<std::vector<uint8_t>> oldSocketManager::dequeue_recv_data(int socketfd)
// {
// auto it = get_socket_vec(socketfd);
// if (it == nullptr)
// {
//     return nullptr;
// }
// else
// {
//     return it->dequeue_recv_data();
// }

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
// }
// std::shared_ptr<std::vector<uint8_t>> oldSocketManager::dequeue_send_data(int socketfd)
// {
//     auto it = get_socket_vec(socketfd);
//     if (it == nullptr)
//     {
//         return nullptr;
//     }
//     else
//     {
//         return it->dequeue_send_data();
//     }
// }

/* std::shared_ptr<socketInfo> oldSocketManager::get_socket_vec(int socketfd)
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
} */

// std::shared_ptr<std::unordered_set<int>> socketManager::get_updated_socket_recv_vec()
// std::shared_ptr<std::vector<int>> oldSocketManager::get_updated_socket_recv_vec()
// {
//     std::unique_lock<std::mutex> lock(mutex_recv_vec);
//     // while(updated_socket_recv_set.empty())
//     while (updated_socket_recv_vec.empty())
//     {
//         recv_vec_cv.wait(lock);
//     }
//     // return std::make_shared<std::unordered_set<int>>(std::move(updated_socket_recv_set));
//     return std::make_shared<std::vector<int>>(std::move(updated_socket_recv_vec));
// }

// // std::shared_ptr<std::unordered_set<int>> socketManager::get_updated_socket_send_vec()
// std::shared_ptr<std::vector<int>> oldSocketManager::get_updated_socket_send_vec()
// {
//     std::unique_lock<std::mutex> lock(mutex_send_vec);
//     // while(updated_socket_send_set.empty())
//     while (updated_socket_send_vec.empty())
//     {
//         send_vec_cv.wait(lock);
//     }
//     // return std::make_shared<std::unordered_set<int>>(std::move(updated_socket_send_set));
//     return std::make_shared<std::vector<int>>(std::move(updated_socket_send_vec));
// }

// // bool socketManager::add_socket_vec(const std::string &username, int socket)
// /* bool oldSocketManager::add_socket_vec(const std::string &userid, const std::string &username, int socketfd)
// {
//     {
//         if (is_username_exist(username))
//         {
//             LOG_INFO("%s:%s:%d // 该用户已登录，不可再登录", __FILE__, __FUNCTION__, __LINE__);
//             return false;
//         }
//         else
//         {
//             std::lock_guard<std::mutex> lock(mutex_socket_map);
//             socket_map[socketfd] = std::make_shared<socketInfo>(socketfd, username);
//         }
//         // socket_vecs[socket] = std::make_shared<socketVector>(socket, username);
//     }
//     {
//         std::lock_guard<std::mutex> lock(mutex_interact_time_set);
//         interaction_time_socketvec_set.insert(socket_map[socketfd]);
//     }

//     // interaction_time_socketvec_set.insert(socket_vecs[socket]);
//     // pq.push(socket_vecs[socket]);
//     // username_socketfd[username] = socket;
//     std::shared_ptr<std::vector<uint8_t>> temp;
//     {
//         std::lock_guard<std::mutex> lock(mutex_offline_user_data);
//         temp = std::make_shared<std::vector<uint8_t>>(std::move(*sendto_offline_user_data[username]));
//         sendto_offline_user_data.erase(username);
//     }
//     enqueue_send_data(socketfd, temp);
//  */
// // auto temp = sendto_offline_user_data[username]
// // enqueue_send_data(socket,sendto_offline_user_data[username]);
// // sendto_offline_user_data.erase(username);

// // int n = sendto_offline_user_data[username].size_approx();
// // if( 0 != n)
// // {
// //     std::shared_ptr<std::vector<uint8_t>> temp = std::make_shared<std::vector<uint8_t>>();
// //     if(sendto_offline_user_data[username].try_dequeue_bulk(temp->end(),n))
// //     {
// //         this->enqueue_send_data(socket,temp);
// //     }
// // }
// // todo
// // int n =sendto_offline_user_data[socket].size_approx();
// // std::shared_ptr<std::vector<uint8_t>> temp = std::make_shared<std::vector<uint8_t>>();
// // if(n!=0)
// // {
// //     if(sendto_offline_user_data[socket].try_dequeue_bulk(temp->end(),n))
// //     {
// //         this->enqueue_send_data(socket,temp);
// //     }
// //     else
// //     {
// //         //失败了
// //         return;
// //     }
// // }
// // }

// void oldSocketManager::enqueue_offline_data(const std::string &userid, std::shared_ptr<std::vector<uint8_t>> data)
// {
//     // sendto_offline_user_data[username].enqueue_bulk(data->data(),data->size());
//     std::lock_guard<std::mutex> lock(mutex_offline_user_data);
//     sendto_offline_user_data[userid]->insert(sendto_offline_user_data[userid]->end(), data->begin(), data->end());
// }

// // bool oldSocketManager::delete_socket_vec(int socket)
// // {
// //     auto it = get_socket_vec(socket);
// //     if (it == nullptr)
// //     {
// //         LOG_WARING("%s:%s:%d // 未找到套接字对应的socketVector", __FILE__, __FUNCTION__, __LINE__);
// //         return false;
// //     }
// //     else
// //     {
// //         if (it->is_recv_data_empty() && it->is_send_data_empty())
// //         {
// //             std::lock_guard<std::mutex> lock(mutex_socket_map);
// //             socket_map.erase(socket);
// //             return true;
// //         }
// //         else
// //         {
// //             // todo 等待socketVector清空后再删除，如何实现？
// //             LOG_WARING("%s:%s:%d // 套接字对应的socketVector未清空", __FILE__, __FUNCTION__, __LINE__);
// //             return false;
// //         }
// //     }
// // }

// // void socketManager::init()
// // {

// // }

// /* void oldSocketManager::update_socket_interaction_time(int socketfd)
// {
//     auto vec = get_socket_vec(socketfd);
//     if (vec == nullptr)
//     {
//         LOG_ERROR("%s:%s:%d // 更新套接字的latesttime时找不到套接字的vec", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     }
//     else
//     {
//         // if(interaction_time_socketvec_set.find(it) == interaction_time_socketvec_set.end())
//         auto it = interaction_time_socketvec_set.find(vec);
//         if (it == interaction_time_socketvec_set.end())
//         {
//             LOG_ERROR("%s:%s:%d // 更新套接字的latesttime时找不到套接字的vec", __FILE__, __FUNCTION__, __LINE__);
//             return;
//         }
//         else
//         {
//             std::lock_guard<std::mutex> lock(mutex_interact_time_set);
//             interaction_time_socketvec_set.erase(it);
//             (*it)->update_last_interaction_time();
//             interaction_time_socketvec_set.insert(vec);
//         }
//     }
// } */

// int oldSocketManager::get_tobesend_heartbeat_socketfd()
// {
//     while (true)
//     {
//         {
//             // 获取锁并检查集合是否为空
//             std::lock_guard<std::mutex> lock(mutex_interact_time_set);

//             if (!interaction_time_socketvec_set.empty())
//             {
//                 // 如果集合不为空，退出循环
//                 break;
//             }
//         } // 锁在这里被释放

//         // 集合为空，释放锁并休眠一段时间
//         std::this_thread::sleep_for(std::chrono::seconds(TIME_OUT));
//     }

//     // 再次获取锁以确保线程安全地访问集合
//     std::lock_guard<std::mutex> lock(mutex_interact_time_set);

//     // 检查第一个元素是否过期
//     if (!interaction_time_socketvec_set.empty() && (*interaction_time_socketvec_set.begin())->is_timeout())
//     {
//         return (*interaction_time_socketvec_set.begin())->get_fd();
//     }
//     else
//     {
//         return -1; // 如果没有过期或集合为空（理论上这里不会为空）
//     }

//     // std::lock_guard<std::mutex> lock(mutex_interact_time_set);

//     // if(interaction_time_socketvec_set.empty())
//     // {
//     //     LOG_WARING("%s:%s:%d // 套接字set中没有套接字", __FILE__, __FUNCTION__, __LINE__);
//     //     return -1;
//     // }
//     // else
//     // {
//     //     if((*interaction_time_socketvec_set.begin())->is_timeout())
//     //     {
//     //         return (*interaction_time_socketvec_set.begin())->get_fd();
//     //     }
//     //     else
//     //     {
//     //         return -2;
//     //     }
//     // }
// }

// /* const std::string &oldSocketManager::get_username(int socketfd)
// {
//     auto it = get_socket_vec(socketfd);
//     if (it == nullptr)
//     {
//         LOG_ERROR("%s:%s:%d // 未找到套接字对应的socketVector", __FILE__, __FUNCTION__, __LINE__);
//         return "";
//     }
//     else
//     {
//         return it->get_username();
//     }
// } */

// oldSocketManager &oldSocketManager::getInstance()
// {
//     static oldSocketManager instance;
//     return instance;
// }

// /* bool oldSocketManager::is_username_exist(const std::string &username)
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto it : socket_map)
//     {
//         if (it.second->get_username() == username)
//         {
//             return true;
//         }
//     }
//     return false;
// } */

// int oldSocketManager::get_socket_by_userid(const std::string &userid)
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto it : socket_map)
//     {
//         if (it.second->get_userid() == userid)
//         {
//             return it.first;
//         }
//     }
//     return USER_OFFLINE;
// }

// // void socketManager::set_socket_isblocking(int socket_fd, bool blocking)
// // {
// //     int flags = fcntl(socket_fd, F_GETFL, 0);
// //     if (flags == -1) {
// //         // std::cerr << "Error getting file status flags: " << strerror(errno) << std::endl;
// //         LOG_ERROR("%s:%s:%d // 错误获取文件状态标志: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
// //     }

// //     if (blocking) {
// //         // 设置为阻塞模式
// //         if (fcntl(socket_fd, F_SETFL, flags & ~O_NONBLOCK) == -1) {
// //             // std::cerr << "Error setting socket to blocking mode: " << strerror(errno) << std::endl;
// //             LOG_ERROR("%s:%s:%d // 错误设置套接字为阻塞模式: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
// //         }
// //     } else {
// //         // 设置为非阻塞模式
// //         if (fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) == -1) {
// //             // std::cerr << "Error setting socket to non-blocking mode: " << strerror(errno) << std::endl;
// //             LOG_ERROR("%s:%s:%d // 错误设置套接字为非阻塞模式: %s", __FILE__, __FUNCTION__, __LINE__, strerror(errno));
// //         }
// //     }
// // }

// void oldSocketManager::add_socket(int socketfd)
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     socket_map[socketfd] = std::make_shared<socketInfo>(socketfd);
//     // set_socket_isblocking(socketfd,false);
//     // return true;
// }

// // socketManager &socketManager::get_instance()
// // {
// //     static socketManager instance;
// //     return instance;
// // }

// void socketUserManager::add_socket(int socket)
// {
// }

// void socketUserManager::remove_socket(int socket)
// {

//     // return true;
// }

// bool socketUserManager::is_socket_exist(int socket)
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     return socket_map->find(socket) != socket_map->end();
// }

// // void socketManager::push_data_to_socket(int socket, socket_data_type type, std::shared_ptr<std::vector<uint8_t>> data)
// // {
// //     if (is_socket_exist(socket) == true)
// //     {
// //         switch (type)
// //         {
// //         case socket_data_type::RECV:
// //             socket_map[socket]->enqueue_recv_data(data);
// //             promise_socket_map[socket]->set_value();
// //             break;
// //         case socket_data_type::SEND:
// //             socket_map[socket]->enqueue_send_data(data);
// //             break;
// //         default:
// //             LOG_ERROR("%s:%s:%d // unknown data type", __FILE__, __FUNCTION__, __LINE__);
// //             break;
// //         }
// //     }
// //     else
// //     {
// //         LOG_WARING("%s:%s:%d // socket not exist", __FILE__, __FUNCTION__, __LINE__);
// //     }
// // }

// // std::shared_ptr<std::vector<uint8_t>> socketManager::pop_data_from_socket(int socket, socket_data_type type)
// // {
// //     if (socket_map.find(socket) != socket_map.end())
// //     {
// //         switch (type)
// //         {
// //         case socket_data_type::RECV:
// //             return socket_map[socket]->dequeue_recv_data();
// //             break;
// //         case socket_data_type::SEND:
// //             return socket_map[socket]->dequeue_send_data();
// //         default:
// //             return nullptr;
// //         }
// //     }
// //     else
// //     {
// //         LOG_WARING("%s:%s:%d // 找不到套接字", __FILE__, __FUNCTION__, __LINE__);
// //         return nullptr;
// //     }
// // }

// void socketUserManager::push_ready_socket_single(int socket)
// {
//     // 加入ready_socket
//     {
//         std::lock_guard<std::mutex> lock(mutex_ready_socket_vec);
//         ready_socket_vec->push_back(socket);
//         cond_ready_socket_vec.notify_all();
//     }

//     // 更新互动时间
//     if (is_socket_exist(socket) == true)
//     {
//         std::lock_guard<std::mutex> lock(mutex_interact_time_set);
//         if (interaction_time_socketdata_set.find(socket_map->at(socket)) != interaction_time_socketdata_set.end())
//         {
//             interaction_time_socketdata_set.erase(socket_map->at(socket));
//             socket_map->at(socket)->update_interaction_time();
//             interaction_time_socketdata_set.insert(socket_map->at(socket));
//         }
//         else
//         {
//             LOG_WARING("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//             return;
//         }
//     }
//     else
//     {
//         LOG_WARING("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     }
//     /* if (socket_map.find(socket) != socket_map.end())
//     {
//         std::lock_guard<std::mutex> lock(mutex_interact_time_set);
//         if (interaction_time_socketvec_set.find(socket_map[socket]) != interaction_time_socketvec_set.end())
//         {
//             interaction_time_socketvec_set.erase(socket_map[socket]);
//             socket_map[socket]->update_last_interaction_time();
//             interaction_time_socketvec_set.insert(socket_map[socket]);
//         }
//         else
//         {
//             LOG_ERROR("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//             return;
//         }
//     }
//     else
//     {
//         LOG_WARING("%s:%s:%d // 没找到socket", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     */
//     //}
// }

// void socketUserManager::push_ready_socket_multiple(const std::shared_ptr<std::vector<int>> vec)
// {
//     {
//         std::lock_guard<std::mutex> lock(mutex_ready_socket_vec);
//         ready_socket_vec->insert(ready_socket_vec->end(), vec->begin(), vec->end());
//         cond_ready_socket_vec.notify_one();
//     }
//     {
//         // std::lock_guard<std::mutex> lock(mutex_socket_map);
//         for (auto i : *vec)
//         {
//             // if (socket_map.find(i) == socket_map.end())
//             // {
//             //     LOG_ERROR("%s:%s:%d // 没找到socket", __FILE__, __FUNCTION__, __LINE__);
//             //     vec->erase(std::remove(vec->begin(), vec->end(), i), vec->end());
//             // }
//             if (is_socket_exist(i) == false)
//             {
//                 vec->erase(std::remove(vec->begin(), vec->end(), i), vec->end());
//             }
//         }
//     }
//     {
//         std::lock_guard<std::mutex> lock(mutex_interact_time_set);
//         for (auto i : *vec)
//         {
//             // if (interaction_time_socketvec_set.find(socket_map[i]) != interaction_time_socketvec_set.end())
//             if (interaction_time_socketdata_set.find(socket_map->at(i)) != interaction_time_socketdata_set.end())
//             {
//                 // interaction_time_socketvec_set.erase(socket_map[i]);
//                 // socket_map[i]->update_last_interaction_time();
//                 // interaction_time_socketvec_set.insert(socket_map[i]);
//                 interaction_time_socketdata_set.erase(socket_map->at(i));
//                 socket_map->at(i)->update_interaction_time();
//                 interaction_time_socketdata_set.insert(socket_map->at(i));
//             }
//             else
//             {
//                 LOG_WARING("%s:%s:%d // 没找到套接字", __FILE__, __FUNCTION__, __LINE__);
//                 continue;
//             }
//         }
//     }
// }

// int socketUserManager::pop_ready_socket_single()
// {
//     std::unique_lock<std::mutex> lock(mutex_ready_socket_vec);
//     cond_ready_socket_vec.wait(lock, [this]
//                                { return !ready_socket_vec->empty(); });
//     int temp = ready_socket_vec->back();
//     ready_socket_vec->pop_back();
//     return temp;
// }

// std::shared_ptr<std::vector<int>> socketUserManager::pop_ready_socket_multiple()
// {
//     std::unique_lock<std::mutex> lock(mutex_ready_socket_vec);
//     cond_ready_socket_vec.wait(lock, [this]
//                                { return !ready_socket_vec->empty(); });
//     std::shared_ptr<std::vector<int>> temp = std::make_shared<std::vector<int>>();
//     temp->swap(*ready_socket_vec);
//     // ready_socket_vec->clear();
//     return temp;
// }

// socketUserManager::socketUserManager()
// {
//     init();
// }

// socketUserManager::~socketUserManager()
// {
//     deleter();
// }

// void socketUserManager::push_recv_data_queue(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     // auto recv = std::make_unique<socketData>(socket, std::nullopt, std::move(data));
//     // std::lock_guard<std::mutex> lock(mutex_recv_queue);
//     // recv_data_queue->push(std::move(recv));
//     // cond_recv_queue.notify_one();
//     if (is_socket_exist(socket) == true)
//     {
//         socket_map->at(socket)->push_recv_data(std::move(data));
//         std::lock_guard<std::mutex> lock(mutex_recv_queue);
//         recv_data_queue->push(socket_map->at(socket));
//         cond_recv_queue.notify_one();
//     }
//     else
//     {
//         LOG_WARING("%s:%s:%d // 没找到socket", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     }
// }

// void socketUserManager::push_send_data_queue(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     // auto send = std::make_unique<socketData>(socket, std::move(data), std::nullopt);
//     // std::lock_guard<std::mutex> lock(mutex_send_queue);
//     // send_data_queue->push(std::move(send));
//     // cond_send_queue.notify_one();
//     if (is_socket_exist(socket) == true)
//     {
//         socket_map->at(socket)->push_send_data(std::move(data));
//         std::lock_guard<std::mutex> lock(mutex_send_queue);
//         send_data_queue->push(socket_map->at(socket));
//         cond_send_queue.notify_one();
//     }
//     else
//     {
//         LOG_WARING("%s:%s:%d // 没找到socket", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     }
// }

// // std::unique_ptr<socketData> socketManager::pop_recv_socketdata_queue()
// std::shared_ptr<socketUserInfo> socketUserManager::pop_recv_socketdata_queue()
// {
//     std::unique_lock<std::mutex> lock(mutex_recv_queue);
//     cond_recv_queue.wait(lock, [this]
//                          { return !recv_data_queue->empty(); });
//     // auto data = std::move(recv_data_queue->front());
//     // recv_data_queue->pop();
//     // return std::move(data);
//     auto data = recv_data_queue->front();
//     recv_data_queue->pop();
//     return std::move(data);
// }

// // std::unique_ptr<socketData> socketManager::pop_send_socketdata_queue()
// std::shared_ptr<socketUserInfo> socketUserManager::pop_send_socketdata_queue()
// {
//     std::unique_lock<std::mutex> lock(mutex_send_queue);
//     cond_send_queue.wait(lock, [this]
//                          { return !send_data_queue->empty(); });
//     auto data = send_data_queue->front();
//     send_data_queue->pop();
//     return std::move(data);
// }

// void socketUserManager::push_recv_data_single_set(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     {
//         std::lock_guard<std::mutex> lock(mutex_socket_map);
//         socket_map->at(socket)->push_recv_data(std::move(data));
//     }
//     {
//         std::unique_lock<std::mutex> lock(mutex_recv_notempty_socket_set);
//         recv_notempty_socket_set->insert(socket);
//         cond_recv_notempty_socket_set.notify_one();
//     }
// }

// void socketUserManager::push_send_data_single_set(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     {
//         std::lock_guard<std::mutex> lock(mutex_socket_map);
//         socket_map->at(socket)->push_send_data(std::move(data));
//     }
//     {
//         std::unique_lock<std::mutex> lock(mutex_send_notempty_socket_set);
//         send_notempty_socket_set->insert(socket);
//         cond_send_notempty_socket_set.notify_one();
//     }
// }

// void socketUserManager::push_recv_data_multiple_set(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     {
//         std::lock_guard<std::mutex> lock(mutex_socket_map);
//         for (auto &[socket, socket_data] : *data)
//         {
//             socket_map->at(socket)->push_recv_data(std::move(socket_data));
//         }
//     }
//     {
//         std::unique_lock<std::mutex> lock(mutex_recv_notempty_socket_set);
//         for (auto &[socket, socket_data] : *data)
//         {
//             recv_notempty_socket_set->insert(socket);
//         }
//         cond_recv_notempty_socket_set.notify_one();
//     }
// }

// void socketUserManager::push_send_data_multiple_set(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     {
//         std::lock_guard<std::mutex> lock(mutex_socket_map);
//         for (auto &[socket, socket_data] : *data)
//         {
//             socket_map->at(socket)->push_send_data(std::move(socket_data));
//         }
//     }
//     {
//         std::lock_guard<std::mutex> lock(mutex_send_notempty_socket_set);
//         for (auto &[socket, data] : *data)
//         {
//             send_notempty_socket_set->insert(socket);
//         }
//         cond_send_notempty_socket_set.notify_one();
//     }
// }

// // std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> socketManager::pop_recv_data_map_map()
// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> socketUserManager::pop_recv_data_multiple_set()
// {
//     // std::unique_ptr<std::vector<uint8_t>> data;
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> recv_data_vec;
//     std::unique_lock<std::mutex> lock(mutex_recv_notempty_socket_set);
//     cond_recv_notempty_socket_set.wait(lock, [&]()
//                                        { return !recv_notempty_socket_set->empty(); });
//     for (auto &socket : *recv_notempty_socket_set)
//     {
//         recv_data_vec->emplace_back(socket_map->at(socket));
//     }
//     recv_notempty_socket_set->clear();
//     return std::move(recv_data_vec);
// }

// // std::unique_ptr<std::unordered_map<int,std::shared_ptr<std::vector<uint8_t>>>> socketManager::pop_send_data_map_map()
// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> socketUserManager::pop_send_data_multiple_set()
// {
//     // std::unique_ptr<std::vector<uint8_t>> data;
//     // std::unique_ptr<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>> data_map = std::make_unique<std::unordered_map<int,std::unique_ptr<std::vector<uint8_t>>>>();
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> send_data_vec = std::make_unique<std::vector<std::shared_ptr<socketUserInfo>>>();
//     std::unique_lock<std::mutex> lock(mutex_send_notempty_socket_set);
//     cond_send_notempty_socket_set.wait(lock, [this]
//                                        { return !send_notempty_socket_set->empty(); });
//     for (auto &socket : *send_notempty_socket_set)
//     {
//         // data_map->emplace(socket,std::move(socket_map->at(socket)->pop_send_data()));
//         send_data_vec->emplace_back(socket_map->at(socket));
//     }
//     send_notempty_socket_set->clear();
//     return std::move(send_data_vec);
// }

// std::shared_ptr<socketUserInfo> socketUserManager::pop_recv_data_single_set()
// {
//     std::unique_lock<std::mutex> lock(mutex_recv_notempty_socket_set);
//     cond_recv_notempty_socket_set.wait(lock, [&]()
//                                        { return !recv_notempty_socket_set->empty(); });
//     auto socket = *recv_notempty_socket_set->begin();
//     recv_notempty_socket_set->erase(socket);
//     return socket_map->at(socket);
// }

// std::shared_ptr<socketUserInfo> socketUserManager::pop_send_data_single_set()
// {
//     std::unique_lock<std::mutex> lock(mutex_send_notempty_socket_set);
//     cond_send_notempty_socket_set.wait(lock, [&]()
//                                        { return !send_notempty_socket_set->empty(); });
//     auto socket = *send_notempty_socket_set->begin();
//     send_notempty_socket_set->erase(socket);
//     return socket_map->at(socket);
// }

// void socketUserManager::push_recv_data_single_loop(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     if (is_socket_exist(socket) == true)
//     {
//         socket_map->at(socket)->push_recv_data(std::move(data));
//         return;
//     }
//     else
//     {
//         LOG_WARING("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     }
// }

// void socketUserManager::push_send_data_single_loop(int socket, std::unique_ptr<std::vector<uint8_t>> data)
// {
//     if (is_socket_exist(socket) == true)
//     {
//         socket_map->at(socket)->push_send_data(std::move(data));
//         return;
//     }
//     else
//     {
//         LOG_WARING("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//         return;
//     }
// }

// void socketUserManager::push_recv_data_multiple_loop(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     for (auto &[socket, data] : *data)
//     {
//         if (is_socket_exist(socket) == true)
//         {
//             socket_map->at(socket)->push_recv_data(std::move(data));
//         }
//         else
//         {
//             LOG_WARING("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//             continue;
//         }
//     }
// }

// void socketUserManager::push_send_data_multiple_loop(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> data)
// {
//     for (auto &[socket, data] : *data)
//     {
//         if (is_socket_exist(socket) == true)
//         {
//             socket_map->at(socket)->push_send_data(std::move(data));
//         }
//         else
//         {
//             LOG_WARING("%s:%s:%d // 找不到socket", __FILE__, __FUNCTION__, __LINE__);
//             continue;
//         }
//     }
// }

// std::shared_ptr<socketUserInfo> socketUserManager::pop_recv_data_single_loop()
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto &[socket, data] : *socket_map)
//     {
//         if (data->is_recv_data_empty() == true)
//         {
//             continue;
//         }
//         else
//         {
//             return data;
//         }
//     }
// }

// std::shared_ptr<socketUserInfo> socketUserManager::pop_send_data_single_loop()
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto &[socket, data] : *socket_map)
//     {
//         if (data->is_send_data_empty() == true)
//         {
//             continue;
//         }
//         else
//         {
//             return data;
//         }
//     }
// }

// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> socketUserManager::pop_recv_data_multiple_loop()
// {
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> recv_data_vec = std::make_unique<std::vector<std::shared_ptr<socketUserInfo>>>();
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto &[socket, data] : *socket_map)
//     {
//         if (data->is_recv_data_empty() == true)
//         {
//             continue;
//         }
//         else
//         {
//             recv_data_vec->emplace_back(data);
//         }
//     }
//     return std::move(recv_data_vec);
// }

// std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> socketUserManager::pop_send_data_multiple_loop()
// {
//     std::unique_ptr<std::vector<std::shared_ptr<socketUserInfo>>> send_data_vec = std::make_unique<std::vector<std::shared_ptr<socketUserInfo>>>();
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto &[socket, data] : *socket_map)
//     {
//         if (data->is_send_data_empty() == true)
//         {
//             continue;
//         }
//         else
//         {
//             send_data_vec->emplace_back(data);
//         }
//     }
//     return std::move(send_data_vec);
// }

// bool socketUserManager::is_user_exist_username(const std::string &username)
// {
// }

// bool socketUserManager::is_user_exist_userid(const std::string &username)
// {
// }

// bool IuserManager::is_userid_exist(const std::string &userid)
// {
//     return logined_userid_set->find(userid) != logined_userid_set->end();
// }

// void IuserManager::push_userid(const std::string &userid)
// {
//     std::lock_guard<std::mutex> lock(mutex_logined_userid_set);
//     logined_userid_set->insert(userid);
// }

// void IuserManager::delete_userid(const std::string &userid)
// {
//     std::lock_guard<std::mutex> lock(mutex_logined_userid_set);
//     logined_userid_set->erase(userid);
// }

// std::unique_ptr<std::vector<std::string>> IuserManager::get_all_logined_userid()
// {
//     std::lock_guard<std::mutex> lock(mutex_logined_userid_set);
//     return std::make_unique<std::vector<std::string>>(*logined_userid_set);
// }

// void socketUserManager::push_all_userid()
// {
//     std::lock_guard<std::mutex> lock(mutex_socket_map);
//     for (auto& [userid, userinfo] : *socket_map) {
//         IuserManager::push_userid(userinfo->get_user_id());
//     }

// }

// bool userManager::is_user_exist_at_socket(int socketFd)
// {
//     std::lock_guard<std::mutex> lock(mutex_fd_user_map);
//     return fd_user_map->contains(socketFd);
// }




void userManager::push_socket_haveRecvData_user_single(int socketFd, std::unique_ptr<std::vector<uint8_t>> recv_data)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    if (fd_to_user_map->contains(socketFd) == true)
    {
        (*fd_to_user_map)[socketFd]->push_socket_recvData(std::move(recv_data));
        std::lock_guard<std::mutex> lock1(mutex_socket_haveSendData_user_set);
        socket_haveRecvData_user_set->insert((*fd_to_user_map)[socketFd]);
        cv_socket_haveRecvData_user_set.notify_one();
        return;
    }
    else
    {
        return;
    }
}
void userManager::push_socket_haveRecvData_user_map(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> recv_data_map)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    for (auto &pair : *recv_data_map)
    {
        if (fd_to_user_map->contains(pair.first) == true)
        {
            (*fd_to_user_map)[pair.first]->push_socket_recvData(std::move(pair.second));
            std::lock_guard<std::mutex> lock1(mutex_socket_haveRecvData_socket_set);
            socket_haveRecvData_user_set->insert((*fd_to_user_map)[pair.first]);
            continue;
        }
        else
        {
            continue;
        }
    }
    cv_socket_haveRecvData_user_set.notify_one();
    return;
}
void userManager::push_socket_haveSendData_user_single(int socketFd, std::unique_ptr<std::vector<uint8_t>> send_data)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    if (fd_to_user_map->contains(socketFd) == true)
    {
        (*fd_to_user_map)[socketFd]->push_socket_sendData(std::move(send_data));
        std::unique_lock<std::mutex> lock1(mutex_socket_haveSendData_user_set);
        socket_haveSendData_user_set->insert((*fd_to_user_map)[socketFd]);
        cv_socket_haveRecvData_user_set.notify_one();
        return;
    }
    else
    {
        return;
    }
}
void userManager::push_socket_haveSendData_user_map(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> send_data_map)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    for (auto &pair : *send_data_map)
    {
        if (fd_to_user_map->contains(pair.first) == true)
        {
            (*fd_to_user_map)[pair.first]->push_socket_sendData(std::move(pair.second));
            std::unique_lock<std::mutex> lock1(mutex_socket_haveSendData_user_set);
            socket_haveSendData_user_set->insert((*fd_to_user_map)[pair.first]);
            continue;
        }
        else
        {
            continue;
        }
    }
    cv_socket_haveSendData_user_set.notify_one();
}

std::shared_ptr<User> userManager::pop_socket_haveRecvData_user_single()
{
    std::unique_lock<std::mutex> lock(mutex_socket_haveRecvData_user_set);
    cv_socket_haveRecvData_user_set.wait(lock, [this]
                                         { return !this->socket_haveRecvData_user_set->empty(); });
    auto first_user = *socket_haveRecvData_user_set->begin();
    socket_haveRecvData_user_set->erase(socket_haveRecvData_user_set->begin());
    return first_user;
}
std::unique_ptr<std::vector<std::shared_ptr<User>>> userManager::pop_socket_haveRecvData_user_vec()
{
    std::unique_lock<std::mutex> lock(mutex_socket_haveRecvData_user_set);
    cv_socket_haveRecvData_user_set.wait(lock, [this]
                                         { return !this->socket_haveRecvData_user_set->empty(); });
    auto res = std::make_unique<std::vector<std::shared_ptr<User>>>();
    for (auto &user : *socket_haveRecvData_user_set)
    {
        res->emplace_back(user);
    }
    socket_haveRecvData_user_set->clear();
    return std::move(res);
}

std::shared_ptr<User> userManager::pop_socket_haveSendData_user_single()
{
    std::unique_lock<std::mutex> lock(mutex_socket_haveSendData_user_set);
    cv_socket_haveSendData_user_set.wait(lock, [this]
                                         { return !this->socket_haveSendData_user_set->empty(); });
    auto first_user = *socket_haveSendData_user_set->begin();
    socket_haveSendData_user_set->erase(socket_haveSendData_user_set->begin());
    return first_user;
}
std::unique_ptr<std::vector<std::shared_ptr<User>>> userManager::pop_socket_haveSendData_user_vec()
{
    std::unique_lock<std::mutex> lock(mutex_socket_haveSendData_user_set);
    cv_socket_haveSendData_user_set.wait(lock, [this]
                                         { return !this->socket_haveSendData_user_set->empty(); });
    auto res = std::make_unique<std::vector<std::shared_ptr<User>>>();
    for (auto &user : *socket_haveSendData_user_set)
    {
        res->emplace_back(user);
    }
    socket_haveSendData_user_set->clear();
    return std::move(res);
}

void userManager::push_socket_haveRecvData_socket_single(int socketFd, std::unique_ptr<std::vector<uint8_t>> data)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    if (fd_to_user_map->contains(socketFd) == true)
    {
        (*fd_to_user_map)[socketFd]->push_socket_recvData(std::move(data));
        std::lock_guard<std::mutex> lock1(mutex_socket_haveRecvData_socket_set);
        socket_haveRecvData_socket_set->insert(socketFd);
        cv_socket_haveRecvData_socket_set.notify_one();
        return;
    }
}
void userManager::push_socket_haveRecvData_socket_map(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> recv_data_map)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    for (auto &pair : *recv_data_map)
    {
        if (fd_to_user_map->contains(pair.first) == true)
        {
            (*fd_to_user_map)[pair.first]->push_socket_recvData(std::move(pair.second));
            std::lock_guard<std::mutex> lock1(mutex_socket_haveRecvData_socket_set);
            socket_haveRecvData_socket_set->insert(pair.first);
            continue;
        }
        else
        {
            continue;
        }
    }
    cv_socket_haveRecvData_socket_set.notify_one();
    return;
}
void userManager::push_socket_haveSendData_socket_single(int socketFd, std::unique_ptr<std::vector<uint8_t>> send_data)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    if (fd_to_user_map->contains(socketFd) == true)
    {
        (*fd_to_user_map)[socketFd]->push_socket_sendData(std::move(send_data));
        std::lock_guard<std::mutex> lock1(mutex_socket_haveSendData_socket_set);
        socket_haveSendData_socket_set->insert(socketFd);
        cv_socket_haveSendData_socket_set.notify_one();
        return;
    }
    else
    {
        return;
    }
}
void userManager::push_socket_haveSendData_socket_map(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> send_data_map)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    for (auto &pair : *send_data_map)
    {
        if (fd_to_user_map->contains(pair.first) == true)
        {
            (*fd_to_user_map)[pair.first]->push_socket_sendData(std::move(pair.second));
            std::lock_guard<std::mutex> lock1(mutex_socket_haveSendData_socket_set);
            socket_haveSendData_socket_set->insert(pair.first);
            continue;
        }
        else
        {
            continue;
        }
    }
    cv_socket_haveSendData_socket_set.notify_one();
}

std::shared_ptr<User> userManager::pop_socket_haveRecvData_socket_single()
{
    std::unique_lock<std::mutex> lock(mutex_socket_haveRecvData_socket_set);

    while (true)
    {
        // 等待直到有可用的数据接收套接字
        cv_socket_haveRecvData_socket_set.wait(lock, [this]
                                               { return !this->socket_haveRecvData_socket_set->empty(); });

        // 获取第一个套接字并移除它
        int s = *socket_haveRecvData_socket_set->begin();
        socket_haveRecvData_socket_set->erase(socket_haveRecvData_socket_set->begin());

        // 释放锁以查找用户
        lock.unlock();

        // 尝试获取与套接字关联的用户
        {
            std::unique_lock<std::mutex> lock1(mutex_fd_to_user_map);
            auto it = fd_to_user_map->find(s);
            if (it != fd_to_user_map->end())
            {
                return it->second; // 返回用户指针
            }
        }

        // 如果没找到用户，这里不需要显式地重新获取锁，
        // 因为下一次循环开始时会自动获取锁
    }
}
std::unique_ptr<std::vector<std::shared_ptr<User>>> userManager::pop_socket_haveRecvData_socket_vec()
{
    while (true)
    {
        // 等待直到有可用的数据接收套接字
        std::unique_lock<std::mutex> lock(mutex_socket_haveRecvData_socket_set);
        cv_socket_haveRecvData_socket_set.wait(lock, [this]
                                               { return !this->socket_haveRecvData_socket_set->empty(); });

        // 移动所有套接字到临时容器以释放锁
        std::vector<int> temp_sockets(socket_haveRecvData_socket_set->begin(), socket_haveRecvData_socket_set->end());
        socket_haveRecvData_socket_set->clear();
        lock.unlock();

        // 创建结果容器
        auto res = std::make_unique<std::vector<std::shared_ptr<User>>>();

        // 尝试获取与每个套接字关联的用户
        {
            std::unique_lock<std::mutex> lock1(mutex_fd_to_user_map);
            for (auto &s : temp_sockets)
            {
                auto it = fd_to_user_map->find(s);
                if (it != fd_to_user_map->end())
                {
                    res->emplace_back(it->second);
                }
                // 如果需要处理未找到用户的套接字，可以在这里添加逻辑
            }
        }

        // 如果找到了任何用户，则返回结果；否则继续等待新的套接字
        if (!res->empty())
        {
            return std::move(res);
        }
    }
}
std::shared_ptr<User> userManager::pop_socket_haveSendData_socket_single()
{
    while (true)
    {
        // 等待直到有可用的数据发送套接字
        std::unique_lock<std::mutex> lock(mutex_socket_haveSendData_socket_set);
        cv_socket_haveSendData_socket_set.wait(lock, [this]
                                               { return !this->socket_haveSendData_socket_set->empty(); });

        // 获取第一个套接字并移除它
        int s = *socket_haveSendData_socket_set->begin();
        socket_haveSendData_socket_set->erase(socket_haveSendData_socket_set->begin());
        lock.unlock();

        // 尝试获取与套接字关联的用户
        {
            std::unique_lock<std::mutex> lock1(mutex_fd_to_user_map);
            auto it = fd_to_user_map->find(s);
            if (it != fd_to_user_map->end())
            {
                return it->second; // 返回用户指针
            }
            // 如果没找到用户，则继续循环等待下一个套接字
        }
    }
}
std::unique_ptr<std::vector<std::shared_ptr<User>>> userManager::pop_socket_haveSendData_socket_vec()
{
    while (true)
    {
        // 等待直到有可用的数据发送套接字
        std::unique_lock<std::mutex> lock(mutex_socket_haveSendData_socket_set);
        cv_socket_haveSendData_socket_set.wait(lock, [this]
                                               { return !this->socket_haveSendData_socket_set->empty(); });

        // 移动所有套接字到临时容器以释放锁
        std::vector<int> temp_sockets(socket_haveSendData_socket_set->begin(), socket_haveSendData_socket_set->end());
        socket_haveSendData_socket_set->clear();
        lock.unlock();

        // 创建结果容器
        auto res = std::make_unique<std::vector<std::shared_ptr<User>>>();

        // 尝试获取与每个套接字关联的用户
        {
            std::unique_lock<std::mutex> lock1(mutex_fd_to_user_map);
            for (auto &s : temp_sockets)
            {
                auto it = fd_to_user_map->find(s);
                if (it != fd_to_user_map->end())
                {
                    res->emplace_back(it->second);
                }
                // 如果需要处理未找到用户的套接字，可以在这里添加逻辑
                else
                {
                    continue;
                }
            }
        }

        // 如果找到了任何用户，则返回结果；否则继续等待新的套接字
        if (!res->empty())
        {
            return std::move(res);
        }
    }
}

void userManager::push_socket_haveRecvData_loop_single(int socketFd, std::unique_ptr<std::vector<uint8_t>> data)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    if (fd_to_user_map->find(socketFd) != fd_to_user_map->end())
    {
        (*fd_to_user_map)[socketFd]->push_socket_recvData(std::move(data));
        // cv_fd_user_map.notify_one();
    }
    else
    {
        return;
    }
}
void userManager::push_socket_haveRecvData_loop_map(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> recv_data_map)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    for (auto &it : *recv_data_map)
    {
        if (fd_to_user_map->find(it.first) != fd_to_user_map->end())
        {
            (*fd_to_user_map)[it.first]->push_socket_recvData(std::move(it.second));
        }
        else
        {
            continue;
        }
    }
    // cv_fd_user_map.notify_one();
}
void userManager::push_socket_haveSendData_loop_single(int socketFd, std::unique_ptr<std::vector<uint8_t>> data)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    if (fd_to_user_map->find(socketFd) != fd_to_user_map->end())
    {
        (*fd_to_user_map)[socketFd]->push_socket_sendData(std::move(data));
        // cv_fd_user_map.notify_one();
    }
    else
    {
        return;
    }
}
void userManager::push_socket_haveSendData_loop_map(std::unique_ptr<std::unordered_map<int, std::unique_ptr<std::vector<uint8_t>>>> send_data_map)
{
    std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
    for (auto &it : *send_data_map)
    {
        if (fd_to_user_map->find(it.first) != fd_to_user_map->end())
        {
            (*fd_to_user_map)[it.first]->push_socket_sendData(std::move(it.second));
        }
        else
        {
            continue;
        }
    }
    // cv_fd_user_map.notify_one();
}
std::shared_ptr<User> userManager::pop_socket_haveRecvData_loop_single()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
        for (auto &pair : *fd_to_user_map)
        {
            if (!pair.second->is_socket_recvData_empty())
            {
                return pair.second;
            }
            else
            {
                continue;
            }
        }
    }
}
std::unique_ptr<std::vector<std::shared_ptr<User>>> userManager::pop_socket_haveRecvData_loop_vec()
{
    std::unique_ptr<std::vector<std::shared_ptr<User>>> res = std::make_unique<std::vector<std::shared_ptr<User>>>();

    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
        for (auto &pair : *fd_to_user_map)
        {
            if (!pair.second->is_socket_recvData_empty())
            {
                res->emplace_back(pair.second);
            }
            else
            {
                continue;
            }
        }
        if (!res->empty())
        {
            return std::move(res);
        }
        else
        {
            continue;
        }
    }
}
std::shared_ptr<User> userManager::pop_socket_haveSendData_loop_single()
{
    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
        for (auto &pair : *fd_to_user_map)
        {
            if (!pair.second->is_socket_sendData_empty())
            {
                return pair.second;
            }
            else
            {
                continue;
            }
        }
    }
}
std::unique_ptr<std::vector<std::shared_ptr<User>>> userManager::pop_socket_haveSendData_loop_vec()
{
    std::unique_ptr<std::vector<std::shared_ptr<User>>> res = std::make_unique<std::vector<std::shared_ptr<User>>>();

    while (true)
    {
        std::unique_lock<std::mutex> lock(mutex_fd_to_user_map);
        for (auto &pair : *fd_to_user_map)
        {
            if (!pair.second->is_socket_sendData_empty())
            {
                res->emplace_back(pair.second);
            }
            else
            {
                continue;
            }
        }
        if (!res->empty())
        {
            return std::move(res);
        }
        else
        {
            continue;
        }
    }
}