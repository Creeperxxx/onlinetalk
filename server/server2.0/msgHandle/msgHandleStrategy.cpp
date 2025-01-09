#include "msgHandleStrategy.h"

// std::shared_ptr<message> controlUserLoginStrategy::handle(std::shared_ptr<message> msg)
// {
//     //response json:
//     // { "login_status": "success"};

//     //login json
//     //{ "logintype": "username","id": "123456", "password": "123456"}
//     dataHeader header(messageType::Notice, messageAction::USER_LOGIN_STATUS, std::nullopt, msg->getHeader().getSenderName(), msg->getHeader().getSenderId());
//     nlohmann::json response;
//     // header.setType(messageType::Notice);
//     // header.setAction(messageAction::USER_LOGIN_STATUS);
//     // header.setReceiverId(msg->getHeader().getSenderId());
//     // header.setReceiverName(msg->getHeader().getSenderName());

//     // 使用构造函数来进行vector<uint8_t>和string之间的转换

//     std::string data_str = std::string(msg->getData().begin(), msg->getData().end());
//     try
//     {
//         nlohmann::json data = nlohmann::json::parse(data_str);
//         std::string login_mode_str = data[JSON_LOGIN_MODE];
//         std::string user_account;
//         loginType login_type_enum;
//         if(login_mode_str == JSON_LOGIN_MODE_USERNAME)
//         {
//             user_account = data[JSON_LOGIN_USERNAME];
//             login_type_enum = loginType::USERNAME;
//         }
//         else if(login_mode_str == JSON_LOGIN_MODE_ID)
//         {
//             user_account = data[JSON_LOGIN_MODE_ID];
//             login_type_enum = loginType::USERID;
//         }
//         std::string user_passwd = data[JSON_LOGIN_PASSWORD];
//         // std::string user_id = data[USER_ID_FIELD];
//         // std::string user_passwd = data[USER_PASSWD_FIELD];

//         // std::string user_info = databaseV1::getInstance().get_user_info(user_id);
//         std::string user_info = databaseV2::get_instance().get_user_info(user_account, login_type_enum);
//         if (user_info.empty())
//         {
//             LOG_ERROR("%s:%s:%d // 数据库没有找到用户信息", __FILE__, __FUNCTION__, __LINE__);

//             response[JSON_LOGIN_STATUS] = JSON_LOGIN_STATUS_FAILED;
//             // return nullptr;
//         }
//         else
//         {
//             nlohmann::json user_info_json = nlohmann::json::parse(user_info);

//             if (user_passwd == user_info_json[MYSQL_TABLE_USER_FIELD_PASSWD])
//             {
//                 //这里需要用到socketmanager，有两种方案：
//                 //  1. 将socketmanager设计为单例模式
//                 //  2. 将socketmanager作为接口类成员属性。
//                 //最终选单例模式了

//                 oldSocketManager::getInstance().add_socket_vec()

//                 //到socketmanager中看是否已登录，如果没有则加入进去

//                 LOG_INFO("%s:%s:%d // 用户登录成功", __FILE__, __FUNCTION__, __LINE__);
//                 response[JSON_FIELD_LOGIN_STATUS]= JSON_FIELD_LOGIN_STATUS_SUCCESS;
//                 int socket = msg->getHeader().getSenderSocketFd();
//                 if(socket == 0)
//                 {
//                     LOG_ERROR("%s:%s:%d // socket为空", __FILE__, __FUNCTION__, __LINE__);
//                 }
//                 else
//                 {
//                     //将用户名到套接字的映射存储起来，
//                 }

//                 // return nullptr;
//             }
//             else
//             {
//                 LOG_INFO("%s:%s:%d // %s用户登录失败,登录方式%s", __FILE__, __FUNCTION__, __LINE__,user_account,login_mode_str);
//                 // response[JSON_FIELD_LOGIN_STATUS]= JSON_FIELD_LOGIN_STATUS_FAIL;
//                 response[JSON_LOGIN_STATUS] = JSON_LOGIN_STATUS_FAILED;
//             }
//         }

//         std::string temp_str = response.dump();
//         std::vector<uint8_t> temp(temp_str.begin(), temp_str.end());
//         return std::make_shared<message>(header, temp);

//     }
//     catch (nlohmann::json::parse_error &e)
//     {
//         LOG_ERROR("%s:%s:%d // json解析错误，具体为：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
//         return nullptr;
//     }
// }

