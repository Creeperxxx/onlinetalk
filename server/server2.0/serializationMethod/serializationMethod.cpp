#include "serializationMethod.h"

std::shared_ptr<std::vector<uint8_t>> cerealSerializationMethod::serialize_message(std::shared_ptr<message> msg)
{
    try
    {
        std::ostringstream ss(std::ios::binary);
        cereal::BinaryOutputArchive oarchive(ss);
        oarchive(msg);
        auto str = ss.str();
        return std::make_shared<std::vector<uint8_t>>(reinterpret_cast<uint8_t*>(str.data()), reinterpret_cast<uint8_t*>(str.data()) + str.size());
    }catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // 异常为：%s", __FILE__, __FUNCTION__, __LINE__, e.what());
        return nullptr;
    }


    // try
    // {
    //     std::stringstream ss;
    //     {
    //         cereal::BinaryOutputArchive oarchive(ss);

    //         // 序列化 messageHeader
    //         oarchive(CEREAL_NVP(msg->getHeader().getType()),
    //                  CEREAL_NVP(msg->getHeader().getAction()),
    //                  CEREAL_NVP(msg->getHeader().getSenderName()),
    //                  CEREAL_NVP(msg->getHeader().getSenderId()),
    //                  //  CEREAL_NVP(msg->getHeader().getMessageSize()),
    //                  CEREAL_NVP(msg->getHeader().getSessionId()),
    //                  CEREAL_NVP(msg->getHeader().getGroupId()),
    //                  CEREAL_NVP(msg->getHeader().getReceiverId()),
    //                  CEREAL_NVP(msg->getHeader().getReceiverName()),
    //                  CEREAL_NVP(msg->getHeader().getIsCompressed()));

    //         // 序列化 message 数据
    //         oarchive(CEREAL_NVP(msg->getData()));
    //     }
    //     auto str = ss.str();
    //     return std::make_shared<std::vector<uint8_t>>(str.begin(), str.end());
    // } catch (const cereal::Exception &e)
    // {
    //     // 处理 cereal 库抛出的异常
    //     std::cerr << "序列化失败: " << e.what() << std::endl;
    //     return nullptr;
    // } catch (const std::exception &e)
    // {
    //     // 处理其他标准库异常
    //     std::cerr << "序列化失败: " << e.what() << std::endl;
    //     return nullptr;
    // }
}

std::shared_ptr<message> cerealSerializationMethod::deserialize_message(std::shared_ptr<std::vector<uint8_t>> data)
{
    try 
    {
        std::shared_ptr<message> msg = std::make_shared<message>();
        std::istringstream iss(std::string(data->begin(), data->end()),std::ios::binary);
        cereal::BinaryInputArchive iarchive(iss);
        iarchive(*msg);
        return msg;
    }catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // 异常为：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
        return nullptr;
    }
    // try
    // {
    //     std::stringstream ss(std::string(data->begin(), data->end()));
    //     auto msg = std::make_shared<message>();
    //     {
    //         cereal::BinaryInputArchive iarchive(ss);

    //         // 反序列化 messageHeader
    //         messageType type;
    //         messageAction action;
    //         /* std::string sender_name;
    //         int sender_id;
    //         // uint32_t message_size;
    //         int session_id;
    //         std::optional<int> group_id;
    //         std::optional<int> receiver_id;
    //         std::optional<std::string> receiver_name;
    //         bool compressed; */
    //         std::string sender_name;
    //         std::string sender_id;
    //         std::string receiver_name;
    //         std::string receiver_id;
    //         std::string session_id;
    //         std::string group_id;
    //         bool compressed;
    //         iarchive(CEREAL_NVP(type),
    //                  CEREAL_NVP(action),
    //                  CEREAL_NVP(sender_name),
    //                  CEREAL_NVP(sender_id),
    //                  CEREAL_NVP(receiver_name),
    //                  CEREAL_NVP(receiver_id),
    //                  //  CEREAL_NVP(message_size),
    //                  CEREAL_NVP(session_id),
    //                  CEREAL_NVP(group_id),
    //                 //  CEREAL_NVP(receiver_id),
    //                 //  CEREAL_NVP(receiver_name),
    //                  CEREAL_NVP(compressed));

    //         // 反序列化 message 数据
    //         std::vector<uint8_t> msg_data;
    //         iarchive(CEREAL_NVP(msg_data));
    //         // 创建 message 对象

    //         msg->setHeader(dataHeader(type,action,sender_name,sender_id,receiver_name,receiver_id,session_id,group_id,compressed));
    //         msg->setData(msg_data);
    //     }
    //     return msg;
    // }
    // catch (const cereal::Exception &e)
    // {
    //     // 处理 cereal 库抛出的异常
    //     std::cerr << "反序列化失败: " << e.what() << std::endl;
    //     return nullptr;
    // }
    // catch (const std::exception &e)
    // {
    //     // 处理其他标准库异常
    //     std::cerr << "反序列化失败: " << e.what() << std::endl;
    //     return nullptr;
    // }
}

