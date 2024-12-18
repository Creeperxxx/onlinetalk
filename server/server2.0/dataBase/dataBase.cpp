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
database& database::get_instance()
{
    static database instance;
    return instance;
}

std::string database::get_user_info_from_cacheordb(const std::string &account ,loginType type)
{
    //1. 先查redis
    //2. 查不到就查mysql
    //3. 更新到redis
    std::string redis_query_key;
    std::string res_string;
    std::string mysql_query_sql;
    std::vector<std::variant<int,std::string>> params;
    std::string redis_findid_key;
    std::string user_id;
    if(type == loginType::USERNAME)
    {
        // std::string redis_findid_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_find_userid_key(account);
        redis_findid_key = m_redisMethods->build_key_find_userid(account);
        user_id = m_redisMethods->redis_get(redis_findid_key);
        if(user_id.empty())
        {
            LOG_WARING("%s:%s:%d // 在redis中找不到%s对应的id",__FILE__,__FUNCTION__,__LINE__,account);
            mysql_query_sql = "SELECT * FROM " + MYSQL_TABLE + " WHERE " + USER_NAME_FIELD + " =?";
            params.push_back(account);
        }
        else   
        {
            // redis_query_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_cache_key(user_id);
            redis_query_key = m_redisMethods->build_key_find_userinfo(user_id);
            res_string = m_redisMethods->redis_get(redis_query_key);
            if(!res_string.empty())
            {
                return res_string;
            }
            else
            {
                mysql_query_sql = "SELECT * FROM " + MYSQL_TABLE + " WHERE " + USER_ID_FIELD + " =?";
                params.push_back(user_id);
            }
        }
    }
    else
    {
        // redis_query_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_cache_key(account);
        redis_query_key = m_redisMethods->build_key_find_userinfo(account);
        res_string = m_redisMethods->redis_get(redis_query_key);
        if(!res_string.empty())
        {
            return res_string;
        }
        else
        {
            mysql_query_sql = "SELECT * FROM " + MYSQL_TABLE + " WHERE " + USER_ID_FIELD + " =?";
            params.push_back(account);
        }
    }
    auto result = m_mysqlMethods->execute_sql(statementType::QUERY,mysql_query_sql,params);
    if(std::holds_alternative<bool>(result))
    {
        LOG_ERROR("%s:%s:%d // 执行查询语句返回bool类型",__FILE__,__FUNCTION__,__LINE__);
        return "";
    }
    else
    {
        auto res = std::move(std::get<std::unique_ptr<sql::ResultSet,decltype(&sql::ResultSet::close)>>(result));
        if(res->next())
        {
            nlohmann::json user_info;
            user_info[USER_ID_FIELD] = std::to_string(res->getInt(USER_ID_FIELD));
            user_info[USER_NAME_FIELD] = res->getString(USER_NAME_FIELD);
            user_info[USER_PASSWD_FIELD] = res->getString(USER_PASSWD_FIELD);
            user_info[USER_EMAIL_FIELD] = res->getString(USER_EMAIL_FIELD);

            res_string = user_info.dump();
            if(res_string.empty())
            {
                LOG_ERROR("%s:%s:%d // 序列化json失败",__FILE__,__FUNCTION__,__LINE__);
                return "";
            }
            //更新到redis
            redis_findid_key = m_redisMethods->build_key_find_userid(res->getString(USER_NAME_FIELD));
            m_redisMethods->redis_set(redis_findid_key,std::to_string(res->getInt(USER_ID_FIELD)),std::nullopt,REDIS_EXPIRE_USERINFO);
            redis_query_key = m_redisMethods->build_key_find_userinfo(std::to_string(res->getInt(USER_ID_FIELD)));
            m_redisMethods->redis_set(redis_query_key,res_string,std::nullopt,REDIS_EXPIRE_USERINFO);
            // bool flag = dynamic_cast<redisMethods*>(m_redisMethods.get())->update_redis_cache(res->getString(USER_NAME_FIELD),std::to_string(res->getInt(USER_ID_FIELD)),res_string);

            // bool flag = m_redisMethods->update_redis_cache(res->getString(USER_NAME_FIELD),std::to_string(res->getInt(USER_ID_FIELD)),res_string);
            // if(!flag)
            // {
            //     LOG_ERROR("%s:%s:%d // 更新redis缓存失败",__FILE__,__FUNCTION__,__LINE__);
            // }
            // return res_string;
        }
        else
        {
            LOG_ERROR("%s:%s:%d // mysql没有找到用户信息",__FILE__,__FUNCTION__,__LINE__);
            return "";
        }
    }
}

