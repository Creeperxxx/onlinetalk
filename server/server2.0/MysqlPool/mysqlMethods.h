#pragma once
#include "mysqlpool.h"
#include <memory>
#include <variant>
// enum class statementType
// {
//     QUERY,
//     NOTQUERY
// };


// class ImysqlMethods
// {
// public:
//     // virtual void init() = 0;
//     // virtual std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql ) = 0;

//     // virtual std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql , std::vector<std::variant<int,std::string>>& params) = 0;

//     virtual std::variant<bool , std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>> execute_sql(statementType type ,const std::string& sql, std::vector<std::variant<int,std::string>>& params) = 0;

//     // virtual bool execute_notquery(const std::string& sql) = 0;
//     // virtual bool execute_notquery(const std::string& sql , std::vector<std::variant<int,std::string>>& params) = 0;

//     virtual std::shared_ptr<sql::Connection> getConnection() = 0;
// // private:
//     // std::unique_ptr<MySQLConnectionPool> m_mysqlpool;
// };

// class mysqlMethods//: public ImysqlMethods
// {
// public:
//     // void init() override;
//     std::shared_ptr<sql::Connection> getConnection();
//     // std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql) override;
//     // bool execute_notquery(const std::string& sql) override;
// // private:
// //     std::shared_ptr<MySQLConnectionPool> m_mysqlpool;
// };

class mysqlMethods //: public ImysqlMethods
{
public:
    // std::shared_ptr<sql::Connection> getConnection();

    std::variant<bool, std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>> execute_sql(statementType type,const std::string& sql, std::vector<std::variant<int,std::string>>& params);
};