nlohmann::json controlUserRegistryStrategy::handle(nlohmann::json msg)
{
    try
    {
        nlohmann::json response = IControlStrategy::construct_a_return_message(msg);
        std::string user_name = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_USER_NAME);
        std::string user_passwd = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_USER_PASSWD);
        std::string user_email = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_USER_EMAIl);

        std::string register_status;
        uint64_t ret_id;

        if (database::get_instance().is_user_exist_in_db(user_name, user_email) == true)
        {
            // 注册失败
            register_status = MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS_FAILURE;
        }
        else
        {
            // 可以注册
            // 1. 加入到数据库中
            // 2. 获取返回的主键放入返回消息中
            ret_id = database::get_instance().add_user_to_db(user_name, user_passwd, user_email);
            response[MESSAGE_KEY_DATA][MESSAGE_KEY_DATA_CONTENT][MESSAGE_KEY_DATA_CONTENT_REGISTER_RET_USERID] = ret_id;

            if (ret_id > 0)
            {
                register_status = MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS_SUCCESS;
            }
            else
            {
                register_status = MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS_FAILURE;
            }
        }
        response[MESSAGE_KEY_DATA][MESSAGE_KEY_DATA_CONTENT][MESSAGE_KEY_DATA_CONTENT_REGISTER_STATUS] = register_status;

        response[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_ACTION] = MESSAGE_ACTION_NOTICE_USER_REGISTER_RESULT;

        return response;
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json解析错误，具体为：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
        return IStrategy::construct_error_response(msg, e.what());
    }
}

nlohmann::json controlUserLoginStrategy::handle(nlohmann::json msg)
{
    try
    {
        std::string login_type = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE);
        // std::string login_user_name;
        // std::string login_user_id;
        std::string account;
        std::string login_user_passwd = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_LOGIN_PASSWD);
        if (login_type == MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_ID)
        {
            account = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_LOGIN_ID);
        }
        else if (login_type == MESSAGE_KEY_DATA_CONTENT_LOGIN_TYPE_NAME)
        {
            account = msg.at(MESSAGE_KEY_DATA).at(MESSAGE_KEY_DATA_CONTENT).at(MESSAGE_KEY_DATA_CONTENT_LOGIN_NAME);
        }
        else
        {
            LOG_ERROR("%s:%s:%d // 登录方式错误", __FILE__, __FUNCTION__, __LINE__);
            return IStrategy::construct_error_response(msg, "登录方式错误");
        }
        if(database::get_instance().get_login_result(login_type,account,login_user_passwd) == true)
        {
            //判断是否登录
            //1. 先到服务器临时存储登录信息中找，然后到redis中是否登录
            //2. 如果未登录，则允许登录，然后将离线消息发送给他
            
        }
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // json解析错误，具体为：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
        return IStrategy::construct_error_response(msg, e.what());
    }
}

nlohmann::json IControlStrategy::construct_a_return_message(nlohmann::json msg)
{
    nlohmann::json ret;
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_RECEIVER_ID] = msg.at(MESSAGE_KEY_HEADER).at(MESSAGE_KEY_HEADER_SENDER_ID);
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_RECEIVER_NAME] = msg.at(MESSAGE_KEY_HEADER).at(MESSAGE_KEY_HEADER_SENDER_NAME);
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SENDER_ID] = MESSAGE_HEADER_SENDER_ID_SERVER;
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SENDER_NAME] = MESSAGE_HEADER_SENDER_NAME_SERVER;
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_TYPE] = MESSAGE_TYPE_NOTICE;
    return ret;
}

nlohmann::json IStrategy::construct_error_response(nlohmann::json msg, const std::string &error)
{
    nlohmann::json ret;
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SENDER_ID] = MESSAGE_HEADER_SENDER_ID_SERVER;
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SENDER_NAME] = MESSAGE_HEADER_SENDER_NAME_SERVER;
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_RECEIVER_ID] = msg.at(MESSAGE_KEY_HEADER).at(MESSAGE_KEY_HEADER_SENDER_ID);
    ret[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_RECEIVER_NAME] = msg.at(MESSAGE_KEY_HEADER).at(MESSAGE_KEY_HEADER_SENDER_NAME);
    ret[MESSAGE_KEY_DATA][MESSAGE_KEY_DATA_ERROR] = error;
    return ret;
}