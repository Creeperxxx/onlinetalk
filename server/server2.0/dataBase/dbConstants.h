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
inline const std::string REDIS_JSON_FIELD_STREAMNAME = "stream_name";
inline const std::string REDIS_JSON_FIELD_MESSAGEID = "msg_id";

inline const int REDIS_EXPIRE_USERINFO = 60*60;

inline const int REDIS_STREAM_XREADGROUP_SLEEPTIME = 1000; //redis stream xreadgroup命令如果没有读到数据则默认sleep的时间
inline const int REDIS_STREAM_XREADGROUP_BLOCK = 1000;  //redis stream xreadgroup命令默认阻塞时间
inline const int REDIS_STREAM_XREADGROUP_COUNT = 1; //redis stream xreadgroup命令默认返回的消息数量
inline const std::string REDIS_STREAM_STREAMNAME_SERVER = "server";
inline const std::string REDIS_STREAM_GROUPNAME_SERVER= "mygroup";
inline const int REDIS_STREAM_XACK_FAILED_SLEEPTIME = 1000; //redis stream xack命令失败后sleep的时间
inline const int REDIS_STREAM_XACK_FAILED_MAXRETRY = 10; //redis stream xack命令失败后最大重试次数



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
inline const std::string MYSQL_DATABASE= "chat";

    //mysql user表
inline const std::string MYSQL_TABLE_USERS = "users";
inline const std::string MYSQL_TABLE_USER_FIELD_ID = "user_id";
inline const std::string MYSQL_TABLE_USER_FIELD_NAME = "user_name";
inline const std::string MYSQL_TABLE_USER_FIELD_PASSWD = "user_passwd";
inline const std::string MYSQL_TABLE_USER_FIELD_EMAIL = "user_email";
    //mysql messages表
inline const std::string MYSQL_TABLE_MESSAGES = "messages";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_ID = "message_id";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_SENDER_ID = "sender_id";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_RECEIVER_ID = "receiver_id";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_SENDER_NAME = "sender_name";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_RECEIVER_NAME = "receiver_name";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_CONTENT = "content";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_MESSAGETYPE = "message_type";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_ISDELIVERED = "is_delivered";
inline const std::string MYSQL_TABLE_MESSAGES_FIELD_CREATEDAT = "created_at";

//message
inline const std::string DEFAULT_SENDER_NAME = "default_sender_name";
inline const int DEFAULT_SENDER_ID = 0;
inline const int DEFAULT_SESSION_ID = 0;
inline const int DEFAULT_GROUP_ID = 0;
inline const int DEFAULT_RECEIVER_ID = 0;
inline const std::string DEFAULT_RECEIVER_NAME = "default_receiver_name";