#pragma once
#include "mysqlpool.h"
#include <memory>
#include <variant>
#include <optional>
#include <any>
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

class mysqlRes
{
public:
    void set_res(std::unique_ptr<sql::ResultSet,decltype(&sql::ResultSet::close)> res){m_res = std::move(res);}
    void set_affect_rows(int rows){affect_rows = rows;}
    void set_generated_key(uint64_t key){generated_key = key;}
    bool set_is_error(bool is_error){this->is_error = is_error;}
    std::unique_ptr<sql::ResultSet,decltype(&sql::ResultSet::close)> get_res(){return std::move(m_res);}
    int get_affect_rows(){return affect_rows;}
    uint64_t get_generated_key(){return generated_key;}
    bool get_is_error(){return is_error;}
private:
    std::unique_ptr<sql::ResultSet,decltype(&sql::ResultSet::close)> m_res;
    int affect_rows;
    uint64_t generated_key;
    bool is_error;
};

class mysqlMethods //: public ImysqlMethods
{
// public:
    // std::shared_ptr<sql::Connection> getConnection();

    // std::variant<bool, std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>> execute_sql(statementType type,const std::string& sql, std::vector<std::variant<int,std::string>>& params);
    // std::variant<bool, std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>> execute_sql(statementType type,const std::string& sql, std::optional<std::vector<std::variant<int,std::string>>> params = std::nullopt);
    // std::unique_ptr<mysqlRes> execute_sql(statementType type,const std::string& sql, std::optional<std::vector<std::variant<int,std::string>>> params = std::nullopt);
    friend class database;
private:
    static std::unique_ptr<mysqlRes> execute_sql(statementType type,const std::string& sql, std::optional<std::vector<std::any>> params = std::nullopt);
};

