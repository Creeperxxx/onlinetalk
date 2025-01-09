#pragma once
#include <string>


//登录
enum class loginType
{
    USERNAME,
    USERID
};
// inline const std::string JSON_LOGIN_MODE = "login_mode"; 
// inline const std::string JSON_LOGIN_MODE_USERNAME = "username";
// inline const std::string JSON_LOGIN_MODE_ID = "id";
// inline const std::string JSON_LOGIN_USERNAME = "username";
// inline const std::string JSON_LOGIN_PASSWORD = "password";
// inline const std::string JSON_LOGIN_ID = "id";   
// //response json
// inline const std::string JSON_LOGIN_STATUS = "login_status";
// inline const std::string JSON_LOGIN_STATUS_SUCCESS = "success";
// inline const std::string JSON_LOGIN_STATUS_FAILED = "failed";

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

// inline const std::string REDIS_PRIEFIX_FIND_USERINFO = "user:user_info:";
// inline const std::string REDIS_PRIEFIX_FIND_USERID = "user:find_userid_by_username:";
// inline const std::string REDIS_JSON_FIELD_STREAMNAME = "stream_name";
// inline const std::string REDIS_JSON_FIELD_MESSAGEID = "msg_id";

inline const int REDIS_EXPIRE_USERINFO = 60*60;
    //redis stream

inline const int REDIS_STREAM_XREADGROUP_SLEEPTIME = 1000; //redis stream xreadgroup命令如果没有读到数据则默认sleep的时间
inline const int REDIS_STREAM_XREADGROUP_BLOCK = 1000;  //redis stream xreadgroup命令默认阻塞时间
inline const int REDIS_STREAM_XREADGROUP_COUNT = 1; //redis stream xreadgroup命令默认返回的消息数量
inline const std::string REDIS_STREAM_STREAMNAME_SERVER = "server";
inline const std::string REDIS_STREAM_GROUPNAME_SERVER= "mygroup";
inline const std::string REDIS_STREAM_CONSUMERNAME_SERVER = "consumer";
inline const int REDIS_STREAM_XACK_FAILED_SLEEPTIME = 1000; //redis stream xack命令失败后sleep的时间
inline const int REDIS_STREAM_XACK_FAILED_MAXRETRY = 10; //redis stream xack命令失败后最大重试次数
inline const int REDIS_STREAM_READER_NUM = 1;
inline const int REDIS_STREAM_WRITER_NUM = 1;
inline const std::string REDIS_STREAM_MESSAGE_FIELD_NAME = "message";
inline const std::string REDIS_STREAM_REGISTERNAME_PREFIX = "register";




enum class redisSetMode
{
    NX,//键不存在则设置，存在则不设置
    XX//键存在则设置，不存在则不设置
};

//mysql
enum class statementType
{
    QUERY,
    INSERT,
    OTHER
};

inline const std::string MYSQL_ADDRESS = "127.0.0.1";
inline const size_t  MYSQL_POOL_SIZE = 10;
inline const std::string MYSQL_USER = "root";
inline const std::string MYSQL_PASSWORD = "";
inline const std::string MYSQL_DATABASE= "chat";
inline const std::string MYSQL_GET_LAST_INSERT_ID = "SELECT LAST_INSERT_ID()";

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
// inline const std::string DEFAULT_SENDER_NAME = "default_sender_name";
// inline const int DEFAULT_SENDER_ID = 0;
// inline const int DEFAULT_SESSION_ID = 0;
// inline const int DEFAULT_GROUP_ID = 0;
// inline const int DEFAULT_RECEIVER_ID = 0;
// inline const std::string DEFAULT_RECEIVER_NAME = "default_receiver_name";
inline const std::string MESSAGE_HEADER_SENDER_ID_DEFAULT = "default_sender_id";
inline const std::string MESSAGE_HEADER_SENDER_NAME_DEFAULT = "default_sender_name";
inline const std::string MESSAGE_HEADER_RECEIVER_NAME_DEAFULT = "default_receiver_name";
inline const std::string MESSAGE_HEADER_RECEIVER_ID_DEFAULT = "default_receiver_id";
inline const std::string MESSAGE_HEADER_SESSION_ID_DEFAULT = "default_session_id";
inline const std::string MESSAGE_HEADER_GROUP_ID_DEFAULT = "default_group_id";
inline const bool MESSAGE_HEADER_IS_COMPRESSED_DEFAULT = false;
inline const int MESSAGE_HEADER_SENDER_SOCKET_FD_DEFAULT = -1;
inline const std::string MESSAGE_HEADER_SENDER_ID_SERVER = "server_id";
inline const std::string MESSAGE_HEADER_SENDER_NAME_SERVER = "server_name";


