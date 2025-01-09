#include "mysqlMethods.h"

// void mysqlMethodsV1::init()
// {
//     m_mysqlpool = MySQLConnectionPool::getInstance();
//     m_mysqlpool->initialize_pool();
// }

// std::unique_ptr<sql::ResultSet> mysqlMethodsV1::execute_query(const std::string& sql)
// {
//     auto conn = MySQLConnectionPool::getInstance().getConnection();
//     if (conn)
//     {
//         std::unique_ptr<sql::Statement> stmt(conn->createStatement());
//         std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(sql));
//         return res;
//     }
//     else
//     {
//         LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
//         return nullptr;
//     }
// }

// bool mysqlMethodsV1::execute_notquery(const std::string& sql)
// {
//     auto conn = MySQLConnectionPool::getInstance().getConnection();
//     if (conn)
//     {
//         std::unique_ptr<sql::Statement> stmt(conn->createStatement());
//         bool flag = stmt->execute(sql);
//         return flag;
//     }
//     else
//     {
//         LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
//         return false;
//     }
// }

// std::shared_ptr<sql::Connection> MySQLConnectionPool::getConnection()
// {
//     return MySQLConnectionPool::getInstance().getConnection();
// }

// std::shared_ptr<sql::Connection> mysqlMethods::getConnection()
// {
//     return MySQLConnectionPool::getInstance().getConnection();
// }

// std::variant<bool , std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>> mysqlMethods::execute_sql(statementType type,const std::string& sql, std::vector<std::variant<int,std::string>>& params)
// std::variant<bool, std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>> execute_sql(statementType type,const std::string& sql, std::optional<std::vector<std::variant<int,std::string>>> params)
std::unique_ptr<mysqlRes> execute_sql(statementType type, const std::string &sql, std::optional<std::vector<std::any>> params = std::nullopt)
{
    try
    {
        // auto conn = MySQLConnectionPool::getInstance().getConnection();
        auto connraii = mysqlConnRAII();
        auto conn = connraii.get_connection();
        auto ret = std::make_unique<mysqlRes>();
        if (conn == nullptr)
        {
            LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
            ret->set_is_error(true);
            return std::move(ret);
        }
        if (!params.has_value())
        {
            std::unique_ptr<sql::Statement, decltype(&sql::Statement::close)> stmt(conn->createStatement(), &sql::Statement::close);
            switch (type)
            {
            case statementType::QUERY:
            {
                std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)> res(stmt->executeQuery(sql), &sql::ResultSet::close);
                ret->set_res(std::move(res));
                break;
            }
            case statementType::INSERT:
            {
                int affect_rows = stmt->executeUpdate(sql);
                ret->set_affect_rows(affect_rows);
                if (affect_rows > 0)
                {
                    std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)> res_last_id(stmt->executeQuery(MYSQL_GET_LAST_INSERT_ID), &sql::ResultSet::close);
                    if (res_last_id->next())
                    {
                        ret->set_generated_key(res_last_id->getUInt64(1));
                    }
                }
                break;
            }
            case statementType::OTHER:
            {
                int affect_rows = stmt->executeUpdate(sql);
                ret->set_affect_rows(affect_rows);
                break;
            }
                // else if(type == statementType::NOTQUERY)
                // {
                //     // return stmt->executeUpdate(sql) > 0;
                //     ret->set_affect_rows(stmt->executeUpdate(sql));
                //     return std::move(ret);
                // }
            }
            return std::move(ret);
        }

        std::unique_ptr<sql::PreparedStatement, decltype(&sql::PreparedStatement::close)> stmt(conn->prepareStatement(sql), &sql::PreparedStatement::close);
        int paramindex = 1;
        for (const auto &param : params.value())
        {
            if (param.type() == typeid(uint64_t))
            {
                stmt->setUInt64(paramindex, std::any_cast<uint64_t>(param));
            }
            else if (param.type() == typeid(int))
            {
                stmt->setInt(paramindex, std::any_cast<int>(param));
            }
            else if (param.type() == typeid(std::string))
            {
                stmt->setString(paramindex, std::any_cast<std::string>(param));
            }
            else if (param.type() == typeid(bool))
            {
                stmt->setBoolean(paramindex, std::any_cast<bool>(param));
            }
            paramindex++;

            // if (std::holds_alternative<int>(param))
            // {
            //     stmt->setInt(paramindex, std::get<int>(param));
            // }
            // else if (std::holds_alternative<std::string>(param))
            // {
            //     stmt->setString(paramindex, std::get<std::string>(param));
            // }
            // ++paramindex;
        }
        switch (type)
        {
        case statementType::QUERY:
        {
            std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)> res(stmt->executeQuery(sql), &sql::ResultSet::close);
            ret->set_res(std::move(res));
            break;
        }
        case statementType::INSERT:
        {
            int affect_rows = stmt->executeUpdate(sql);
            ret->set_affect_rows(affect_rows);
            if (affect_rows > 0)
            {
                std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)> res_last_id(stmt->executeQuery(MYSQL_GET_LAST_INSERT_ID), &sql::ResultSet::close);
                if (res_last_id->next())
                {
                    ret->set_generated_key(res_last_id->getUInt64(1));
                }
            }
            break;
        }
        case statementType::OTHER:
        {
            ret->set_affect_rows(stmt->executeUpdate(sql));
            break;
        }
        }
        return std::move(ret);

        // if (type == statementType::QUERY)
        // {
        // std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)> res(stmt->executeQuery(), &sql::ResultSet::close);
        // return res;
        // }
        // else if (type == statementType::NOTQUERY)
        // {
        // return stmt->executeUpdate() > 0;
        // stmt->executeUpdate();
        // stmt->select
        // }
    }
    catch (sql::SQLException &e)
    {
        LOG_ERROR("%s:%s:%d // sql发生错误：%s , sqlstate: %s , errorcode:%d", __FILE__, __FUNCTION__, __LINE__, e.what(), e.getSQLState().c_str(), e.getErrorCode());
        return nullptr;
    }
}