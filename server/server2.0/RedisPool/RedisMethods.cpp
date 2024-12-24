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

std::string redisMethods::redis_set_get(const std::string& key)
// std::string redisMethods::redis_get(const std::string& key)
{
    // auto conn = redisPool::getInstance().get_connection();
    auto connraii = redisConnRAII();
    auto conn = connraii.get_connection();
    if (!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return "";
    }
    else
    {
        // redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "GET %s", key.c_str()));
        std::unique_ptr<redisReply,void(*)(redisReply*)> raw_reply(static_cast<redisReply*>(redisCommand(conn.get(), "GET %s", key.c_str())),[](redisReply* reply){freeReplyObject(reply);});
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



bool redisMethods::redis_set_set(const std::string& key,const std::string& value ,std::optional<redisSetMode> mode,std::optional<int> expiretime)
// bool redisMethods::redis_set(const std::string& key,const std::string& value)
{
    // auto conn = redisPool::getInstance().get_connection();
    auto connraii = redisConnRAII();
    auto conn = connraii.get_connection();
    if (!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    else
    {
        // redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "SET %s %s", key.c_str(), value.c_str()));

        // std::string mode_str = "";
        // if(mode.has_value())
        // {
        //     if(mode.value() == redisSetMode::NX)
        //     {
        //         mode_str = "NX";
        //     }
        //     else
        //     {
        //         mode_str = "XX";
        //     }
        // }
        // std::string expiretime_mode = "";
        // if(expiretime.has_value())
        // {
        //     expiretime_mode = "EX";
        // }
        // else
        // {

        // }
        // std::unique_ptr<redisReply,void(*)(redisReply*)> raw_reply(static_cast<redisReply*>(redisCommand(conn.get(), "SET %s %s %s %d %s", key.c_str(), value.c_str(), expiretime ,)),[](redisReply* reply){freeReplyObject(reply);});

        std::string command_str = "SET " + key + " " + value;
        if(mode.has_value())
        {
            if(mode.value() == redisSetMode::NX)
            {
                command_str += " NX";
            }
            else
            {
                command_str += " XX";
            }
        }
        if(expiretime.has_value())
        {
            command_str += " EX " + std::to_string(expiretime.value());
        }
        std::unique_ptr<redisReply,void(*)(redisReply*)> raw_reply(static_cast<redisReply*>(redisCommand(conn.get(), command_str.c_str())),[](redisReply* reply){freeReplyObject(reply);});
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

bool redisMethods::redis_del(const std::string& key)
{
    // auto conn = redisPool::getInstance().get_connection();
    auto connraii = redisConnRAII();
    auto conn = connraii.get_connection();
    if (!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    else
    {
        // redisReply* raw_reply = static_cast<redisReply*>(redisCommand(conn.get(), "DEL %s", key.c_str()));
        std::unique_ptr<redisReply,void(*)(redisReply*)> raw_reply(static_cast<redisReply*>(redisCommand(conn.get(), "DEL %s", key.c_str())),[](redisReply* reply){freeReplyObject(reply);});
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

std::string redisMethods::build_key_find_userinfo(const std::string& userid)
{
    return REDIS_PRIEFIX_FIND_USERINFO + userid;
}

std::string redisMethods::build_key_find_userid(const std::string& username)
{
    return REDIS_PRIEFIX_FIND_USERID + username;
}

// bool redisMethods::update_redis_cache(const std::string& username,const std::string& userid,const std::string& userinfo)
// {
//     std::string redis_findid_key = build_find_userid_key(username);
//     std::string redis_cache_key = build_cache_key(userid);
//     if(!redis_set(redis_findid_key,userid))
//     {
//         LOG_ERROR("%s:%s:%d // redis更新name到id的映射失败", __FILE__, __FUNCTION__, __LINE__);
//         return false;    
//     }
//     if(!redis_set(redis_cache_key,userinfo))
//     {
//         LOG_ERROR("%s:%s:%d // redis更新id到userinfo的映射失败", __FILE__, __FUNCTION__, __LINE__);
//         return false;
//     }
//     return true;
// }

std::string redisMethods::consume_msg(const std::string &stream_name,const std::string& group_name,const std::string& consumer_name,const std::optional<int> block_time,const std::optional<int> count)
{
    redisConnRAII connraii;
    auto conn = connraii.get_connection();
    if(!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
    // std::string res_str;
    //json字符串,使用json库
    // nlohmann::json res_json;

    //这里阻塞的等待消息，是否需要优化？
    //默认阻塞1s，获取1条消息
    std::string command_str = "XREADGROUP GROUP " + group_name + " " + consumer_name + " COUNT " + std::to_string(count.value()) + " BLOCK " + std::to_string(block_time.value()) + " STREAMS " + stream_name + " >";
    // std::unique_ptr<redisReply, void(*)(redisReply*)> consume_reply(static_cast<redisReply*>(redisCommand(conn.get(), "XREADGROUP GROUP %s %s COUNT %d BLOCK %d STREAMS %s >", group_name.c_str(), consumer_name.c_str(),block_time.value() ,count.value(),stream_name.c_str())),[](redisReply* reply){freeReplyObject(reply);});
    std::unique_ptr<redisReply, void(*)(redisReply*)> consume_reply(static_cast<redisReply*>(redisCommand(conn.get(), command_str.c_str())),[](redisReply* reply){freeReplyObject(reply);});
    if(consume_reply == nullptr || consume_reply->type == REDIS_REPLY_ERROR )
    {
        LOG_ERROR("%s:%s:%d // 消费者：%s消费消息失败", __FILE__, __FUNCTION__, __LINE__,consumer_name.c_str());
    }
    else if(consume_reply->type == REDIS_REPLY_NIL)
    {
        LOG_WARING("%s:%s:%d // 消费者：%s没有消息", __FILE__, __FUNCTION__, __LINE__,consumer_name.c_str());
    }
    else if(consume_reply->type != REDIS_REPLY_ARRAY)
    {
        LOG_ERROR("%s:%s:%d // 消费者：%s消费消息失败", __FILE__, __FUNCTION__, __LINE__,consumer_name.c_str());
    }
    else if(consume_reply->type == REDIS_REPLY_ARRAY)
    {
        LOG_INFO("%s:%s:%d // 消费者：%s消费消息成功", __FILE__, __FUNCTION__, __LINE__,consumer_name.c_str());
        return get_string_from_redisreply(consume_reply.get());
        // std::string stream_name;
        // std::string message_id;
        // std::string field;
        // std::string value;
        //这里从结果集中取出消息，层次为：流名称->消息id->消息内容
        // for(int i = 0; i < consume_reply->elements; i++)
        // {
        //     stream_name = consume_reply->element[i]->element[0]->str;
        //     res_json[REDIS_JSON_FIELD_STREAMNAME] = stream_name;

        //     std::unique_ptr<redisReply, void(*)(redisReply*)> stream_reply(static_cast<redisReply*>(consume_reply->element[i]->element[1]),[](redisReply* reply){freeReplyObject(reply);});
        //     if(stream_reply->type != REDIS_REPLY_ARRAY || stream_reply->elements != 2)
        //     {
        //         LOG_ERROR("%s:%s:%d // 消费者：%s消费消息失败", __FILE__, __FUNCTION__, __LINE__,consumer_name.c_str());
        //     }
        //     else
        //     {
        //         for(int j = 0; j < stream_reply->elements; j++)
        //         {
        //             message_id = stream_reply->element[j]->element[0]->str;
        //             res_json[REDIS_JSON_FIELD_MESSAGEID] = message_id;
        //             std::unique_ptr<redisReply, void(*)(redisReply*)> message_reply(static_cast<redisReply*>(stream_reply->element[j]->element[1]),[](redisReply* reply){freeReplyObject(reply);});
        //             if(message_reply->type!= REDIS_REPLY_ARRAY || message_reply->elements!= 2)
        //             {
        //                 LOG_ERROR("%s:%s:%d // 消费者：%s消费消息失败", __FILE__, __FUNCTION__, __LINE__,consumer_name.c_str());
        //             }
        //             else
        //             {
        //                 for(int k = 0; k < message_reply->elements - 1; k += 2)
        //                 {
        //                     field = message_reply->element[k]->str;
        //                     value = message_reply->element[k+1]->str;

        //                 }
        //             }
        //         }
        //     }
        // }
        
    }
}

bool redisMethods::init_create_consumer_group(const std::string &stream_name ,const std::string& group_name)
{
    redisConnRAII connraii;
    auto conn = connraii.get_connection();
    if(!conn)
    {
        LOG_ERROR("%s:%s:%d // 得到的数据库连接为nullptr", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    std::unique_ptr<redisReply, void(*)(redisReply*)> create_group_reply(static_cast<redisReply*>(redisCommand(conn.get(), "XGROUP CREATE %s %s $ MKSTREAM", stream_name.c_str(), group_name.c_str())),[](redisReply* reply){freeReplyObject(reply);});
    if(create_group_reply && create_group_reply->type == REDIS_REPLY_ERROR)
    {
        if(strstr(create_group_reply->str, "BUSYGROUP") != nullptr)
        {
            LOG_INFO("%s:%s:%d // 消费者组%s已经存在", __FILE__, __FUNCTION__, __LINE__,group_name.c_str());
            return true;
        }
        else
        {
            LOG_ERROR("%s:%s:%d // 创建消费者组%s失败", __FILE__, __FUNCTION__, __LINE__,group_name.c_str());
            return false;
        }
    }
    else
    {
        LOG_INFO("%s:%s:%d // 创建消费者组%s成功", __FILE__, __FUNCTION__, __LINE__,group_name.c_str());
        return true;
    }
    
}

std::string redisMethods::get_string_from_redisreply(redisReply* reply)
{
    if(reply == nullptr)
    {
        return "";
    }
    nlohmann::json res_json;
    std::unique_ptr<redisReply,void(*)(redisReply*)> reply_ptr(reply,[](redisReply* reply){freeReplyObject(reply);});
    if(reply_ptr->type == REDIS_REPLY_STRING)
    {
        LOG_WARING("%s:%s:%d // redis返回的是字符串,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply->str);
    }
    else if (reply_ptr->type == REDIS_REPLY_ERROR)
    {
        LOG_WARING("%s:%s:%d // redis返回的是错误,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply->str);
    }
    else if (reply_ptr->type == REDIS_REPLY_INTEGER)
    {
        LOG_WARING("%s:%s:%d // redis返回的是整数,内容为:%d", __FILE__, __FUNCTION__, __LINE__, reply->integer);
    }
    else if(reply_ptr->type == REDIS_REPLY_NIL)
    {
        LOG_WARING("%s:%s:%d // redis返回的是nil", __FILE__, __FUNCTION__, __LINE__);
    } 
    else if(reply_ptr->type == REDIS_REPLY_STATUS)
    {
        LOG_WARING("%s:%s:%d // redis返回的是状态,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply->str);
    }
    else if(reply_ptr->type == REDIS_REPLY_ARRAY)
    {
        for(int i = 0; i < reply_ptr->elements; i++)
        {
            res_json[REDIS_JSON_FIELD_STREAMNAME] = reply->element[i]->element[0]->str;
            for(int j = 0; j < reply->element[i]->element[1]->elements; j++)
            {
                res_json[REDIS_JSON_FIELD_MESSAGEID] = reply->element[i]->element[1]->element[j]->element[0]->str;
                for(int k = 0; k < reply->element[i]->element[1]->element[j]->element[1]->elements - 1; k += 2)
                {
                    res_json[reply->element[i]->element[1]->element[j]->element[1]->element[k]->str] = reply->element[i]->element[1]->element[j]->element[1]->element[k+1]->str;
                }
            }
        }

        // for(int i = 0; i < reply_ptr->elements; i++)
        // {
        //     std::unique_ptr<redisReply,void(*)(redisReply*)> stream_ptr(reply_ptr->element[i],[](redisReply* reply){freeReplyObject(reply);});
        //     if(reply_ptr->type != REDIS_REPLY_ARRAY)
        //     {
        //         LOG_WARING("%s:%s:%d // redis返回的不是数组,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply_ptr->str);
        //     }
        //     else
        //     {
        //         res_json[REDIS_JSON_FIELD_STREAMNAME] = stream_ptr->element[0]->str;
        //         std::unique_ptr<redisReply,void(*)(redisReply*)> message_ptr(stream_ptr->element[1],[](redisReply* reply){freeReplyObject(reply);});
        //         if(message_ptr->type!= REDIS_REPLY_ARRAY)
        //         {
        //             LOG_WARING("%s:%s:%d // redis返回的不是数组,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply_ptr->str);
        //         }
        //         else
        //         {
        //             for(int j = 0; j < message_ptr->elements; j++)
        //             {
        //                 std::unique_ptr<redisReply,void(*)(redisReply*)> message_element_ptr(message_ptr->element[j],[](redisReply* reply){freeReplyObject(reply);});
        //                 if(message_element_ptr->type!= REDIS_REPLY_ARRAY)
        //                 {
        //                     LOG_WARING("%s:%s:%d // redis返回的不是数组,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply_ptr->str);
        //                 }
        //                 else
        //                 {
        //                     res_json[REDIS_JSON_FIELD_MESSAGEID] = message_element_ptr->element[0]->str;
        //                     std::unique_ptr<redisReply,void(*)(redisReply*)> message_content_ptr(message_element_ptr->element[1],[](redisReply* reply){freeReplyObject(reply);});
        //                     if(message_content_ptr->type!= REDIS_REPLY_ARRAY)
        //                     {
        //                         LOG_WARING("%s:%s:%d // redis返回的不是数组,内容为:%s", __FILE__, __FUNCTION__, __LINE__, reply_ptr->str);
        //                     }
        //                     else
        //                     {
        //                         for(int k = 0; k < message_content_ptr->elements - 1; k+=2)
        //                         {
        //                             res_json[message_content_ptr->element[k]->str] = message_content_ptr->element[k+1]->str;
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }
    }
    return res_json.dump();
}


