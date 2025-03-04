#include "dataBase.h"

// std::once_flag databaseV1::init_once;
// std::unique_ptr<databaseV1> databaseV1::m_instance = nullptr;

// databaseV1 &databaseV1::getInstance()
// {
//     std::call_once(init_once, []()
//                    { m_instance.reset(new databaseV1()); });
//     return *m_instance;
// }

// // std::string databaseV1::get_user_info(int userid)
// std::string databaseV1::get_user_info(const std::string &account, loginType type)
// {
//     // 1. 先读redis
//     //  std::string cache_key = CACHE_PRIEFIX + std::to_string(userid);
//     std::string query_sql;
//     if (type == loginType::USERNAME)
//     {
//         std::string redis_query_key = CACHE_FINDUSERID_PRIEFIX + account;
//         std::string user_id = m_redisMethods->redis_get(redis_query_key);
//         if (user_id.empty())
//         {
//             LOG_WARING("%s:%s:%d // 在redis中找不到%s对应的id", __FILE__, __FUNCTION__, __LINE__, account);
//             // 读mysql
//             query_sql = "SELECT * FROM " + MYSQL_TABLE + " HWERE " + USER_NAME_FIELD + " = " + account;
//         }
//         else
//         {
//             std::string cache_key = CACHE_PRIEFIX + user_id;
//             std::string res_string = m_redisMethods->redis_get(cache_key);
//             if(!res_string.empty())
//             {
//                 return res_string;
//             }
//             else
//             {
//                 query_sql = "SELECT * FROM " + MYSQL_TABLE + " HWERE " + USER_ID_FIELD + " = " + user_id;
//             }
//         }
//     }
//     else
//     {
//         std::string cache_key = CACHE_PRIEFIX + account;
//         std::string res_string = m_redisMethods->redis_get(cache_key);
//         if(!res_string.empty())
//         {
//             return res_string;
//         }
//         else
//         {
//             query_sql = "SELECT * FROM " + MYSQL_TABLE + " HWERE " + USER_ID_FIELD + " = " + account;
//         }
//     }
//     std::string res_string;
//     // std::string cache_key = CACHE_PRIEFIX + userid;
//     // std::string res_string = m_redisMethods->redis_get(cache_key);
//     // if (!res_string.empty())
//     // {
//     //     return res_string;
//     // }
//     // else
//     // {
//         // 2. 找不到了读mysql
//         // std::string query_sql = "SELECT * FROM " + MYSQL_TABLE + " HWERE " + USER_ID_FIELD + " = " + userid; // std::to_string(userid);

//         auto res = m_mysqlMethods->execute_sql(statementType::QUERY, query_sql,"");
//         if (res == nullptr)
//         {
//             return "";
//         }
//         else
//         {
//             if (res->next())
//             {
//                 nlohmann::json user_info;
//                 user_info[USER_ID_FIELD] = std::to_string(res->getInt(USER_ID_FIELD));
//                 user_info[USER_NAME_FIELD] = res->getString(USER_NAME_FIELD);
//                 user_info[USER_PASSWD_FIELD] = res->getString(USER_PASSWD_FIELD);
//                 user_info[USER_EMAIL_FIELD] = res->getString(USER_EMAIL_FIELD);
//                 res_string = user_info.dump();
//                 // 3. 更新到redis
//                 if(type == loginType::USERNAME)
//                 {
//                     std::string redis_query_key = CACHE_FINDUSERID_PRIEFIX + account;
//                     if(!m_redisMethods->redis_set(redis_query_key, std::to_string(res->getInt(USER_ID_FIELD))))
//                     {
//                         LOG_ERROR("%s:%s:%d // 更新redis失败", __FILE__, __FUNCTION__, __LINE__);
//                     }
//                     redis_query_key = CACHE_PRIEFIX + std::to_string(res->getInt(USER_ID_FIELD));
//                     if(!m_redisMethods->redis_set(redis_query_key, std::to_string(res->getInt(USER_ID_FIELD))))
//                     {
//                         LOG_ERROR("%s:%s:%d // 更新redis失败", __FILE__, __FUNCTION__, __LINE__);
//                     }
//                 }
//                 else
//                 {
//                     std::string redis_query_key = CACHE_PRIEFIX + account;
//                     if(!m_redisMethods->redis_set(redis_query_key, std::to_string(res->getInt(USER_ID_FIELD))))
//                     {
//                         LOG_ERROR("%s:%s:%d // 更新redis失败", __FILE__, __FUNCTION__, __LINE__);
//                     }
//                 }
//                 return res_string;

