#include "dataBase.h"
const std::string CACHE_PRIEFIX = "user:";

std::once_flag databaseV1::init_once;
std::unique_ptr<databaseV1> databaseV1::m_instance = nullptr;

databaseV1& databaseV1::getInstance()
{
    std::call_once(init_once, [](){
        m_instance.reset(new databaseV1());
    });
    return *m_instance;
}

std::string databaseV1::get_user_info(int userid)
{
    //1. 先读redis
    std::string cache_key = CACHE_PRIEFIX + std::to_string(userid);
    std::string res_string = m_redisMethods->redis_get(cache_key);
    if(!res_string.empty())
    {
        return res_string;
    }
    else
    {
        //2. 找不到了读mysql
        std::string query_sql = "SELECT * FROM " + MYSQL_TABLE + " HWERE " + USER_ID_FIELD + " = " + std::to_string(userid);
        auto res = m_mysqlMethods->execute_query(query_sql);
        if(res == nullptr)
        {
            return "";
        }
        else
        {
            if(res->next())
            {
                nlohmann::json user_info;
                user_info[USER_ID_FIELD] = res->getInt(USER_ID_FIELD);
                user_info[USER_NAME_FIELD] = res->getString(USER_NAME_FIELD);
                user_info[USER_PASSWD_FIELD] = res->getString(USER_PASSWD_FIELD);
                user_info[USER_EMAIL_FIELD] = res->getString(USER_EMAIL_FIELD);
                res_string = user_info.dump();
                //3. 更新到redis
                if(!m_redisMethods->redis_set(cache_key, res_string))
                {
                    LOG_ERROR("%s:%s:%d // 更新redis失败", __FILE__, __FUNCTION__, __LINE__);
                }
                return res_string;
            }
            else
            {
                LOG_ERROR("%s:%s:%d // mysql没有找到用户信息", __FILE__, __FUNCTION__, __LINE__);
                return "";
            }
        }
    }
}

void databaseV1::set_user_info(int userid,const std::string& username,const std::string& password ,const std::string& email)
{
    //1. 先更新mysql
    // std::string update_sql = "UPDATE " + MYSQL_TABLE + " SET " + USER_NAME_FIELD + " = " + username + " , " + USER_PASSWD_FIELD + " = " + password + " WHERE " + USER_ID_FIELD + " = " + std::to_string(userid);
    auto conn = static_cast<mysqlMethodsV1*>(m_mysqlMethods.get())->getConnection();
    if(conn)
    {
        std::string prepare_string = "INSERT INTO " + MYSQL_TABLE + " (" + USER_ID_FIELD + ", " + USER_NAME_FIELD + ", " + USER_PASSWD_FIELD + ", " + USER_EMAIL_FIELD + ") VALUES (?, ?, ?, ?)" + 
        " ON DUPLICATE KEY UPDATE " + USER_NAME_FIELD + " = VALUES(" + USER_NAME_FIELD + "), " + USER_PASSWD_FIELD + " = VALUES(" + USER_PASSWD_FIELD + "), " + USER_EMAIL_FIELD + " = VALUES(" + USER_EMAIL_FIELD + ")";
        std::unique_ptr<sql::PreparedStatement> pstmt(conn->prepareStatement(prepare_string));
        pstmt->setInt(1, userid);
    }

}