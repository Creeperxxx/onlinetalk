#pragma once
#include <zlib.h>
#include "../logSystem/log.h"
#include "../message/message.h"
// #include "../requirement/cereal/types/string.hpp"
// #include "../requirement/cereal/types/variant.hpp"
// #include "../requirement/cereal/types/optional.hpp"
// #include "../requirement/cereal/types/vector.hpp"
// #include "../requirement/cereal/archives/binary.hpp"
#include <cereal/types/string.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/optional.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/archives/binary.hpp>

enum class SerializationMethodType
{
    CEREAL
};

class ISerializationMethod
{
public:
    // virtual std::shared_ptr<std::vector<uint8_t>> serialize_message(std::shared_ptr<message> msg) = 0;
    // virtual std::shared_ptr<message> deserialize_message(std::shared_ptr<std::vector<uint8_t>> data) = 0;
    virtual ~ISerializationMethod() = 0;
};

class cerealSerializationMethod : public ISerializationMethod
{
public:
    static std::shared_ptr<std::vector<uint8_t>> serialize_message(std::shared_ptr<message> msg);
    static std::shared_ptr<message> deserialize_message(std::shared_ptr<std::vector<uint8_t>> data);
    static std::unique_ptr<message> deserialize_message(std::unique_ptr<std::vector<uint8_t>> data);
    static std::shared_ptr<std::vector<uint8_t>> serialize_message_metadata(std::shared_ptr<messageMetaData> msg);
    static std::shared_ptr<messageMetaData> deserialize_message_metadata(std::shared_ptr<std::vector<uint8_t>> data);
    static std::unique_ptr<messageMetaData> deserialize_message_metadata(std::unique_ptr<std::vector<uint8_t>> data);
    static uint32_t calculateCRC32(const uint8_t *data, size_t length);
    ~cerealSerializationMethod() = default;
};

// class SerializationMethodFactory
// {
// public:
    // static std::shared_ptr<ISerializationMethod> createSerializationMethod(const SerializationMethodType& type);
// };

// 序列化支持
namespace cereal {
template<class Archive>
void serialize(Archive & archive, dataHeader& obj) {
    messageType type;
    messageAction action;
    std::optional<std::string> sender_name;
    std::optional<std::string> sender_id;
    std::optional<std::string> session_id;
    std::optional<std::string> group_id;
    std::optional<std::string> receiver_name;
    std::optional<std::string> receiver_id;
    std::optional<bool> is_compressed;
    std::optional<int> sender_socketfd;

    if constexpr (Archive::is_loading::value) {
        // archive(id, name, value,string_vector,variant_value,optional_value);
        archive(type,action,sender_name,sender_id,session_id,group_id,receiver_name,receiver_id,is_compressed,sender_socketfd);
        obj.setType(type);
        obj.setAction(action);
        obj.setSenderId(sender_id.value_or(MESSAGE_HEADER_SENDER_ID_DEFAULT));
        obj.setSenderName(sender_name.value_or(MESSAGE_HEADER_SENDER_NAME_DEFAULT));
        obj.setSessionId(session_id.value_or(MESSAGE_HEADER_SESSION_ID_DEFAULT));
        obj.setGroupId(group_id.value_or(MESSAGE_HEADER_GROUP_ID_DEFAULT));
        obj.setReceiverId(receiver_id.value_or(MESSAGE_HEADER_RECEIVER_ID_DEFAULT));
        obj.setReceiverName(receiver_name.value_or(MESSAGE_HEADER_RECEIVER_NAME_DEFAULT));
        obj.setIsCompressed(is_compressed.value_or(MESSAGE_HEADER_IS_COMPRESSED_DEFAULT));
        obj.setSenderSocketFd(sender_socketfd.value_or(MESSAGE_HEADER_SENDER_SOCKET_FD_DEFAULT));
    } else {
        type = obj.getType();
        action = obj.getAction();
        sender_name = obj.getSenderName();
        sender_id = obj.getSenderId();
        receiver_name = obj.getReceiverName();
        receiver_id = obj.getReceiverId();
        session_id = obj.getSessionId();
        group_id = obj.getGroupId();
        is_compressed = obj.getIsCompressed();
        sender_socketfd = obj.getSenderSocketFd();
        // archive(id, name, value,string_vector,variant_value,optional_value);
        archive(type, action, sender_name, sender_id, receiver_name, receiver_id, session_id, group_id, is_compressed, sender_socketfd);
    }
}
}


namespace cereal
{
    template <class Archive>
    void serialize(Archive& archive, message& obj)
    {
        dataHeader header;
        std::vector<uint8_t> data;
        if constexpr (Archive::is_loading::value)
        {
            archive(header, data);
            obj.setHeader(header);
            obj.setData(data);
        }
        else
        {
            header = obj.getHeader();
            data = obj.getData();
            archive(header, data);
        }
    }
}

namespace cereal
{
    template <class Archive>
    void serialize(Archive & archive, messageMetaData & obj)
    {
        uint32_t check_sum;
        uint32_t length;
        uint32_t sequense_num;
        if(Archive::is_loading::value)
        {
            archive(check_sum, length, sequense_num);
            obj.set_check_sum(check_sum);
            obj.set_length(length);
            obj.set_sequense_num(sequense_num);
        }
        else
        {
            check_sum = obj.get_check_sum();
            length = obj.get_length();
            sequense_num = obj.get_sequense_num();
            archive(check_sum, length, sequense_num);
        }
    }
}