//                 // if (!m_redisMethods->redis_set(cache_key, res_string))
//                 // {
//                 //     LOG_ERROR("%s:%s:%d // 更新redis失败", __FILE__, __FUNCTION__, __LINE__);
//                 // }
//                 // return res_string;
//             }
//             else
//             {
//                 LOG_ERROR("%s:%s:%d // mysql没有找到用户信息", __FILE__, __FUNCTION__, __LINE__);
//                 return "";
//             }
//         }
//     // }
// }

// // void databaseV1::set_user_info(int userid,const std::string& username,const std::string& password ,const std::string& email)
// void databaseV1::set_user_info(const std::string &userid, const std::string &username, const std::string &password, const std::string &email)
// {
//     // 1. 先更新mysql
//     //  std::string update_sql = "UPDATE " + MYSQL_TABLE + " SET " + USER_NAME_FIELD + " = " + username + " , " + USER_PASSWD_FIELD + " = " + password + " WHERE " + USER_ID_FIELD + " = " + std::to_string(userid);
//     auto conn = static_cast<mysqlMethodsV1 *>(m_mysqlMethods.get())->getConnection();
//     if (conn)
//     {
//         std::string prepare_string = "INSERT INTO " + MYSQL_TABLE + " (" + USER_ID_FIELD + ", " + USER_NAME_FIELD + ", " + USER_PASSWD_FIELD + ", " + USER_EMAIL_FIELD + ") VALUES (?, ?, ?, ?)" +
//                                      " ON DUPLICATE KEY UPDATE " + USER_NAME_FIELD + " = VALUES(" + USER_NAME_FIELD + "), " + USER_PASSWD_FIELD + " = VALUES(" + USER_PASSWD_FIELD + "), " + USER_EMAIL_FIELD + " = VALUES(" + USER_EMAIL_FIELD + ")";
//         std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(prepare_string));
//         pstmt->setInt(1, std::stoi(userid));
//         pstmt->setString(2, username);
//         pstmt->setString(3, password);
//         pstmt->setString(4, email);
//         pstmt->execute();

//         // 2. 将redis中的数据删除
//         std::string cache_key = CACHE_PRIEFIX + userid;
//         if (m_redisMethods->redis_del(cache_key))
//         {
//             LOG_INFO("%s:%s:%d // 删除redis成功", __FILE__, __FUNCTION__, __LINE__);
//             return;
//         }
//         else
//         {
//             LOG_WARING("%s:%s:%d // 删除redis失败", __FILE__, __FUNCTION__, __LINE__);
//             return;
//         }
//     }
// }

// databaseV2& databaseV2::get_instance()
// {
//     static databaseV2 instance;
//     return instance;
// }
// database &database::get_instance()
// {
//     static database instance;
//     return instance;
// }

