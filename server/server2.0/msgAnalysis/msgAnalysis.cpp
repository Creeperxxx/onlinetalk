#include "msgAnalysis.h"

std::shared_ptr<IStrategy> strategy_registry::get_strategy(msgType type,msgAction action)
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