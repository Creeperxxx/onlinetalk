#pragma once
#include <string>
#include <variant>
#include <optional>
#include <vector>
#include "../dataBase/dbConstants.h"

enum class messageType
{
    Text,
    Image,
    File,
    Voice,
    Video,
    Control,
    Notice,
};

// enum class textAction
// {
//     USER_SNED_USER = 0,
//     USER_RECIEVE_USER,
//     USER_SEND_GROUP,
//     USER_RECIEVE_GROUP
// };

// enum class imageAction
// {
//     USER_SEND_IMAGE = 100,
//     USER_RECIEVE_IMAGE
// };

// enum class fileAction
// {
//     USER_SEND_FILE = 200,
//     USER_RECIEVE_FILE
// };

// enum class voiceAction
// {
//     USER_SEND_VOICE = 300,
//     USER_RECIEVE_VOICE
// };

// enum class videoAction
// {
//     USER_SEND_VIDEO = 400,
//     USER_RECIEVE_VIDEO
// };

// enum class controlAction
// {
//     USER_LOGIN = 500,
//     USER_LOGOUT,
//     USER_REGISTER,
//     USER_ADD_FRIEND,
//     USER_DELETE_FRIEND
// };

// enum class noticeAction
// {
//     USER_LOGIN_STATUS = 600,
//     USER_ONLINE_STATUS,
//     FRIEND_ONLINE_STATUS,
// };

enum class messageAction
{
    USER_SEND_USER = 0,
    USER_RECEIVE_USER,
    USER_SEND_GROUP,
    USER_RECEIVE_GROUP,

    USER_SEND_IMAGE,
    USER_RECEIVE_IMAGE,

    USER_SEND_FILE,
    USER_RECEIVE_FILE,

    USER_SEND_VOICE,
    USER_RECEIVE_VOICE,

    USER_SEND_VIDEO,
    USER_RECEIVE_VIDEO,

    USER_LOGIN,
    USER_LOGOUT,
    USER_REGISTER,
    USER_ADD_FRIEND,
    USER_DELETE_FRIEND,

    USER_LOGIN_STATUS,
    USER_ONLINE_STATUS,
    FRIEND_ONLINE_STATUS,
    HEARTBEAT,
};

class dataHeader
{
public:
    dataHeader() {}
    dataHeader(messageType type,
                  messageAction action,
                  std::optional<std::string> sender_name = std::nullopt,
                  std::optional<std::string> sender_id = std::nullopt, 
                  std::optional<std::string> receiver_name = std::nullopt,
                  std::optional<std::string> receiver_id = std::nullopt,
                  std::optional<std::string> session_id = std::nullopt,
                  std::optional<std::string> group_id = std::nullopt,
                  std::optional<bool> compressed = std::nullopt)
                : m_type(type),
                  m_action(action),
                  m_sender_name(sender_name),
                  m_sender_id(sender_id),
                  m_session_id(session_id),
                  m_group_id(group_id),
                  m_receiver_id(receiver_id),
                  m_receiver_name(receiver_name),
                  m_is_compressed(compressed) {}

public:
    // 提供公共接口以访问私有成员
    const messageType getType() const {
        return m_type; }
    const messageAction getAction() const {
        return m_action; }
    const std::string getSenderName() const {
        return m_sender_name.value_or(MESSAGE_HEADER_SENDER_NAME_DEFAULT); }
    std::string getSenderId() const {
        return m_sender_id.value_or(MESSAGE_HEADER_SENDER_ID_DEFAULT); }
    // uint32_t getMessageSize() const { return m_message_size; }
    std::string getSessionId() const {
        return m_session_id.value_or(MESSAGE_HEADER_SESSION_ID_DEFAULT);}
    const std::string getGroupId() const {
        return m_group_id.value_or(MESSAGE_HEADER_GROUP_ID_DEFAULT); }
    const std::string getReceiverId() const {
        return m_receiver_id.value_or(MESSAGE_HEADER_RECEIVER_ID_DEFAULT); }
    const std::string getReceiverName() const {
        return m_receiver_name.value_or(MESSAGE_HEADER_RECEIVER_NAME_DEAFULT); }
    bool getIsCompressed() const {
        return m_is_compressed.value_or(false); }
    int getSenderSocketFd() const {
        return m_sender_socketfd.value_or(0); }

    // 设置方法
    void setType(messageType type) {
        m_type = type; }
    void setAction(messageAction a) {
        m_action = a; }
    void setSenderName(const std::string& name) {
        m_sender_name = name; }
    void setSenderId(std::string id) {
        m_sender_id = id; }
    // void setMessageSize(uint32_t size) { m_message_size = size; }
    void setSessionId(std::string id) {
        m_session_id = id; }
    void setGroupId(std::string id) {
        m_group_id = id; }
    void setReceiverId(std::string id) {
        m_receiver_id = id; }
    void setReceiverName(const std::string& name) {
        m_receiver_name = name; }
    void setIsCompressed(bool compressed) {
        m_is_compressed = compressed; }
    void setSenderSocketFd(int fd) {
        m_sender_socketfd = fd; }


private:
    messageType m_type;
    messageAction m_action;
    // std::string m_sender_name;
    std::optional<std::string> m_sender_name;
    // int m_sender_id;
    // std::optional<int> m_sender_id; 
    std::optional<std::string> m_sender_id;
    // uint32_t m_message_size;
    // int m_session_id;
    // std::optional<int> m_session_id;
    std::optional<std::string> m_session_id;
    // std::optional<int> m_group_id;
    std::optional<std::string> m_group_id;
    // std::optional<int> m_receiver_id;
    std::optional<std::string> m_group_id;
    std::optional<std::string> m_receiver_name;
    std::optional<std::string> m_receiver_id;
    std::optional<bool> m_is_compressed;
    std::optional<int> m_sender_socketfd;
};

class message
{
private:
    dataHeader header;
    std::vector<uint8_t> data;

public:
    message() {}
    message(dataHeader h, std::vector<uint8_t> d) : header(h), data(d) {}
    // 获取消息头
    const dataHeader &getHeader() const { return header; }
    const std::vector<uint8_t> &getData() const { return data; }

    // 设置数据
    void setData(std::vector<uint8_t> d) { data = d; }
    void setHeader(dataHeader h) { header = h; }
};

// class messageData
// {
// public:
//     messageData(){}
//     messageData(uint32_t c, uint32_t l, uint32_t s) : check_sum(c), msg_length(l), msg_sequense_num(s) {}
//     uint32_t getCheckSum() const { return check_sum; }
//     uint32_wangt getMsgLength() const { return msg_length; }
//     uint32_t getMsgSequenseNum() const { return msg_sequense_num; }
//     void setCheckSum(uint32_t c) { check_sum = c; }
//     void setMsgLength(uint32_t l) { msg_length = l; }
//     void setMsgSequenseNum(uint32_t s) { msg_sequense_num = s; }
// private:
//     uint32_t check_sum;
//     uint32_t msg_length;
//     uint32_t msg_sequense_num;
// };

class messageMetaData
{
public:
    messageMetaData(uint32_t l, uint32_t s, uint32_t c): length(l), sequense_num(s), check_sum(c) {}
    void set_length(uint32_t l){length = l;};
    void set_sequense_num(uint32_t s){sequense_num = s;};
    void set_check_sum(uint32_t c){check_sum = c;};
    uint32_t get_length(){return length;};
    uint32_t get_sequense_num(){return sequense_num;};
    uint32_t get_check_sum(){return check_sum;};
private:
    uint32_t length;
    uint32_t sequense_num;
    uint32_t check_sum;
};