// std::string database::get_user_info_from_cacheordb(const std::string &account, loginType type)
// {
//     // 1. 先查redis
//     // 2. 查不到就查mysql
//     // 3. 更新到redis
//     std::string redis_query_key;
//     std::string res_string;
//     std::string mysql_query_sql;
//     std::vector<std::variant<int, std::string>> params;
//     std::string redis_findid_key;
//     std::string user_id;
//     if (type == loginType::USERNAME)
//     {
//         // std::string redis_findid_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_find_userid_key(account);
//         redis_findid_key = m_redisMethods->build_key_find_userid(account);
//         user_id = m_redisMethods->redis_set_get(redis_findid_key);
//         if (user_id.empty())
//         {
//             LOG_WARING("%s:%s:%d // 在redis中找不到%s对应的id", __FILE__, __FUNCTION__, __LINE__, account);
//             mysql_query_sql = "SELECT * FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_NAME + " =?";
//             params.push_back(account);
//         }
//         else
//         {
//             // redis_query_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_cache_key(user_id);
//             redis_query_key = m_redisMethods->build_key_find_userinfo(user_id);
//             res_string = m_redisMethods->redis_set_get(redis_query_key);
//             if (!res_string.empty())
//             {
//                 return res_string;
//             }
//             else
//             {
//                 mysql_query_sql = "SELECT * FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_ID + " =?";
//                 params.push_back(user_id);
//             }
//         }
//     }
//     else
//     {
//         // redis_query_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_cache_key(account);
//         redis_query_key = m_redisMethods->build_key_find_userinfo(account);
//         res_string = m_redisMethods->redis_set_get(redis_query_key);
//         if (!res_string.empty())
//         {
//             return res_string;
//         }
//         else
//         {
//             mysql_query_sql = "SELECT * FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_ID + " =?";
//             params.push_back(account);
//         }
//     }
//     auto result = m_mysqlMethods->execute_sql(statementType::QUERY, mysql_query_sql, params);
//     if (std::holds_alternative<bool>(result))
//     {
//         LOG_ERROR("%s:%s:%d // 执行查询语句返回bool类型", __FILE__, __FUNCTION__, __LINE__);
//         return "";
//     }
//     else
//     {
//         auto res = std::move(std::get<std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>>(result));
//         if (res->next())
//         {
//             nlohmann::json user_info;
//             user_info[MYSQL_TABLE_USER_FIELD_ID] = std::to_string(res->getInt(MYSQL_TABLE_USER_FIELD_ID));
//             user_info[MYSQL_TABLE_USER_FIELD_NAME] = res->getString(MYSQL_TABLE_USER_FIELD_NAME);
//             user_info[MYSQL_TABLE_USER_FIELD_PASSWD] = res->getString(MYSQL_TABLE_USER_FIELD_PASSWD);
//             user_info[MYSQL_TABLE_USER_FIELD_EMAIL] = res->getString(MYSQL_TABLE_USER_FIELD_EMAIL);

//             res_string = user_info.dump();
//             if (res_string.empty())
//             {
//                 LOG_ERROR("%s:%s:%d // 序列化json失败", __FILE__, __FUNCTION__, __LINE__);
//                 return "";
//             }
//             // 更新到redis
//             redis_findid_key = m_redisMethods->build_key_find_userid(res->getString(MYSQL_TABLE_USER_FIELD_NAME));
//             m_redisMethods->redis_set_set(redis_findid_key, std::to_string(res->getInt(MYSQL_TABLE_USER_FIELD_ID)), std::nullopt, REDIS_EXPIRE_USERINFO);
//             redis_query_key = m_redisMethods->build_key_find_userinfo(std::to_string(res->getInt(MYSQL_TABLE_USER_FIELD_ID)));
//             m_redisMethods->redis_set_set(redis_query_key, res_string, std::nullopt, REDIS_EXPIRE_USERINFO);
//             // bool flag = dynamic_cast<redisMethods*>(m_redisMethods.get())->update_redis_cache(res->getString(USER_NAME_FIELD),std::to_string(res->getInt(USER_ID_FIELD)),res_string);

//             // bool flag = m_redisMethods->update_redis_cache(res->getString(USER_NAME_FIELD),std::to_string(res->getInt(USER_ID_FIELD)),res_string);
//             // if(!flag)
//             // {
//             //     LOG_ERROR("%s:%s:%d // 更新redis缓存失败",__FILE__,__FUNCTION__,__LINE__);
//             // }
//             // return res_string;
//         }
//         else
//         {
//             LOG_ERROR("%s:%s:%d // mysql没有找到用户信息", __FILE__, __FUNCTION__, __LINE__);
//             return "";
//         }
//     }
// }

