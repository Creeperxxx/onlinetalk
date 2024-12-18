#pragma once
#include <hiredis/hiredis.h>
#include <memory>
#include <queue>
#include <mutex>
#include <vector>
#include <string>
#include "../requirement/moodycamel/concurrentqueue.h"
#include <cstddef>
#include "../logSystem/log.h"
#include <iostream>
#include "RedisMethods.h"
#include "../dataBase/dbConstants.h"

class redisPool
{
    friend class redisConnRAII;
    // friend class IredisMethods;
    // friend class redisMethods;
public:
    // static std::shared_ptr<redisPool> getInstance();
    // static redisPool& getInstance();
    // std::shared_ptr<redisContext> get_connection();
    // void release_connection(std::shared_ptr<redisContext> conn);
    ~redisPool(){}
    // void initialize_pool();
    // void destroy_pool();
    // void redisfree();
private:
    static redisPool& getInstance();
    void initialize_pool();
    std::shared_ptr<redisContext> get_connection();
    void release_connection(std::shared_ptr<redisContext> conn);
    // void initialize_pool(const std::string& address, int port ,size_t pool_size);
    redisPool(){initialize_pool();}
    redisPool(const redisPool&) = delete;
    redisPool& operator=(const redisPool&) = delete;
    // static std::shared_ptr<redisPool> m_instance;
    // static std::unique_ptr<redisPool> m_instance;
    // static std::mutex instance_mutex;
    // static std::once_flag init_once;
    // std::mutex conn_mutex;
    moodycamel::ConcurrentQueue<std::shared_ptr<redisContext>> available_connections;
    std::vector<std::shared_ptr<redisContext>> pool;
    std::mutex pool_mutex;
    // size_t m_pool_size;
    // std::string m_address;
    // int m_port;
};


//RAII自动管理资源获取和释放
class redisConnRAII
{
private:
    std::shared_ptr<redisContext> conn;
    redisPool& redis_pool;
public:
    redisConnRAII():redis_pool(redisPool::getInstance()){conn = redis_pool.get_connection();}
    ~redisConnRAII(){redis_pool.release_connection(conn);}
    std::shared_ptr<redisContext> get_connection(){return conn;}
};