#include "mysqlMethods.h"

// void mysqlMethodsV1::init()
// {
//     m_mysqlpool = MySQLConnectionPool::getInstance();
//     m_mysqlpool->initialize_pool();
// }

std::unique_ptr<sql::ResultSet> mysqlMethodsV1::execute_query(const std::string& sql)
{
    return nullptr;
}

bool mysqlMethodsV1::execute_notquery(const std::string& sql)
{       

    return true;
}