// void database::set_user_info_from_cacheanddb(const std::string &userid, const std::string &username, const std::string &password, const std::string &email)
// {
//     // 1. 先更新mysql
//     //"INSERT INTO " + MYSQL_TABLE + " (" + USER_ID_FIELD + ", " + USER_NAME_FIELD + ", " + USER_PASSWD_FIELD + ", " + USER_EMAIL_FIELD + ") VALUES (?, ?, ?, ?)" +
//     //                                      " ON DUPLICATE KEY UPDATE " + USER_NAME_FIELD + " = VALUES(" + USER_NAME_FIELD + "), " + USER_PASSWD_FIELD + " = VALUES(" + USER_PASSWD_FIELD + "), " + USER_EMAIL_FIELD + " = VALUES(" + USER_EMAIL_FIELD + ")";
//     std::string update_sql = "INSERT INTO " + MYSQL_TABLE_USERS + " (" + MYSQL_TABLE_USER_FIELD_ID + ", " + MYSQL_TABLE_USER_FIELD_NAME + ", " + MYSQL_TABLE_USER_FIELD_PASSWD + ", " + MYSQL_TABLE_USER_FIELD_EMAIL + ") VALUES (?,?,?,?)" +
//                              " ON DUPLICATE KEY UPDATE " + MYSQL_TABLE_USER_FIELD_NAME + " = VALUES(" + MYSQL_TABLE_USER_FIELD_NAME + "), " + MYSQL_TABLE_USER_FIELD_PASSWD + " = VALUES(" + MYSQL_TABLE_USER_FIELD_PASSWD + "), " + MYSQL_TABLE_USER_FIELD_EMAIL + " = VALUES(" + MYSQL_TABLE_USER_FIELD_EMAIL + ")";
//     std::vector<std::variant<int, std::string>> params;
//     params.push_back(std::stoi(userid));
//     params.push_back(username);
//     params.push_back(password);
//     params.push_back(email);
//     auto res = m_mysqlMethods->execute_sql(statementType::NOTQUERY, update_sql, params);
//     if (std::holds_alternative<bool>(res))
//     {
//         if (std::get<bool>(res))
//         {
//             LOG_INFO("%s:%s:%d // 更新mysql成功", __FILE__, __FUNCTION__, __LINE__);
//         }
//         else
//         {
//             LOG_ERROR("%s:%s:%d // 更新mysql失败", __FILE__, __FUNCTION__, __LINE__);
//         }
//     }
//     else
//     {
//         LOG_ERROR("%s:%s:%d // 执行更新语句返回非bool类型", __FILE__, __FUNCTION__, __LINE__);
//     }

//     // 2. 将redis中的数据删除
//     // std::string cache_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_cache_key(userid);
//     std::string cache_key = m_redisMethods->build_key_find_userinfo(userid);
//     m_redisMethods->redis_del(cache_key);
//     std::string redis_findid_key = m_redisMethods->build_key_find_userid(username);
//     m_redisMethods->redis_del(redis_findid_key);
//     // if(m_redisMethods->redis_del(cache_key))
//     // {
//     //     LOG_INFO("%s:%s:%d // 删除redis成功",__FILE__,__FUNCTION__,__LINE__);
//     //     return;
//     // }
//     // else
//     // {
//     //     LOG_WARING("%s:%s:%d // 删除redis失败",__FILE__,__FUNCTION__,__LINE__);
//     //     return;
//     // }
// }

// void database::set_offline_data_from_cacheanddb(const std::string &userid,const std::vector<uint8_t> &data, const time_t score,const int &time_stamp)
// void set_offline_data_from_cacheanddb(const std::string &key_userid, const time_t& score_timestamp , const std::vector<uint8_t> value_offlinemsg,std::optional<int> expire)
// {
//     //将值存储在sorted set中，以时间戳作为分数

// }

// void database::set_msg_from_db(const std::string &sender_id, const std::string &sender_name, const std::string &receiver_id, const std::string &receiver_name, const std::string &msg, const std::string &type)
// {
//     // mysqlConnRAII connraii;
//     // auto conn = connraii.get_connection();
//     // if(conn == nullptr)
//     // {
//     //     LOG_ERROR("%s:%s:%d // 获取mysql连接失败",__FILE__,__FUNCTION__,__LINE__);
//     //     return;
//     // }

