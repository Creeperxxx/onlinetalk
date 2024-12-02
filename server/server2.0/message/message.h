#include <string>
#include <variant>
#include <optional>
#include <vector>
#include "../requirement/cereal/types/string.hpp"
#include "../requirement/cereal/types/variant.hpp"
#include "../requirement/cereal/types/optional.hpp"
#include "../requirement/cereal/types/vector.hpp"
#include "../requirement/cereal/archives/binary.hpp"

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

enum class textAction
{
    USER_SNED_USER = 0,
    USER_RECIEVE_USER,
    USER_SEND_GROUP,
    USER_RECIEVE_GROUP
};

enum class imageAction
{
    USER_SEND_IMAGE = 100,
    USER_RECIEVE_IMAGE
};

enum class fileAction
{
    USER_SEND_FILE = 200,
    USER_RECIEVE_FILE
};

enum class voiceAction
{
    USER_SEND_VOICE = 300,
    USER_RECIEVE_VOICE
};

enum class videoAction
{
    USER_SEND_VIDEO = 400,
    USER_RECIEVE_VIDEO
};

enum class controlAction
{
    USER_LOGIN = 500,
    USER_LOGOUT,
    USER_REGISTER,
    USER_ADD_FRIEND,
    USER_DELETE_FRIEND
};

enum class noticeAction
{
    USER_LOGIN_STATUS = 600,
    USER_ONLINE_STATUS,
    FRIEND_ONLINE_STATUS,
};

class messageHeader
{
public:
    messageHeader(){}
    messageHeader(messageType type,
                  std::variant<textAction, imageAction, fileAction, voiceAction, videoAction, controlAction, noticeAction> action,
                  const std::string &sender_name,
                  int sender_id, uint32_t message_size,
                  int session_id,
                  std::optional<int> group_id = std::nullopt,
                  std::optional<int> receiver_id = std::nullopt,
                  std::optional<std::string> receiver_name = std::nullopt,
                  bool compressed = false)
                : m_type(type),
                  m_action(action),
                  m_sender_name(sender_name),
                  m_sender_id(sender_id),
                  m_message_size(message_size),
                  m_session_id(session_id),
                  m_group_id(group_id),
                  m_receiver_id(receiver_id),
                  m_receiver_name(receiver_name),
                  m_is_compressed(compressed) {}

public:
    // 提供公共接口以访问私有成员
    const messageType &getType() const { return m_type; }
    const std::variant<textAction, imageAction, fileAction, voiceAction, videoAction, controlAction, noticeAction> &getAction() const { return m_action; }
    const std::string &getSenderName() const { return m_sender_name; }
    int getSenderId() const { return m_sender_id; }
    uint32_t getMessageSize() const { return m_message_size; }
    int getSessionId() const { return m_session_id; }
    const std::optional<int> &getGroupId() const { return m_group_id; }
    const std::optional<int> &getReceiverId() const { return m_receiver_id; }
    const std::optional<std::string> &getReceiverName() const { return m_receiver_name; }
    bool getIsCompressed() const { return m_is_compressed; }

    // 设置方法
    void setType(messageType type) { m_type = type; }
    void setAction(std::variant<textAction, imageAction, fileAction, voiceAction, videoAction, controlAction, noticeAction> a) { m_action = a; }
    void setSenderName(const std::string &name) { m_sender_name = name; }
    void setSenderId(int id) { m_sender_id = id; }
    void setMessageSize(uint32_t size) { m_message_size = size; }
    void setSessionId(int id) { m_session_id = id; }
    void setGroupId(std::optional<int> id) { m_group_id = id; }
    void setReceiverId(std::optional<int> id) { m_receiver_id = id; }
    void setReceiverName(std::optional<std::string> name) { m_receiver_name = name; }
    void setIsCompressed(bool compressed) { m_is_compressed = compressed; }

private:
    messageType m_type;
    std::variant<textAction, imageAction, fileAction, voiceAction, videoAction, controlAction, noticeAction> m_action;
    std::string m_sender_name;
    int m_sender_id;
    uint32_t m_message_size;
    int m_session_id;
    std::optional<int> m_group_id;
    std::optional<int> m_receiver_id;
    std::optional<std::string> m_receiver_name;
    bool m_is_compressed;
};

class message
{
private:
    messageHeader header;
    std::vector<uint8_t> data;

public:
    message(){}
    message(messageHeader h, std::vector<uint8_t> d) : header(h), data(d) {}
    // 获取消息头
    const messageHeader &getHeader() const { return header; }
    const std::vector<uint8_t> &getData() const { return data; }

    // 设置数据
    void setData(std::vector<uint8_t> d) { data = d; }
    void setHeader(messageHeader h) { header = h; }
};
