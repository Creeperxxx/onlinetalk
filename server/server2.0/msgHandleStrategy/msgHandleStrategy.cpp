#include "msgHandleStrategy.h"

std::shared_ptr<message> controlUserLoginStrategy::handle(std::shared_ptr<message> msg)
{
    //response json:
    // { "login_status": "success"};

    //login json
    //{ "logintype": "username","id": "123456", "password": "123456"}
    dataHeader header(messageType::Notice, messageAction::USER_LOGIN_STATUS, std::nullopt, msg->getHeader().getSenderName(), msg->getHeader().getSenderId());
    nlohmann::json response;
    // header.setType(messageType::Notice);
    // header.setAction(messageAction::USER_LOGIN_STATUS);
    // header.setReceiverId(msg->getHeader().getSenderId());
    // header.setReceiverName(msg->getHeader().getSenderName());

    // 使用构造函数来进行vector<uint8_t>和string之间的转换

    std::string data_str = std::string(msg->getData().begin(), msg->getData().end());
    try
    {
        nlohmann::json data = nlohmann::json::parse(data_str);
        std::string login_mode_str = data[JSON_LOGIN_MODE];
        std::string user_account;
        loginType login_type_enum;
        if(login_mode_str == JSON_LOGIN_MODE_USERNAME)
        {
            user_account = data[JSON_LOGIN_USERNAME]; 
            login_type_enum = loginType::USERNAME;
        }
        else if(login_mode_str == JSON_LOGIN_MODE_ID)
        {
            user_account = data[JSON_LOGIN_MODE_ID];
            login_type_enum = loginType::USERID;
        }
        std::string user_passwd = data[JSON_LOGIN_PASSWORD];
        // std::string user_id = data[USER_ID_FIELD];
        // std::string user_passwd = data[USER_PASSWD_FIELD];

        // std::string user_info = databaseV1::getInstance().get_user_info(user_id);
        std::string user_info = databaseV2::get_instance().get_user_info(user_account, login_type_enum);
        if (user_info.empty())
        {
            LOG_ERROR("%s:%s:%d // 数据库没有找到用户信息", __FILE__, __FUNCTION__, __LINE__);

            response[JSON_LOGIN_STATUS] = JSON_LOGIN_STATUS_FAILED;
            // return nullptr;
        }
        else
        {
            nlohmann::json user_info_json = nlohmann::json::parse(user_info);

            if (user_passwd == user_info_json[USER_PASSWD_FIELD])
            {
                //这里需要用到socketmanager，有两种方案：
                //  1. 将socketmanager设计为单例模式
                //  2. 将socketmanager作为接口类成员属性。
                //最终选单例模式了

                socketManager::getInstance().add_socket_vec()

                //到socketmanager中看是否已登录，如果没有则加入进去

                LOG_INFO("%s:%s:%d // 用户登录成功", __FILE__, __FUNCTION__, __LINE__);
                response[JSON_FIELD_LOGIN_STATUS]= JSON_FIELD_LOGIN_STATUS_SUCCESS;
                int socket = msg->getHeader().getSenderSocketFd();
                if(socket == 0)
                {
                    LOG_ERROR("%s:%s:%d // socket为空", __FILE__, __FUNCTION__, __LINE__);
                }
                else
                {
                    //将用户名到套接字的映射存储起来，
                }

                // return nullptr;
            }
            else
            {
                LOG_INFO("%s:%s:%d // %s用户登录失败,登录方式%s", __FILE__, __FUNCTION__, __LINE__,user_account,login_mode_str);
                // response[JSON_FIELD_LOGIN_STATUS]= JSON_FIELD_LOGIN_STATUS_FAIL;
                response[JSON_LOGIN_STATUS] = JSON_LOGIN_STATUS_FAILED;
            }
        }

        std::string temp_str = response.dump();
        std::vector<uint8_t> temp(temp_str.begin(), temp_str.end());
        return std::make_shared<message>(header, temp);

    }
    catch (nlohmann::json::parse_error &e)
    {
        LOG_ERROR("%s:%s:%d // json解析错误，具体为：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
        return nullptr;
    }
}