void database::set_user_info_from_cacheanddb(const std::string &userid, const std::string &username, const std::string &password, const std::string &email)
{
    // 1. 先更新mysql
    //"INSERT INTO " + MYSQL_TABLE + " (" + USER_ID_FIELD + ", " + USER_NAME_FIELD + ", " + USER_PASSWD_FIELD + ", " + USER_EMAIL_FIELD + ") VALUES (?, ?, ?, ?)" +
//                                      " ON DUPLICATE KEY UPDATE " + USER_NAME_FIELD + " = VALUES(" + USER_NAME_FIELD + "), " + USER_PASSWD_FIELD + " = VALUES(" + USER_PASSWD_FIELD + "), " + USER_EMAIL_FIELD + " = VALUES(" + USER_EMAIL_FIELD + ")";
    std::string update_sql = "INSERT INTO " + MYSQL_TABLE + " (" + USER_ID_FIELD + ", " + USER_NAME_FIELD + ", " + USER_PASSWD_FIELD + ", " + USER_EMAIL_FIELD + ") VALUES (?,?,?,?)" +
                             " ON DUPLICATE KEY UPDATE " + USER_NAME_FIELD + " = VALUES(" + USER_NAME_FIELD + "), " + USER_PASSWD_FIELD + " = VALUES(" + USER_PASSWD_FIELD + "), " + USER_EMAIL_FIELD + " = VALUES(" + USER_EMAIL_FIELD + ")";
    std::vector<std::variant<int,std::string>> params;
    params.push_back(std::stoi(userid));
    params.push_back(username);
    params.push_back(password);
    params.push_back(email);
    auto res = m_mysqlMethods->execute_sql(statementType::NOTQUERY,update_sql,params);
    if(std::holds_alternative<bool>(res))
    {
        if(std::get<bool>(res))
        {
            LOG_INFO("%s:%s:%d // 更新mysql成功",__FILE__,__FUNCTION__,__LINE__);
        }
        else
        {
            LOG_ERROR("%s:%s:%d // 更新mysql失败",__FILE__,__FUNCTION__,__LINE__);
        }
    }
    else
    {
        LOG_ERROR("%s:%s:%d // 执行更新语句返回非bool类型",__FILE__,__FUNCTION__,__LINE__);
    }
    
    // 2. 将redis中的数据删除
    // std::string cache_key = dynamic_cast<redisMethods*>(m_redisMethods.get())->build_cache_key(userid);
    std::string cache_key = m_redisMethods->build_key_find_userinfo(userid);
    m_redisMethods->redis_del(cache_key);
    std::string redis_findid_key = m_redisMethods->build_key_find_userid(username);
    m_redisMethods->redis_del(redis_findid_key);
    // if(m_redisMethods->redis_del(cache_key))
    // {
    //     LOG_INFO("%s:%s:%d // 删除redis成功",__FILE__,__FUNCTION__,__LINE__);
    //     return;
    // }
    // else
    // {
    //     LOG_WARING("%s:%s:%d // 删除redis失败",__FILE__,__FUNCTION__,__LINE__);
    //     return;
    // }
}

void database::set_offline_data_from_cacheanddb(const std::string &userid,const std::vector<uint8_t> &data)
{
    //先将数据序列化
    
}

