#include "RedisMethods.h"

// std::unique_ptr<redisReply> redisMethodsV1::execute_command(const std::string& rediscommand)
// std::unique_ptr<redisReply,decltype(freeReplyObject)> redisMethodsV1::execute_command(const std::string& rediscommand)
// std::shared_ptr<redisReply> redisMethodsV1::execute_command(const std::string& rediscommand)
// {
//     auto conn = redisPool::getInstance().get_connection();
//     if (!conn)
//     {
//         LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
//         // return RedisReplyPtr(nullptr, freeReplyObject);
//         return nullptr;
//     }
//     else
//     {
//         redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "%s" ,rediscommand.c_str()));
//         if(!raw_reply)
//         {
//             LOG_ERROR("%s:%s:%d // 执行redis命令失败", __FILE__, __FUNCTION__, __LINE__);
//             return nullptr;
//         }
//         else
//         {
//             // return std::unique_ptr<redisReply,decltype(freeReplyObject)>(raw_reply, freeReplyObject);
//             return std::shared_ptr<redisReply>(raw_reply, freeReplyObject);
//         }
//     }
// }

std::string redisMethodsV1::redis_get(const std::string& key)
// std::string redisMethods::redis_get(const std::string& key)
{
    auto conn = redisPool::getInstance().get_connection();
    if (!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return "";
    }
    else
    {
        redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "GET %s", key.c_str()));
        if(!raw_reply)
        {
            LOG_ERROR("%s:%s:%d // 执行redis命令失败", __FILE__, __FUNCTION__, __LINE__);
            return "";
        }
        else
        {
            if(raw_reply->type == REDIS_REPLY_STRING)
            {
                LOG_INFO("%s:%s:%d // redis找到键值对", __FILE__, __FUNCTION__, __LINE__);
                return std::string(raw_reply->str, raw_reply->len);
            }
            else if(raw_reply->type == REDIS_REPLY_NIL)
            {
                LOG_WARING("%s:%s:%d // redis没有找到该键值对", __FILE__, __FUNCTION__, __LINE__);
                // return "";
            }
            else if(raw_reply->type == REDIS_REPLY_ERROR)
            {
                LOG_ERROR("%s:%s:%d // redis执行命令返回错误为%s", __FILE__, __FUNCTION__, __LINE__,raw_reply->str  );
                // return "";
            }
            return "";
        }
    }
}

bool redisMethodsV1::redis_set(const std::string& key,const std::string& value)
// bool redisMethods::redis_set(const std::string& key,const std::string& value)
{
    auto conn = redisPool::getInstance().get_connection();
    if (!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    else
    {
        redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "SET %s %s", key.c_str(), value.c_str()));
        if(!raw_reply)
        {
            LOG_ERROR("%s:%s:%d // 执行redis命令失败", __FILE__, __FUNCTION__, __LINE__);
            return false;
        }
        else
        {
            if(raw_reply->type == REDIS_REPLY_STATUS && std::string(raw_reply->str, raw_reply->len) == "OK")
            {
                LOG_INFO("%s:%s:%d // redis插入键值对:%s %s 成功", __FILE__, __FUNCTION__, __LINE__, key.c_str(), value.c_str());
                return true;
            }
            else 
            {
                LOG_ERROR("%s:%s:%d // redis插入键值对:%s %s 失败", __FILE__, __FUNCTION__, __LINE__, key.c_str(), value.c_str());
                return false;
            }
        }
    }
}

bool redisMethodsV1::redis_del(const std::string& key)
{
    auto conn = redisPool::getInstance().get_connection();
    if (!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    else
    {
        redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "DEL %s", key.c_str()));
        if(!raw_reply)
        {
            LOG_ERROR("%s:%s:%d // 执行redis命令失败", __FILE__, __FUNCTION__, __LINE__);
            return false;
        }
        else
        {
            if(raw_reply->type == REDIS_REPLY_INTEGER && raw_reply->integer == 1)
            {
                LOG_INFO("%s:%s:%d // redis删除键值对:%s 成功", __FILE__, __FUNCTION__, __LINE__, key.c_str());
                return true;
            }
            else
            {
                LOG_ERROR("%s:%s:%d // redis删除键值对:%s 失败", __FILE__, __FUNCTION__, __LINE__, key.c_str());
                return false;
            }
        }
    }
}

std::string redisMethodsV1::build_cache_key(const std::string& userid)
{
    return CACHE_PRIEFIX + userid;
}

std::string redisMethodsV1::build_find_userid_key(const std::string& username)
{
    return CACHE_FINDUSERID_PRIEFIX + username;
}

bool redisMethodsV1::update_redis_cache(const std::string& username,const std::string& userid,const std::string& userinfo)
{
    std::string redis_findid_key = build_find_userid_key(username);
    std::string redis_cache_key = build_cache_key(userid);
    if(!redis_set(redis_findid_key,userid))
    {
        LOG_ERROR("%s:%s:%d // redis更新name到id的映射失败", __FILE__, __FUNCTION__, __LINE__);
        return false;    
    }
    if(!redis_set(redis_cache_key,userinfo))
    {
        LOG_ERROR("%s:%s:%d // redis更新id到userinfo的映射失败", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    return true;
}