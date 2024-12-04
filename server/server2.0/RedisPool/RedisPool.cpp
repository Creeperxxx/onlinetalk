#include "Redispool.h"
const std::string address = "127.0.0.1";
const int port = 6379;
cosnt size_t pool_size = 10;

void redisPool::initialize_pool(const std::string &address, int port, size_t pool_size)
{
    m_address = address;
    m_port = port;
    m_pool_size = pool_size;
    std::shared_ptr<redisContext> conn;
    for (size_t i = 0; i < pool_size; ++i)
    {
        conn = std::shared_ptr<redisContext>(redisConnect(address.c_str(), port), redisFree);
        if (conn == nullptr || conn->err)
        {
            if (conn)
            {
                std::cerr << "Error: " << conn->errstr << std::endl;
                redisFree(conn);
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

std::shared_ptr<redisPool> redisPool::getInstance()
{
    if (nullptr == m_instance)
    {
        std::lock_guard<std::mutex> lock(instance_mutex);
        if (nullptr == m_instance)
        {
            m_instance = std::shared_ptr<redisPool>();
            m_instance->initialize_pool(address, port, pool_size);
        }
    }
    return m_instance;
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

void redisPool::destroy_pool()
{
    if(nullptr!= m_instance)
    {
        delete m_instance;
    }
}

