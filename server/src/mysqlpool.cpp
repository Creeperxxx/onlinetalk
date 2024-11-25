#include "mysqlpool.h"

MySQLConnectionPool* MySQLConnectionPool::m_instance = NULL;
std::mutex MySQLConnectionPool::instance_mutex;

MySQLConnectionPool::~MySQLConnectionPool()
{
    for (auto &conn : pool)
    {
        conn->close();
    }
}

std::shared_ptr<sql::Connection> MySQLConnectionPool::getConnection()
{
    std::unique_lock<std::mutex> lock(pool_mutex);
    while (available_connections.empty())
    {
        cv.wait(lock);
    }
    auto conn = available_connections.front();
    available_connections.pop();
    return conn;
}

void MySQLConnectionPool::releaseConnection(std::shared_ptr<sql::Connection> conn)
{
    std::lock_guard<std::mutex> lock(pool_mutex);
    available_connections.push(conn);
    cv.notify_one();
}

void MySQLConnectionPool::initializePool(   const std::string& url,
                                            const std::string& user, 
                                            const std::string& password,   
                                            const std::string& database, 
                                            size_t pool_size
                                        )
{
    m_instance = NULL;
    m_url = url;
    m_user = user;
    m_password = password;
    m_database = database;
    m_pool_size = pool_size;
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    for (size_t i = 0; i < pool_size; ++i)
    {
        std::shared_ptr<sql::Connection> conn(driver->connect(url, user, password));
        conn->setSchema(database);
        pool.push_back(conn);
        available_connections.push(conn);
    }
}

MySQLConnectionPool* MySQLConnectionPool::getInstance()
{
    if(NULL == m_instance)
    {
        std::lock_guard<std::mutex> lock(instance_mutex);
        if(NULL == m_instance)
        {
            m_instance = new MySQLConnectionPool();
        }
    }
    return m_instance;
}

void MySQLConnectionPool::destroyInstance()
{
    if(NULL!= m_instance)
    {
        delete m_instance;
    }
}