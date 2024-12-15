#pragma once
#include "mysqlpool.h"
#include <memory>

extern const std::string MYSQL_TABLE;
extern const std::string USER_ID_FIELD;
extern const std::string USER_NAME_FIELD;
extern const std::string USER_PASSWD_FIELD;
extern const std::string USER_EMAIL_FIELD;

class ImysqlMethods
{
public:
    // virtual void init() = 0;
    virtual std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql) = 0;
    virtual bool execute_notquery(const std::string& sql) = 0;
private:
    std::unique_ptr<MySQLConnectionPool> m_mysqlpool;
};

class mysqlMethodsV1: public ImysqlMethods
{
public:
    // void init() override;
    std::shared_ptr<sql::Connection> getConnection();
    std::unique_ptr<sql::ResultSet> execute_query(const std::string& sql) override;
    bool execute_notquery(const std::string& sql) override;
// private:
//     std::shared_ptr<MySQLConnectionPool> m_mysqlpool;
};