//     std::string insert_sql = "INSERT INTO " + MYSQL_TABLE_MESSAGES + " (" + MYSQL_TABLE_MESSAGES_FIELD_SENDER_ID + ", " + MYSQL_TABLE_MESSAGES_FIELD_SENDER_NAME + ", " + MYSQL_TABLE_MESSAGES_FIELD_RECEIVER_ID + ", " + MYSQL_TABLE_MESSAGES_FIELD_RECEIVER_NAME + ", " + MYSQL_TABLE_MESSAGES_FIELD_CONTENT + ", " + MYSQL_TABLE_MESSAGES_FIELD_MESSAGETYPE + ") VALUES (?,?,?,?,?,?)";
//     std::vector<std::variant<int, std::string>> params;
//     params.push_back(std::stoi(sender_id));
//     params.push_back(sender_name);
//     params.push_back(std::stoi(receiver_id));
//     params.push_back(receiver_name);
//     params.push_back(msg);
//     params.push_back(type);
//     auto res = m_mysqlMethods->execute_sql(statementType::NOTQUERY, insert_sql, params);
//     if (std::holds_alternative<bool>(res))
//     {
//         if (std::get<bool>(res))
//         {
//             LOG_INFO("%s:%s:%d // 插入mysql成功", __FILE__, __FUNCTION__, __LINE__);
//         }
//         else
//         {
//             LOG_ERROR("%s:%s:%d // 插入mysql失败", __FILE__, __FUNCTION__, __LINE__);
//         }
//     }
//     else
//     {
//         LOG_ERROR("%s:%s:%d // 执行插入语句返回非bool类型", __FILE__, __FUNCTION__, __LINE__);
//     }
// }

// std::shared_ptr<std::vector<std::string>> database::get_alluserid_from_db()
// {
//     std::string select_sql = "SELECT " + MYSQL_TABLE_USER_FIELD_ID + " FROM " + MYSQL_TABLE_USERS;
//     auto res = m_mysqlMethods->execute_sql(statementType::QUERY, select_sql);
//     std::string userid;
//     std::shared_ptr<std::vector<std::string>> userid_vec;
//     if (std::holds_alternative<bool>(res))
//     {
//         LOG_ERROR("%s:%s:%d // 执行查询语句返回bool类型", __FILE__, __FUNCTION__, __LINE__);
//         return nullptr;
//     }
//     else
//     {
//         auto resultset = std::move(std::get<std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>>(res));
//         while (resultset->next())
//         {
//             userid = std::to_string(resultset->getUInt64(MYSQL_TABLE_USER_FIELD_ID)); // id字段为bigint unsigned，要用无符号64位接收,使用stou11转回来
//             userid_vec->push_back(userid);
//         }
//         return userid_vec;
//     }
// }

std::shared_ptr<std::vector<std::string>> database::redis_stream_xreadgroup(const std::string &stream_name, const std::string &group_name, const std::string &consumer_name, const std::optional<int> block_time, const std::optional<int> count)
{
    // return m_redisMethods->redis_stream_xreadgroup(stream_name, group_name, consumer_name, block_time, count);
    return redisMethodsCor::redis_stream_xreadgroup(stream_name, group_name, consumer_name, block_time, count);
    // auto res_vec = m_redisMethods->redis_stream_xreadgroup(stream_name,group_name,consumer_name,block_time,count);
    // if(res_vec->empty())
    // {
    //     LOG_ERROR("%s:%s:%d // 从stream中获取数据失败",__FILE__,__FUNCTION__,__LINE__);
    //     return;
    // }
    // for(auto& res : *res_vec)
    // {
    //     //解析消息

    // }
}

void database::init_stream_consumer_group(const std::string &stream_name, const std::string &groupname)
{
    // m_redisMethods->init_stream_consumer_group(stream_name, groupname);
    redisMethodsCor::init_stream_consumer_group(stream_name, groupname);
}

// std::string database::redis_stream_xadd(const std::string& stream,const std::vector<std::pair<std::string,std::string>>& fields)
std::string database::redis_stream_xadd_fields(const std::string &stream, std::shared_ptr<std::vector<std::pair<std::string, std::string>>> fields)
{
    // return m_redisMethods->redis_stream_xadd(stream, fields);
    return redisMethodsCor::redis_stream_xadd(stream,fields);
}

