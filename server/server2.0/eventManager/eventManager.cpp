#include "eventManager.h"

// eventManager* eventManager::register_callback(std::string event_name,std::any callback)
void eventManager::register_callback(std::string event_name,std::any callback)
{
    if( callback.has_value())
    {
        if(data.find(event_name) != data.end())
        {
            LOG_WARING("%s:%s:%d // 事件已经注册 ",__FILE__,__FUNCTION__,__LINE__);
        }
        data[event_name] = callback;
    }
    else
    {
        LOG_WARING("%s:%s:%d // 回调函数为空 ",__FILE__,__FUNCTION__,__LINE__);
        // return this;
    }
}

std::any eventManager::get_callback(std::string event_name)
{
    if(data.find(event_name)!= data.end())
    {
        return data[event_name];
    }
    else
    {
        LOG_WARING("%s:%s:%d // 事件未注册 ",__FILE__,__FUNCTION__,__LINE__);
        return std::any();
    }
}