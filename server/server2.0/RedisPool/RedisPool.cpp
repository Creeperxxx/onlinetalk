#include "RedisPool.h"

const std::string REDIS_ADDRESS = "127.0.0.1";
const int REDIS_PORT = 6379;
const size_t REDIS_POOL_SIZE = 10;

// std::shared_ptr<redisPool> redisPool::m_instance = nullptr;
std::unique_ptr<redisPool> redisPool::m_instance = nullptr;
std::once_flag redisPool::init_once;

// void redisPool::initialize_pool(const std::string &address, int port, size_t pool_size)
void redisPool::initialize_pool()
{
    // m_address = address;
    // m_port = port;
    // m_pool_size = pool_size;
    // m_instance.reset(new redisPool());
    std::shared_ptr<redisContext> conn;
    for (size_t i = 0; i < REDIS_POOL_SIZE; ++i)
    {
        conn = std::shared_ptr<redisContext>(redisConnect(REDIS_ADDRESS.c_str(), REDIS_PORT), redisFree);
        if (conn == nullptr || conn->err)
        {
            if (conn)
            {
                std::cerr << "Error: " << conn->errstr << std::endl;
                redisFree(conn.get());
            }
            else
            {
                std::cerr << "Cannot allocate redis context" << std::endl;
            }
            continue;
        }
        pool.emplace_back(conn);
        available_connections.enqueue(conn);
    }
}

// std::shared_ptr<redisPool> redisPool::getInstance()
redisPool& redisPool::getInstance()
{
    // std::call_once(init_once, []()
                //    {
                    //    m_instance = std::make_shared<redisPool>();
                    //    m_instance->initialize_pool(address, port, pool_size);
                //    });
    // return m_instance;
    // std::call_once(init_once, &redisPool::initialize_pool);
    std::call_once(init_once , [](){
        m_instance.reset(new redisPool());
        m_instance->initialize_pool();
    });
    return *m_instance;
    // if (nullptr == m_instance)
    // {
    //     std::lock_guard<std::mutex> lock(instance_mutex);
    //     if (nullptr == m_instance)
    //     {
    //         m_instance = std::make_shared<redisPool>();
    //         m_instance->initialize_pool(address, port, pool_size);
    //     }
    // }
    // return m_instance;
}

std::shared_ptr<redisContext> redisPool::get_connection()
{
    std::shared_ptr<redisContext> conn;
    while(!available_connections.try_dequeue(conn))
    {
        std::this_thread::yield();
    }
    return conn;
}

void redisPool::release_connection(std::shared_ptr<redisContext> conn)
{
    available_connections.enqueue(conn);
}

// void redisPool::destroy_pool()
// {
//     if(nullptr!= m_instance)
//     {
//         delete m_instance;
//     }
// }