int database::redis_stream_xack_single(const std::string &stream, const std::string &groupname, const std::string &msgid)
{
    bool flag;
    int trycount = REDIS_STREAM_XACK_FAILED_MAXRETRY;
    while (trycount != 0)
    {
        // flag = m_redisMethods->redis_stream_xack(stream, groupname, msgid);
        flag = redisMethodsCor::redis_stream_xack(stream,groupname,msgid);
        if (flag == true)
        {
            break;
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(REDIS_STREAM_XACK_FAILED_SLEEPTIME));
            trycount--;
        }
    }
    if (trycount == 0)
    {
        LOG_ERROR("%s:%s:%d // 向stream中ack失败", __FILE__, __FUNCTION__, __LINE__);
        return 0;
    }
    else
    {
        return 1;
    }
}

int database::redis_stream_xack_batch(const std::string &stream, const std::string &groupname, std::shared_ptr<std::vector<std::string>> msgids)
{
    int count = 0;
    int res;
    for (auto &msgid : *msgids)
    {
        res = redis_stream_xack_single(stream, groupname, msgid);
        if (res == 1)
        {
            count++;
        }
    }
    return count;
}

std::string database::redis_stream_xadd_msg(const std::string &stream, const std::string &msg)
{
    // return m_redisMethods->redis_stream_xadd(stream, msg);
    return redisMethodsCor::redis_stream_xadd(stream,msg);
}

bool database::is_user_exist_in_db(const std::string &username, const std::string &email)
{
    std::string select_sql = "SELECT * FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_NAME + " = ? AND " + MYSQL_TABLE_USER_FIELD_EMAIL + " = ?";
    std::vector<std::any> params;
    params.push_back(username);
    params.push_back(email);
    // auto res = m_mysqlMethods->execute_sql(statementType::QUERY, select_sql, params);
    auto res = mysqlMethods::execute_sql(statementType::QUERY, select_sql, params);
    auto result = res->get_res();
    while (result != nullptr && result->next())
    {
        if (result->getString(MYSQL_TABLE_USER_FIELD_NAME) == username && result->getString(MYSQL_TABLE_USER_FIELD_EMAIL) == email)
        {
            return true;
        }
    }
    return false;
    // if(std::holds_alternative<bool>(res) == true)
    // {
    //     LOG_ERROR("%s:%s:%d // 执行查询语句返回bool类型", __FILE__, __FUNCTION__, __LINE__);
    //     return true;
    // }
    // else
    // {
    //     auto resultset = std::move(std::get<std::unique_ptr<sql::ResultSet, decltype(&sql::ResultSet::close)>>(res));
    //     while(resultset != nullptr && resultset->next())
    //     {
    //         return true;
    //     }
    //     return false;
    // }
}

uint64_t database::add_user_to_db(const std::string &username, const std::string &password, const std::string &email)
{
    std::string add_sql = "INSERT INTO user(username,password,email) VALUES(?,?,?)";
    // std::vector <std::variant<int,std::string>> params = {username,password,email};
    std::vector<std::any> params = {username, password, email};
    // auto res = m_mysqlMethods->execute_sql(statementType::NOTQUERY,add_sql,params);
    // auto res = m_mysqlMethods->execute_sql(statementType::INSERT, add_sql, params);
    auto res = mysqlMethods::execute_sql(statementType::INSERT, add_sql, params);
    if (res != nullptr)
    {
        return res->get_generated_key();
    }
    return 0;
    // if(std::holds_alternative<bool>(res))
    // {
    //     if(std::get<bool>(res))
    //     {
    //         return
    //     }
    //     else
    //     {
    //         LOG_WARING("%s:%s:%d // 添加用户失败",__FILE__,__FUNCTION__,__LINE__);
    //         return 0;
    //     }
    // }
    // else
    // {
    //     LOG_ERROR("%s:%s:%d // 返回的不是bool类型",__FILE__,__FUNCTION__,__LINE__);
    // }
}

