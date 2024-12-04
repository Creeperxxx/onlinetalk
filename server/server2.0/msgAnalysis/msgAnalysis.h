#pragma once
// #include <nlohmann/json.hpp>
#include "../msgHandleStrategy/msgHandleStrategy.h"
#include <unordered_map>


// using json = nlohmann::json;

// enum msgType
// {
//     USER,
//     FRIEND,
//     GROUP,
//     NOTIFY,
//     FILETRANPORT
// };

// enum msgAction
// {
//     LOGIN,
//     LOGOUT,
//     REGISTER,
//     DEREGISTER,
//     MODY_PER_INFO,
//     RESET_PASSWORD,
//     CHECK_PER_INFO,
    
//     ADD_FRIEND,
//     DEAL_FRIEND_REQUEST,
//     DELETE_FRIEND,
//     SEARCH_FRIEND,
//     SEND_MESSAGE,
//     CHECK_FRIEND_STATUS,
//     SHIELD_FRIEND,
//     CHECK_FRIEND_LISTS,

//     CREATE_GROUP,
//     JOIN_GROUP,
//     QUIT_GROUP,
//     DELETE_GROUP,
//     INVITE_TO_JOIN_GROUP,
//     KICK_OFF_GROUP,
//     SET_MANAGET,
//     MODI_GROUP_INFO,
//     UPDATE_GROUP_NOTICE,
//     CHECK_MEMBER_LIST,
//     CHECK_HISTORY_MESSAGE,

//     MSG_STATUS_NOTICE,
    
//     UPLOAD_FILE,
//     DOWNLOAD_FILE,

// };

class strategy_registry;
//思路：
//x 使用策略模式加抽象工厂模式，每种消息下的每个动作都专门设计一个处理策略。然后由抽象工厂来生产策略。
//√ 使用注册表加策略模式，每种消息下的每个动作都专门设计一个处理策略。将策略注册到注册表中。
class msg_analysis
{
public:
    void init();
    std::shared_ptr<message> handle(std::shared_ptr<message> msg);
private:
    void text_init();
    void image_init();
    void file_init();
    void voice_init();
    void video_init();
    void control_init();
    void notice_init();
private:
    std::shared_ptr<strategy_registry> strategy_registry_;
};

class strategy_registry
{
public:
    template <typename T>
    void register_strategy(messageType type,messageAction action);
    std::shared_ptr<IStrategy> get_strategy(messageType type,messageAction action);
private:
    std::unordered_map<messageType,std::unordered_map<messageAction,std::shared_ptr<IStrategy>>> registry;
};





template <typename T>
void strategy_registry::register_strategy(messageType type,messageAction action)
{
    auto strategy = std::make_shared<T>();
    registry[type][action] = strategy;
}



