#pragma once
#include "../RedisPool/RedisMethods.h"
#include "../MysqlPool/mysqlMethods.h"
#include "../logSystem/log.h"
#include <nlohmann/json.hpp>
#include "dbConstants.h"

// class Idatabase
// {
// public:
//     Idatabase():m_mysqlMethods(std::make_unique<mysqlMethodsV1>()),
//                 m_redisMethods(std::make_unique<redisMethodsV1>())
//                 {}
//     // virtual std::string get_user_info(int userid);
//     virtual std::string get_user_info(const std::string& account , loginType type);
//     // virtual std::string get_user_info(const std::string& username);
//     // virtual void set_user_info(int userid,const std::string& username,const std::string& password , const std::string& email);
//     virtual void set_user_info(const std::string& userid,const std::string& username,const std::string& password, const std::string& email);
//     virtual void delete_user_info(int userid);   
// protected:
//     std::unique_ptr<ImysqlMethods> m_mysqlMethods;
//     std::unique_ptr<IredisMethods> m_redisMethods;
// };



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

// class databaseV2 : public Idatabase
class database
{
public:
    // static databaseV2& get_instance();
    // static database& get_instance();
    // ~databaseV2(){}
    // ~database(){}
    // std::string get_user_info_from_cacheordb(const std::string& account, loginType);
    // void set_user_info_from_cacheanddb(const std::string& userid,const std::string& username,const std::string& password, const std::string& email);
    // void set_offline_data_from_cacheanddb(const std::string &userid, const std::vector<uint8_t> &data);
    // void set_offline_data_from_cacheanddb(const std::string &key_userid, const time_t& score_timestamp , const std::vector<uint8_t> value_offlinemsg,std::optional<int> expire = std::nullopt); //是否考虑将data的类型改为string？
    // void set_msg_from_db(const std::string& sender_id,const std::string &sender_name,const std::string &receiver_id, const std::string &receiver_name, const std::string &msg,const std::string& type);
    static std::shared_ptr<std::vector<std::string>> get_alluserid_from_db();
    
    static std::shared_ptr<std::vector<std::string>> redis_stream_xreadgroup(const std::string& stream_name,const std::string& group_name,const std::string& consumer_name, const std::optional<int> block_time = std::nullopt,const std::optional<int> count = std::nullopt);
    // std::string redis_stream_xadd(const std::string& stream,const std::vector<std::pair<std::string,std::string>>& fields);
    static std::string redis_stream_xadd_fields(const std::string& stream,std::shared_ptr<std::vector<std::pair<std::string,std::string>>> fields);
    static std::string redis_stream_xadd_msg(const std::string& stream,const std::string& msg);
    static int redis_stream_xack_single(const std::string& stream,const std::string& groupname,const std::string& id);
    static int redis_stream_xack_batch(const std::string& stream,const std::string& groupname,std::shared_ptr<std::vector<std::string>> ids);
    static void init_stream_consumer_group(const std::string& stream_name,const std::string& groupname);
    static bool is_user_exist_in_db(const std::string& username,const std::string& email);
    static bool get_login_result(const std::string type,const std::string& account,const std::string& password);
    static uint64_t add_user_to_db(const std::string& username,const std::string& password,const std::string& email);

    static void add_logined_userid(const std::string& set_key = REDIS_KEY_LOGINED_USERID,std::unique_ptr<std::vector<std::string>> userid_vec);
    static bool is_userid_exist_in_redis(const std::string& set_key = REDIS_KEY_LOGINED_USERID ,const std::string& userid);
    static void delete_logined_userid_in_redis(const std::string& set_key = REDIS_KEY_LOGINED_USERID ,const std::string& userid);

    static std::string get_userid_by_name_passwd_in_db(const std::string& username,const std::string& passwd);
    // static std::string get_userinfo_by_userid_in_db(const std::string& userid);
    static bool is_login_success_by_userid_in_db(const std::string& userid,const std::string& passwd);  
    // void delete_user_info(int userid);
// private:
    // database():m_mysqlMethods(std::make_unique<mysqlMethods>()),m_redisMethods(std::make_unique<redisMethods>()){} // {
        // init_stream_consumer_group(REDIS_STREAM_STREAMNAME_SERVER,REDIS_STREAM_GROUPNAME_SERVER);
    // }
    // std::unique_ptr<ImysqlMethods> m_mysqlMethods;
    // std::unique_ptr<mysqlMethods> m_mysqlMethods;
    // std::unique_ptr<redisMethods> m_redisMethods;
};
//我突然意识到，好像不是所有的数据都要加入到redis缓存中。新的问题产生了，所以哪些数据应该被加入到缓存中呢？