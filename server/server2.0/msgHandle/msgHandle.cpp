#include "msgAnalysis.h"

std::shared_ptr<IStrategy> strategy_registry::get_strategy(messageType type, messageAction action)
{
    auto it = registry.find(type);
    if (it != registry.end())
    {
        auto it2 = it->second.find(action);
        if (it2 != it->second.end())
        {
            return it2->second;
        }
    }
    perror("no strategy");
    return nullptr;
}

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

void msgHandler::handle(std::shared_ptr<socketData> data)
{
    auto recv_data = data->pop_recv_data();
    
}