uint32_t cerealSerializationMethod::calculateCRC32(const uint8_t *data, size_t length)
{
    uint32_t crc = crc32(0L, Z_NULL, 0); // 初始化CRC32
    crc = crc32(crc, data, length);      // 计算CRC32
    return crc;
}

/* std::shared_ptr<ISerializationMethod> SerializationMethodFactory::createSerializationMethod(const SerializationMethodType &type)
{
    switch (type)
    {
    case SerializationMethodType::CEREAL:
        return std::make_shared<cerealSerializationMethod>();
    default:
        return nullptr;
    }
} */

std::shared_ptr<std::vector<uint8_t>> cerealSerializationMethod::serialize_message_metadata(std::shared_ptr<messageMetaData> msg)
{
    try
    {
        std::ostringstream ss(std::ios::binary);
        cereal::BinaryOutputArchive oarchive(ss);
        oarchive(msg);
        auto str = ss.str();
        return std::make_shared<std::vector<uint8_t>>(reinterpret_cast<uint8_t*>(str.data()),reinterpret_cast<uint8_t*>(str.data())+ str.size());
    }catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // 异常为：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
        return nullptr;
    }
}

std::shared_ptr<messageMetaData> cerealSerializationMethod::deserialize_message_metadata(std::shared_ptr<std::vector<uint8_t>> data)
{
    try
    {
        std::istringstream ss(std::string(data->begin(), data->end()),std::ios::binary);
        cereal::BinaryInputArchive iarchive(ss);
        auto msg = std::make_shared<messageMetaData>();
        iarchive(*msg);
        return msg;     
    }catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // 异常为：" , __FILE__,__FUNCTION__,__LINE__,e.what());
        return nullptr;
    }
}

std::unique_ptr<messageMetaData> cerealSerializationMethod::deserialize_message_metadata(std::unique_ptr<std::vector<uint8_t>> data)
{
    try
    {
        std::istringstream ss(std::string(data->begin(),data->end()),std::ios::binary);
        cereal::BinaryInputArchive iarchive(ss);
        auto msg = std::make_unique<messageMetaData>();
        iarchive(*msg);
        return std::move(msg);
    }catch(const std::exception& e)
    {
        LOG_ERROR("%s:%s:%d // 异常为：" , __FILE__,__FUNCTION__,__LINE__,e.what());
        return nullptr;
    }
}

std::unique_ptr<message> cerealSerializationMethod::deserialize_message(std::unique_ptr<std::vector<uint8_t>> data)
{
    try
    {
        std::istringstream ss(std::string(data->begin(), data->end()),std::ios::binary);
        cereal::BinaryInputArchive iarchive(ss);
        auto msg = std::make_unique<message>();
        iarchive(*msg);
        return std::move(msg);
    }
    catch (const std::exception& e)
    {
        // std::cout << e.what() << std::endl;
        LOG_ERROR("%s:%s:%d // 异常为：%s",__FILE__,__FUNCTION__,__LINE__,e.what());
        return nullptr;
    }
}