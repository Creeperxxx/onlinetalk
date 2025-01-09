// #include "msgAnalysis.h"
#include "msgHandle.h"

// std::shared_ptr<IStrategy> strategy_registry::get_strategy(messageType type, messageAction action)
// {
//     auto it = registry.find(type);
//     if (it != registry.end())
//     {
//         auto it2 = it->second.find(action);
//         if (it2 != it->second.end())
//         {
//             return it2->second;
//         }
//     }
//     perror("no strategy");
//     return nullptr;
// }

void msgHandler::text_init()
{
    // strategy_registry_->register_strategy<user_send_user>(messageType::Text, messageAction::USER_SEND_USER);
}

void msgHandler::image_init()
{
}

void msgHandler::file_init()
{
}

void msgHandler::voice_init()
{
}

void msgHandler::video_init()
{
}

void msgHandler::control_init()
{
    std::shared_ptr<IStrategy> s = std::make_shared<controlUserRegistryStrategy>();
    strategy_registry_->register_strategy(MESSAGE_TYPE_CONTROL,MESSAGE_ACTION_REGISTER,s);
}
void msgHandler::notice_init()
{
}

// std::shared_ptr<message> msgHandler::handle(std::shared_ptr<message> msg)
// {
//     auto strategy = strategy_registry_->get_strategy(msg->getHeader().getType(), msg->getHeader().getAction());
//     if (strategy)
//     {
//         return strategy->handle(msg);
//     }
//     else
//     {
//         return nullptr;
//     }
// }

void msgHandler::init()
{
    strategy_registry_ = std::make_unique<strategy_registry>();
}

// void msgHandler::handle(std::shared_ptr<socketData> data)
// {
//     auto recv_data = data->pop_recv_data();

// }

void msgHandler::handle(std::unique_ptr<message> msg)
{
    auto msg_str = get_value_from_msg(std::move(msg));
    handle_msg(std::move(msg_str));
}

// std::unique_ptr<std::string> msgHandler::get_value_from_msg(std::unique_ptr<message> msg)
nlohmann::json msgHandler::get_value_from_msg(std::unique_ptr<message> msg)
{
    nlohmann::json json_msg;
    dataHeader h = msg->getHeader();
    json_msg[MESSAGE_KEY_DATA] = std::string(msg->getData().begin(), msg->getData().end());
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_TYPE] = dataHeader::to_string(h.getType());
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_ACTION] = dataHeader::to_string(h.getAction());
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SENDER_NAME] = h.getSenderName();
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SENDER_ID] = h.getSenderId();
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_RECEIVER_NAME] = h.getReceiverName();
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_RECEIVER_ID] = h.getReceiverId();
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_SESSION_ID] = h.getSessionId();
    json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_GROUP_ID] = h.getGroupId();
    if (h.getIsCompressed() == true)
    {
        json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_IS_COMPRESSED] = "true";
    }
    else
    {
        json_msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_IS_COMPRESSED] = "false";
    }
    // return std::move(std::make_unique<std::string>(json_msg.dump()));
    return json_msg;
}

// std::string msgHandler::get_value_from_msg(std::unique_ptr<std::string> msg)
// {
//     nlohmann::json json_msg = nlohmann::json::parse(*msg);

// }

// void msgHandler::handle_msg(std::unique_ptr<std::string> msg)
void msgHandler::handle_msg(nlohmann::json msg)
{
    if (msg.contains(MESSAGE_KEY_HEADER))
    {
        if (msg[MESSAGE_KEY_HEADER].contains(MESSAGE_KEY_HEADER_TYPE) == true && msg[MESSAGE_KEY_HEADER].contains(MESSAGE_KEY_HEADER_ACTION) == true)
        {
            auto sta = strategy_registry_->get_strategy(msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_TYPE], msg[MESSAGE_KEY_HEADER][MESSAGE_KEY_HEADER_ACTION]);
            if(sta != nullptr)
            {
                sta->handle(msg);
            }
        }
        else
        {
            LOG_ERROR("%s:%s:%d // 消息中不包含消息类型或消息动作", __FILE__, __FUNCTION__, __LINE__);
        }
    }
    else
    {
        LOG_ERROR("%s:%s:%d // 消息中不包含头信息", __FILE__, __FUNCTION__, __LINE__);
        return;
    }
}
void msgHandler::handle(std::unique_ptr<std::string> msg)
{
    handle_msg(nlohmann::json::parse(*msg));
}

void strategy_registry::register_strategy(std::string type, std::string action, std::shared_ptr<IStrategy> strategy)
{
    // registry[type] = std::make_pair(action,strategy);
    registry[type]->emplace(action, strategy);
}

std::shared_ptr<IStrategy> strategy_registry::get_strategy(std::string type, std::string action)
{
    try
    {
        return registry.at(type)->at(action);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // 找不到对应的策略", __FILE__, __FUNCTION__, __LINE__);
        return nullptr;
    }
}