inline const std::string MESSAGE_KEY_HEADER_SENDER_ID = "sender_id";
inline const std::string MESSAGE_KEY_HEADER_SENDER_NAME = "sender_name";
inline const std::string MESSAGE_KEY_HEADER_RECEIVER_ID = "receiver_id";
inline const std::string MESSAGE_KEY_HEADER_RECEIVER_NAME = "receiver_name";
inline const std::string MESSAGE_KEY_HEADER = "header";
inline const std::string MESSAGE_KEY_DATA = "data";
inline const std::string MESSAGE_KEY_HEADER_REGISTER_TOKEN = "register_token";
inline const std::string MESSAGE_KEY_HEADER_RECEIVER_STREAM = "receiver_stream";
inline const std::string MESSAGE_KEY_HEADER_TYPE = "message_type";
inline const std::string MESSAGE_KEY_HEADER_ACTION = "message_action";
inline const std::string MESSAGE_KEY_HEADER_SESSION_ID = "session_id";
inline const std::string MESSAGE_KEY_HEADER_GROUP_ID = "group_id";
inline const std::string MESSAGE_KEY_HEADER_IS_COMPRESSED = "is_compressed";
inline const std::string MESSAGE_KEY_DATA_CONTENT = "content";
inline const std::string MESSAGE_KEY_DATA_CONTENT_USER_NAME = MYSQL_TABLE_USER_FIELD_NAME;
inline const std::string MESSAGE_KEY_DATA_CONTENT_USER_PASSWD = MYSQL_TABLE_USER_FIELD_PASSWD;
inline const std::string MESSAGE_KEY_DATA_CONTENT_USER_EMAIl = MYSQL_TABLE_USER_FIELD_EMAIL;
inline const std::string MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS = "login_status";
inline const std::string MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS_SUCCESS = "success";
inline const std::string MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS_FAILURE = "failure";
inline const std::string MESSAGE_KEY_DATA_CONTENT_REGISTER_RET_USERID = "register_userid";
inline const std::string MESSAGE_KEY_DATA_ERROR = "error";
inline const std::string MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE = "login_type";
inline const std::string MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_ID = "id";
inline const std::string MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_NAME = "name";
inline const std::string MESSAGE_KEY_DATA_CONTENT_LOGIN_PASSWD = "user_passwd";
inline const std::string MESSAGE_KEY_DATA_CONTENT_LOGIN_ID = "user_id";
inline const std::string MESSAGE_KEY_DATA_CONTENT_LOGIN_NAME = "user_name";

inline const std::string MESSAGE_TYPE_TEXT = "text";
inline const std::string MESSAGE_TYPE_FILE = "file";
inline const std::string MESSAGE_TYPE_IMAGE = "image";
inline const std::string MESSAGE_TYPE_VOICE = "voice";
inline const std::string MESSAGE_TYPE_VIDEO = "video";
inline const std::string MESSAGE_TYPE_CONTROL = "control";
inline const std::string MESSAGE_TYPE_NOTICE = "notice";
inline const std::string MESSAGE_TYPE_NONE = "none_type";

inline const std::string MESSAGE_ACTION_NONE = "none_action";
inline const std::string MESSAGE_ACTION_NOTICE_USER_REGISTER_RESULT = "user_register_result";
inline const std::string MESSAGE_ACTION_REGISTER = "register";

/*
{
    "header" : {
        "sender_id" : "sender_id",
        "sender_name" : "sender_name",
        "receiver_id" : "receiver_id",
        "receiver_name" : "receiver_name",
        "receiver_stream" : "receiver_stream",
        "message_type" : "message_type",
        "message_action" : "message_action",
        "session_id" : "session_id",
        "group_id" : "group_id"
    },
    "data" : {
        "content" : {

        }
        "error" :
    }
}
*/
//content:


/*
register msg:
{
    user_name : "user_name",
    user_passwd : "user_passwd",
    user_email : "user_email",
}
*/

/*
register response:
{
    "login_status" : "failure" || "success",
    "register_userid" : 
} 
*/
/*
login:
{
    "login_type" : id || name,
    "user_name" : "user_name,
    "user_id" : "user_id",
    "user_passwd" : "user_passwd",
}
*/