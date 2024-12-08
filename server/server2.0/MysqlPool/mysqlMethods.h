#pragma once
#include "mysqlpool.h"
#include <memory>

class ImysqlMethods
{
public:
    virtual void init() = 0;
    virtual std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql) = 0;
    virtual bool execute_notquery(const std::string& sql) = 0;
private:
    std::shared_ptr<MySQLConnectionPool> m_mysqlpool;
};

class mysqlMethodsV1: public ImysqlMethods
{
public:
    void init() override;
    std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql) override;
    bool execute_notquery(const std::string& sql) override;
private:
    std::shared_ptr<MySQLConnectionPool> m_mysqlpool;
};