#include "mysqlMethods.h"
const std::string MYSQL_TABLE = "users";
const std::string USER_ID_FIELD = "id";
const std::string USER_NAME_FIELD = "username";
const std::string USER_PASSWD_FIELD = "password";
const std::string USER_EMAIL_FIELD = "email";

// void mysqlMethodsV1::init()
// {
//     m_mysqlpool = MySQLConnectionPool::getInstance();
//     m_mysqlpool->initialize_pool();
// }

std::unique_ptr<sql::ResultSet> mysqlMethodsV1::execute_query(const std::string& sql)
{
    auto conn = MySQLConnectionPool::getInstance().getConnection();
    if (conn)
    {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(sql));
        return res;
    }
    else
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return nullptr;
    }
}

bool mysqlMethodsV1::execute_notquery(const std::string& sql)
{       
    auto conn = MySQLConnectionPool::getInstance().getConnection();
    if (conn)
    {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        bool flag = stmt->execute(sql);
        return flag;
    }
    else
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return false; 
    }
}

std::shared_ptr<sql::Connection> MySQLConnectionPool::getConnection()
{
    return MySQLConnectionPool::getInstance().getConnection();
}