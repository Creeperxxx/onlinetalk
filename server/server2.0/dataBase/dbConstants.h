#pragma once
#include <string>


//登录
enum class loginType
{
    USERNAME,
    USERID
};
inline const std::string JSON_LOGIN_MODE = "login_mode"; 
inline const std::string JSON_LOGIN_MODE_USERNAME = "username";
inline const std::string JSON_LOGIN_MODE_ID = "id";
inline const std::string JSON_LOGIN_USERNAME = "username";
inline const std::string JSON_LOGIN_PASSWORD = "password";
inline const std::string JSON_LOGIN_ID = "id";
//response json
inline const std::string JSON_LOGIN_STATUS = "login_status";
inline const std::string JSON_LOGIN_STATUS_SUCCESS = "success";
inline const std::string JSON_LOGIN_STATUS_FAILED = "failed";

/*
{
    "login_mode" : "username" || "id",
    "username" : "username",
    "id" : "id",
    "password" : "password"
}
{
    "login_status" : "success" || "failed"

}
*/

//redis
inline const std::string REDIS_POOL_ADDRESS = "127.0.0.1";
inline const int REDIS_POOL_PORT = 6379;
inline const size_t REDIS_POOL_SIZE = 10;

inline const std::string REDIS_PRIEFIX_FIND_USERINFO = "user:user_info:";
inline const std::string REDIS_PRIEFIX_FIND_USERID = "user:find_userid_by_username:";

inline const int REDIS_EXPIRE_USERINFO = 60*60;

enum class redisSetMode
{
    NX,//键不存在则设置，存在则不设置
    XX//键存在则设置，不存在则不设置
};

//mysql
enum class statementType
{
    QUERY,
    NOTQUERY
};

inline const std::string MYSQL_ADDRESS = "127.0.0.1";
inline const size_t  MYSQL_POOL_SIZE = 10;
inline const std::string MYSQL_USER = "root";
inline const std::string MYSQL_PASSWORD = "";
inline const std::string MYSQL_DATABASE= "onlinechat";

inline const std::string MYSQL_TABLE = "users";
inline const std::string USER_ID_FIELD = "id";
inline const std::string USER_NAME_FIELD = "username";
inline const std::string USER_PASSWD_FIELD = "password";
inline const std::string USER_EMAIL_FIELD = "email";

//message
inline const std::string DEFAULT_SENDER_NAME = "default_sender_name";
inline const int DEFAULT_SENDER_ID = 0;
inline const int DEFAULT_SESSION_ID = 0;
inline const int DEFAULT_GROUP_ID = 0;
inline const int DEFAULT_RECEIVER_ID = 0;
inline const std::string DEFAULT_RECEIVER_NAME = "default_receiver_name";