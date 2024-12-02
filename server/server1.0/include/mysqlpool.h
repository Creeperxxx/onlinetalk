#pragma once
#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>
//raii


class MySQLConnectionPool {
private:
    std::string m_url;
    std::string m_user;
    std::string m_password;
    std::string m_database;
    size_t m_pool_size;
    std::vector<std::shared_ptr<sql::Connection>> pool;
    std::queue<std::shared_ptr<sql::Connection>> available_connections;
    std::mutex pool_mutex;
    static std::mutex instance_mutex;
    std::condition_variable cv;
    static MySQLConnectionPool* m_instance;
    MySQLConnectionPool(const MySQLConnectionPool&) = delete;
    MySQLConnectionPool& operator=(const MySQLConnectionPool&) = delete;
    ~MySQLConnectionPool();
    MySQLConnectionPool(){}
public:
    static MySQLConnectionPool* getInstance();
    std::shared_ptr<sql::Connection> getConnection();
    void releaseConnection(std::shared_ptr<sql::Connection> conn);
    void initializePool(const std::string& url,
                        const std::string& user, 
                        const std::string& password,   
                        const std::string& database, 
                        size_t pool_size); 
    static void destroyInstance();
    std::shared_ptr<sql::ResultSet> executestatement(const std::string& sql);
};

