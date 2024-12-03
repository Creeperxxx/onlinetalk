#include "msgAnalysis.h"

std::shared_ptr<IStrategy> strategy_registry::get_strategy(messageType type,messageAction action)
{
    auto it = registry.find(type);
    if(it!= registry.end())
    {
        auto it2 = it->second.find(action);
        if(it2!=it->second.end())
        {
            return it2->second;
        }
    }
    perror("no strategy");
    return nullptr;
}

void msg_analysis::text_init()
{
    strategy_registry_->register_strategy<user_send_user>(messageType::Text,messageAction::USER_SEND_USER);
}

void msg_analysis::image_init()
{

}

void msg_analysis::file_init()
{

}

void msg_analysis::voice_init()
{

}

void msg_analysis::video_init()
{

}

void msg_analysis::control_init()
{

}
void msg_analysis::notice_init()
{

}

std::shared_ptr<message> msg_analysis::handle(std::shared_ptr<message> msg)
{
    auto strategy = strategy_registry_->get_strategy(msg->getHeader().getType(),msg->getHeader().getAction());
    if(strategy)
    {
        return strategy->handle(msg);
    }
    else
    {
        return nullptr;
    }
}