#include "serializationMethod.h"

std::shared_ptr<std::vector<uint8_t>> serializationMethodV1::serialize_message(std::shared_ptr<message> msg)
{
    try
    {
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);

            // 序列化 messageHeader
            oarchive(CEREAL_NVP(msg->getHeader().getType()),
                     CEREAL_NVP(msg->getHeader().getAction()),
                     CEREAL_NVP(msg->getHeader().getSenderName()),
                     CEREAL_NVP(msg->getHeader().getSenderId()),
                     //  CEREAL_NVP(msg->getHeader().getMessageSize()),
                     CEREAL_NVP(msg->getHeader().getSessionId()),
                     CEREAL_NVP(msg->getHeader().getGroupId()),
                     CEREAL_NVP(msg->getHeader().getReceiverId()),
                     CEREAL_NVP(msg->getHeader().getReceiverName()),
                     CEREAL_NVP(msg->getHeader().getIsCompressed()));

            // 序列化 message 数据
            oarchive(CEREAL_NVP(msg->getData()));
        }
        auto str = ss.str();
        return std::make_shared<std::vector<uint8_t>>(str.begin(), str.end());
    } catch (const cereal::Exception &e)
    {
        // 处理 cereal 库抛出的异常
        std::cerr << "序列化失败: " << e.what() << std::endl;
        return nullptr;
    } catch (const std::exception &e)
    {
        // 处理其他标准库异常
        std::cerr << "序列化失败: " << e.what() << std::endl;
        return nullptr;
    }
}

std::shared_ptr<message> serializationMethodV1::deserialize_message(std::shared_ptr<std::vector<uint8_t>> data)
{
    try
    {
        std::stringstream ss(std::string(data->begin(), data->end()));
        auto msg = std::make_shared<message>();
        {
            cereal::BinaryInputArchive iarchive(ss);

            // 反序列化 messageHeader
            messageType type;
            messageAction action;
            std::string sender_name;
            int sender_id;
            // uint32_t message_size;
            int session_id;
            std::optional<int> group_id;
            std::optional<int> receiver_id;
            std::optional<std::string> receiver_name;
            bool compressed;
            iarchive(CEREAL_NVP(type),
                     CEREAL_NVP(action),
                     CEREAL_NVP(sender_name),
                     CEREAL_NVP(sender_id),
                     //  CEREAL_NVP(message_size),
                     CEREAL_NVP(session_id),
                     CEREAL_NVP(group_id),
                     CEREAL_NVP(receiver_id),
                     CEREAL_NVP(receiver_name),
                     CEREAL_NVP(compressed));

            // 反序列化 message 数据
            std::vector<uint8_t> msg_data;
            iarchive(CEREAL_NVP(msg_data));
            // 创建 message 对象

            msg->setHeader(dataHeader(type, action, sender_name, sender_id, session_id, group_id, receiver_id, receiver_name, compressed));
            msg->setData(msg_data);
        }
        return msg;
    }
    catch (const cereal::Exception &e)
    {
        // 处理 cereal 库抛出的异常
        std::cerr << "反序列化失败: " << e.what() << std::endl;
        return nullptr;
    }
    catch (const std::exception &e)
    {
        // 处理其他标准库异常
        std::cerr << "反序列化失败: " << e.what() << std::endl;
        return nullptr;
    }
}
