#pragma once
#include "RedisPool.h"
#include <memory>
#include <vector>
#include <string.h>
#include <optional>
#include <nlohmann/json.hpp>

// using RedisReplyPtr = std::unique_ptr<redisReply, void(*)(redisReply*)>;

//我去，这里属于是强行面向接口编程了啊，感觉这个虚基类完全没有必要反而让项目更麻烦了我服了
// class IredisMethods
// {
    
// public:
//     // using RedisReplyPtr = std::unique_ptr<redisReply, decltype(freeReplyObject)>;
//     // virtual std::unique_ptr<redisReply> execute_command(const std::string& rediscommand) = 0;
//     // virtual std::unique_ptr<redisReply,decltype(freeReplyObject)> execute_command(const std::string& rediscommand) = 0;
//     // virtual std::shared_ptr<redisReply> execute_command(const std::string& rediscommand) = 0;

//     virtual std::string redis_get(const std::string& key);
//     virtual bool redis_set(const std::string& key,const std::string& value);
//     virtual bool redis_del(const std::string& key);
// // private:
// //     std::unique_ptr<redisPool> m_redispool;
// };

/*
redis方法
1. 插入键值对。要求值可以为5种数据类型
2. 查询键值对。
3. 删除键值对。
4. 更新键值对。
5. 
*/
class redisMethods //: public IredisMethods
{
// public:
    // bool insert(const std::string &key, const std::string &value) override;
    // std::unique_ptr<redisReply> execute_command(const std::string& rediscommand) override;
    // std::unique_ptr<redisReply,decltype(freeReplyObject)> execute_command(const std::string& rediscommand) override;
    // std::shared_ptr<redisReply> execute_command(const std::string& rediscommand) override;
    friend class database;
private:
    bool redis_del(const std::string& key);

    std::string redis_set_get(const std::string& key);
    bool redis_set_set(const std::string& key,const std::string& value, std::optional<redisSetMode> mode = std::nullopt, std::optional<int> expire = std::nullopt);
    
    // std::string build_cache_key(const std::string& userid);
    // std::string build_key_find_userinfo(const std::string& userid);
    // std::string build_find_userid_key(const std::string& username);
    // std::string build_key_find_userid(const std::string& username);
    // std::shared_ptr<std::vector<std::string>> redis_stream_xreadgroup(const std::string& stream_name,const std::string& group_name,const std::string& consumer_name, const std::optional<int> block_time = std::nullopt,const std::optional<int> count = std::nullopt);
    // bool init_stream_consumer_group(const std::string& stream_name,const std::string& groupname);
    // // std::string redis_stream_xadd(const std::string& stream,const std::vector<std::pair<std::string,std::string>>& fields);
    // std::string redis_stream_xadd(const std::string& stream,std::shared_ptr<std::vector<std::pair<std::string,std::string>>> fields);
    // std::string redis_stream_xadd(const std::string& stream,const std::string& msg);
    // bool redis_stream_xack(const std::string& stream,const std::string& groupname,const std::string& id);
    // void redis_sadd_push_logined_userid(const std::string& set_key,std::unique_ptr<std::vector<std::string>> userid_vec);
    // std::unique_ptr<std::vector<std::string>> redis_smembers_get_logined_userid(const std::string& set_key = REDIS_KEY_LOGINED_USERID);
    // void redis_srem_pop_logined_userid(const std::string& set_key = REDIS_KEY_LOGINED_USERID,const std::string& userid);
// private:
//     std::shared_ptr<std::vector<std::string>> get_string_from_redisreply(redisReply* reply);
    // void judge_redisreply_status(redisReply* reply);
    // bool update_redis_cache(const std::string& username,const std::string& userid,const std::string& userinfo);
// private:
//     static std::unique_ptr<redisMethods> m_instance;
};


class redisMethodsLoginedUserid 
{
    friend class database;
private:
    static std::unique_ptr<std::vector<std::string>> redis_smembers_get_logined_userid(const std::string& set_key = REDIS_KEY_LOGINED_USERID); 
    static void redis_sadd_push_logined_userid(const std::string& set_key,std::unique_ptr<std::vector<std::string>> userid_vec);
    static void redis_srem_pop_logined_userid(const std::string& set_key = REDIS_KEY_LOGINED_USERID,const std::string& userid);
    static bool redis_sismember_logined_userid(const std::string& set_key = REDIS_KEY_LOGINED_USERID,const std::string& userid);
};

class redisMethodsCor
{
    friend class database;
private:
    static std::shared_ptr<std::vector<std::string>> redis_stream_xreadgroup(const std::string& stream_name,const std::string& group_name,const std::string& consumer_name, const std::optional<int> block_time = std::nullopt,const std::optional<int> count = std::nullopt);
    static bool init_stream_consumer_group(const std::string& stream_name,const std::string& groupname);
    static std::string redis_stream_xadd(const std::string& stream,std::shared_ptr<std::vector<std::pair<std::string,std::string>>> fields);
    static std::string redis_stream_xadd(const std::string& stream,const std::string& msg);
    static bool redis_stream_xack(const std::string& stream,const std::string& groupname,const std::string& id);
    static std::shared_ptr<std::vector<std::string>> get_string_from_redisreply(redisReply* reply);
};

