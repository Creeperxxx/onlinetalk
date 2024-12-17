#pragma once
#include "../RedisPool/RedisMethods.h"
#include "../MysqlPool/mysqlMethods.h"
#include "../logSystem/log.h"
#include <nlohmann/json.hpp>
#include "dbConstants.h"

class Idatabase
{
public:
    Idatabase():m_mysqlMethods(std::make_unique<mysqlMethodsV1>()),
                m_redisMethods(std::make_unique<redisMethodsV1>())
                {}
    // virtual std::string get_user_info(int userid);
    virtual std::string get_user_info(const std::string& account , loginType type);
    // virtual std::string get_user_info(const std::string& username);
    // virtual void set_user_info(int userid,const std::string& username,const std::string& password , const std::string& email);
    virtual void set_user_info(const std::string& userid,const std::string& username,const std::string& password, const std::string& email);
    virtual void delete_user_info(int userid);   
protected:
    std::unique_ptr<ImysqlMethods> m_mysqlMethods;
    std::unique_ptr<IredisMethods> m_redisMethods;
};

// class databaseV1 : public Idatabase
// {
// public:
//     static databaseV1& getInstance();
//     ~databaseV1(){}
//     // std::string get_user_info(int userid) override;
//     std::string get_user_info(const std::string& userid , loginType) override;
//     // std::string get_user_info(const std::string& username) override;
//     // void set_user_info(int userid,const std::string& username,const std::string& password , const std::string& email) override;
//     void set_user_info(const std::string& userid,const std::string& username,const std::string& password, const std::string& email) override;
//     void delete_user_info(int userid) override;
// private:
//     databaseV1(): Idatabase(){}
// private:
//     static std::unique_ptr<databaseV1> m_instance;
//     static std::once_flag init_once;
// };

class databaseV2 : public Idatabase
{
public:
    static databaseV2& get_instance();
    ~databaseV2(){}
    std::string get_user_info(const std::string& account, loginType) override;
    void set_user_info(const std::string& userid,const std::string& username,const std::string& password, const std::string& email) override;
    void delete_user_info(int userid) override;
};