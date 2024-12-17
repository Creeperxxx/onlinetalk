#pragma once
#include "RedisPool.h"
#include <memory>
#include <vector>


// using RedisReplyPtr = std::unique_ptr<redisReply, void(*)(redisReply*)>;

class IredisMethods
{
    
public:
    // using RedisReplyPtr = std::unique_ptr<redisReply, decltype(freeReplyObject)>;
    // virtual std::unique_ptr<redisReply> execute_command(const std::string& rediscommand) = 0;
    // virtual std::unique_ptr<redisReply,decltype(freeReplyObject)> execute_command(const std::string& rediscommand) = 0;
    // virtual std::shared_ptr<redisReply> execute_command(const std::string& rediscommand) = 0;

    virtual std::string redis_get(const std::string& key);
    virtual bool redis_set(const std::string& key,const std::string& value);
    virtual bool redis_del(const std::string& key);
// private:
//     std::unique_ptr<redisPool> m_redispool;
};
/*
redis方法
1. 插入键值对。要求值可以为5种数据类型
2. 查询键值对。
3. 删除键值对。
4. 更新键值对。
5. 
*/
class redisMethodsV1 : public IredisMethods
{
public:
    // bool insert(const std::string &key, const std::string &value) override;
    // std::unique_ptr<redisReply> execute_command(const std::string& rediscommand) override;
    // std::unique_ptr<redisReply,decltype(freeReplyObject)> execute_command(const std::string& rediscommand) override;
    // std::shared_ptr<redisReply> execute_command(const std::string& rediscommand) override;

    std::string redis_get(const std::string& key);
    bool redis_set(const std::string& key,const std::string& value);
    bool redis_del(const std::string& key);
    std::string build_cache_key(const std::string& userid);
    std::string build_find_userid_key(const std::string& username);
    bool update_redis_cache(const std::string& username,const std::string& userid,const std::string& userinfo);
// private:
//     static std::unique_ptr<redisMethods> m_instance;
};