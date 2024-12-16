#include "mysqlpool.h"
const std::string MYSQL_ADDRESS = "127.0.0.1";
const size_t  MYSQL_POOL_SIZE = 10;
const std::string MYSQL_USER = "root";
const std::string MYSQL_PASSWORD = "";
const std::string MYSQL_DATABASE= "onlinechat";

std::unique_ptr<MySQLConnectionPool> MySQLConnectionPool::m_instance = NULL;
// std::mutex MySQLConnectionPool::instance_mutex;
std::once_flag MySQLConnectionPool::init_once;

// MySQLConnectionPool::~MySQLConnectionPool()
// {
//     for (auto &conn : pool)
//     {
//         conn->close();
//     }
// }

std::shared_ptr<sql::Connection> MySQLConnectionPool::getConnection()
{
    std::shared_ptr<sql::Connection> conn;
    while(!available_connections.try_dequeue(conn))
    {
        std::this_thread::yield();
    }
    return conn;
}

void MySQLConnectionPool::releaseConnection(std::shared_ptr<sql::Connection> conn)
{
    available_connections.enqueue(conn);
}
void MySQLConnectionPool::initialize_pool()
{
    // std::shared_ptr<sql::Connection> conn;
    // m_instance.reset(new MySQLConnectionPool());
    sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
    for(size_t i = 0;i <  MYSQL_POOL_SIZE;++i)
    {
        // auto conn = std::make_shared<sql::Connection>(*(driver->connect(MYSQL_ADDRESS, MYSQL_USER, MYSQL_PASSWORD)),[](sql::Connection *conn){
        auto conn = std::shared_ptr<sql::Connection>(driver->connect(MYSQL_ADDRESS, MYSQL_USER, MYSQL_PASSWORD),[](sql::Connection *conn){
            if(conn != nullptr)
            {
                conn->close();
            }
        });
        conn->setSchema(MYSQL_DATABASE);
        pool.push_back(conn); 
        available_connections.enqueue(conn);
    }
}
// void MySQLConnectionPool::initializePool(   const std::string& url,
//                                             const std::string& user, 
//                                             const std::string& password,   
//                                             const std::string& database, 
//                                             size_t pool_size
//                                         )
// {
//     m_url = url;
//     m_user = user;
//     m_password = password;
//     m_database = database;
//     m_pool_size = pool_size;
//     sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
//     for (size_t i = 0; i < pool_size; ++i)
//     {
//         std::shared_ptr<sql::Connection> conn(driver->connect(url, user, password));
//         conn->setSchema(database);
//         pool.push_back(conn);
//         available_connections.enqueue(conn);
//     }
// }

// std::unique_ptr<MySQLConnectionPool> MySQLConnectionPool::getInstance()
MySQLConnectionPool& MySQLConnectionPool::getInstance()
{
    // std::call_once(init_once, [](){
        // m_instance = std::shared_ptr<MySQLConnectionPool>( new MySQLConnectionPool());
        // m_instance = std::make_shared<MySQLConnectionPool>();

    // });
    // std::call_once(init_once,&MySQLConnectionPool::initialize_pool);
    std::call_once(init_once, [](){
        m_instance.reset(new MySQLConnectionPool());
        m_instance->initialize_pool();
    });
    return *m_instance;
}

// void MySQLConnectionPool::destroyInstance()
// {
//     if(NULL!= m_instance)
//     {
//         delete m_instance;
//     }
// }

// std::shared_ptr<sql::ResultSet> MySQLConnectionPool::executestatement(const std::string& sql)
// {
//     auto conn = getConnection();
//     std::unique_ptr<sql::Statement> stmt(conn->createStatement());
//     std::shared_ptr<sql::ResultSet> res(stmt->executeQuery(sql));
//     releaseConnection(conn);
//     return res;
// }