bool database::get_login_result(const std::string type, const std::string &account, const std::string &password)
{
    std::string query_sql = "SELECT * FROM " + MYSQL_TABLE_USERS + " WHERE ";
    if (type == MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_ID)
    {
        query_sql += MYSQL_TABLE_USER_FIELD_ID + " = ?";
    }
    else if (type == MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_NAME)
    {
        query_sql += MYSQL_TABLE_USER_FIELD_NAME + " = ?";
    }
    else
    {
        LOG_ERROR("%s:%s:%d // 登录类型错误", __FILE__, __FUNCTION__, __LINE__);
        return false;
    }
    query_sql += " AND " + MYSQL_TABLE_USER_FIELD_PASSWD + " = ?";
    std::vector<std::any> params;
    params.push_back(account);
    params.push_back(password);
    // auto res = m_mysqlMethods->execute_sql(statementType::QUERY, query_sql, params);
    auto res = mysqlMethods::execute_sql(statementType::QUERY, query_sql, params);
    if (res != nullptr)
    {
        auto resultset = res->get_res();
        while (resultset != nullptr && resultset->next())
        {
            if (resultset->getString(MYSQL_TABLE_USER_FIELD_PASSWD) == password &&
                ((type == MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_ID && resultset->getString(MYSQL_TABLE_USER_FIELD_ID) == account) || (type == MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_NAME && resultset->getString(MYSQL_TABLE_USER_FIELD_NAME) == account)))
            {
                return true;
            }
        }
    }
    return false;
}

void database::add_logined_userid(const std::string& set_key,std::unique_ptr<std::vector<std::string>> userid_vec)
{
    // m_redisMethods->redis_sadd_push_logined_userid(std::move(userid_vec));
    redisMethodsLoginedUserid::redis_sadd_push_logined_userid(set_key,std::move(userid_vec));
}

bool database::is_userid_exist_in_redis(const std::string& set_key,const std::string& userid)
{
    return redisMethodsLoginedUserid::redis_sismember_logined_userid(set_key,userid);
}

void database::delete_logined_userid_in_redis(const std::string& set_key ,const std::string& userid)
{
    redisMethodsLoginedUserid::redis_srem_pop_logined_userid(set_key,userid);
}

std::string database::get_userid_by_name_passwd_in_db(const std::string& username,const std::string& passwd)
{
    mysqlConnRAII connraii;
    auto conn = connraii.get_connection();
    if(conn == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 获取数据库连接失败",__FILE__,__FUNCTION__,__LINE__);
        return "";
    }
    // std::string select_sql = "SELECT " + MYSQL_TABLE_USER_FIELD_ID + " FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_NAME + " = ?";
    std::string select_sql = "SELECT " + MYSQL_TABLE_USER_FIELD_ID + " FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_NAME + " = ? AND " + MYSQL_TABLE_USER_FIELD_PASSWD + " = ?";
    std::vector<std::any> params;
    params.push_back(username);
    params.push_back(passwd);
    auto res = mysqlMethods::execute_sql(statementType::QUERY, select_sql, params);
    if(res == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 执行查询语句失败",__FILE__,__FUNCTION__,__LINE__);
        return "";
    }
    auto result = res->get_res();
    if(result == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 执行查询语句返回nullptr",__FILE__,__FUNCTION__,__LINE__);
        return "";
    }
    if(result->next())
    {
        return std::to_string(result->getUInt64(MYSQL_TABLE_USER_FIELD_ID));
    }
    return "";
}

bool database::is_login_success_by_userid_in_db(const std::string& userid,const std::string& passwd)
{
    mysqlConnRAII connraii;
    auto conn = connraii.get_connection();
    if(conn == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 获取数据库连接失败",__FILE__,__FUNCTION__,__LINE__);
        return false;
    }
    std::string select_sql = "SELECT " + MYSQL_TABLE_USER_FIELD_ID + " FROM " + MYSQL_TABLE_USERS + " WHERE " + MYSQL_TABLE_USER_FIELD_ID + " = ? AND " + MYSQL_TABLE_USER_FIELD_PASSWD + " = ?";
    std::vector<std::any> params;
    params.push_back(userid);
    params.push_back(passwd);
    auto res = mysqlMethods::execute_sql(statementType::QUERY, select_sql, params);
    if(res == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 执行查询语句失败",__FILE__,__FUNCTION__,__LINE__);
        return false;
    }
    auto result = res->get_res();
    if(result == nullptr)
    {
        LOG_ERROR("%s:%s:%d // 执行查询语句返回nullptr",__FILE__,__FUNCTION__,__LINE__);
        return false;
    }
    if(result->next())
    {
        return true;
    }
    return false;
}