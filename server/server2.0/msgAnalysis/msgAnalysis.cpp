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

void msgAnalysis::text_init()
{
    strategy_registry_->register_strategy<user_send_user>(messageType::Text, messageAction::USER_SEND_USER);
}

void msgAnalysis::image_init()
{
}

void msgAnalysis::file_init()
{
}

void msgAnalysis::voice_init()
{
}

void msgAnalysis::video_init()
{
}

void msgAnalysis::control_init()
{
}
void msgAnalysis::notice_init()
{
}

std::shared_ptr<message> msgAnalysis::handle(std::shared_ptr<message> msg)
{
    auto strategy = strategy_registry_->get_strategy(msg->getHeader().getType(), msg->getHeader().getAction());
    if (strategy)
    {
        return strategy->handle(msg);
    }
    else
    {
        return nullptr;
    }
}

void msgAnalysis::init()
{
    strategy_registry_ = std::make_unique<strategy_registry>();
    
}