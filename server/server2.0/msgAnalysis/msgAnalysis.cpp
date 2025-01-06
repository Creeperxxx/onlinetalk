#include "msgAnalysis.h"

// void msgAnalysis::socket_data_analysis(const std::shared_ptr<socketData> data)
// {
//     auto recv_data = data->pop_recv_data();
//     int offest = 0;
//     int recv_data_size = recv_data->size();
//     while(offest < recv_data_size)
//     {

//     }
// }

void msgAnalysis::socket_recv_data_analysis(std::unique_ptr<std::vector<uint8_t>> data)
{
    process_incoming_data(std::move(data));
    // int offset = 0;
    // int recv_data_size = data->size();
    // uint32_t meta_data_length = 0;

    // while (offset < recv_data_size)
    // {
    //     if (recv_data_size - offset < sizeof(uint32_t))
    //     {
    //         LOG_ERROR("%s:%s:%d // 数据不足，无法读取元数据长度", __FILE__, __FUNCTION__, __LINE__);
    //         return;
    //     }

    //     std::memcpy(&meta_data_length, data->data() + offset, sizeof(uint32_t));
    //     meta_data_length = ntohl(meta_data_length);
    //     offset += sizeof(uint32_t);

    //     if (meta_data_length > 0)
    //     {
    //         if (recv_data_size - offset < static_cast<int>(meta_data_length))
    //         {
    //             LOG_ERROR("%s:%s:%d // 数据不足，无法读取元数据", __FILE__, __FUNCTION__, __LINE__);
    //             return;
    //         }

    //         try
    //         {
    //             auto meta_data = std::make_unique<std::vector<uint8_t>>(
    //                 data->data() + offset,
    //                 data->data() + offset + meta_data_length);
    //             offset += meta_data_length;

    //             auto msg_meta_data = cerealSerializationMethod::deserialize_message_metadata(std::move(meta_data));

    //             if (msg_meta_data != nullptr)
    //             {
    //                 int length = msg_meta_data->get_length();

    //                 if (length > 0)
    //                 {
    //                     if (recv_data_size - offset < length)
    //                     {
    //                         LOG_ERROR("%s:%s:%d // 数据不足，无法读取消息", __FILE__, __FUNCTION__, __LINE__);
    //                         return;
    //                     }

    //                     if (cerealSerializationMethod::calculateCRC32(data->data() + offset, length) == msg_meta_data->get_check_sum())
    //                     {
    //                         auto msg_data = std::make_unique<std::vector<uint8_t>>(
    //                             data->data() + offset,
    //                             data->data() + offset + length);
    //                         offset += length;

    //                         auto msg = cerealSerializationMethod::deserialize_message(std::move(msg_data));

    //                         if (msg != nullptr)
    //                         {
    //                             // 处理消息
    //                         }
    //                         else
    //                         {
    //                             continue;
    //                         }
    //                     }
    //                     else
    //                     {
    //                         LOG_ERROR("%s:%s:%d // 校验和验证失败", __FILE__, __FUNCTION__, __LINE__);
    //                         offset += length;
    //                         continue;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     LOG_ERROR("%s:%s:%d // 消息长度为0", __FILE__, __FUNCTION__, __LINE__);
    //                     return;
    //                 }
    //             }
    //             else
    //             {
    //                 LOG_ERROR("%s:%s:%d // 元数据反序列化失败", __FILE__, __FUNCTION__, __LINE__);
    //                 return;
    //             }
    //         }
    //         catch (const std::exception &e)
    //         {
    //             LOG_ERROR("%s:%s:%d // 异常: %s", __FILE__, __FUNCTION__, __LINE__, e.what());
    //             return;
    //         }
    //     }
    // }





    // int offest = 0;
    // int recv_data_size = data->size();
    // uint32_t meta_data_length = 0;
    // while(offest < recv_data_size)
    // {
    //     std::memcpy(&meta_data_length,data->data()+offest,sizeof(uint32_t));
    //     meta_data_length = ntohl(meta_data_length);
    //     if(meta_data_length > 0)
    //     {
    //         offest += sizeof(meta_data_length);
    //         std::unique_ptr<std::vector<uint8_t>> meta_data(new std::vector<uint8_t>(data->begin()+offest,data->begin()+offest+meta_data_length));
    //         std::unique_ptr<messageMetaData> msg_meta_data = cerealSerializationMethod::deserialize_message_metadata(std::move(meta_data));
    //         if(msg_meta_data != nullptr)
    //         {
    //             offest += meta_data_length;
    //             int length = msg_meta_data->get_length();
    //             if(length > 0)
    //             {
    //                 if(cerealSerializationMethod::calculateCRC32(data->data() + offest,length) == msg_meta_data->get_check_sum())
    //                 {
    //                     std::unique_ptr<std::vector<uint8_t>> msg_data(new std::vector<uint8_t>(data->data() + offest,data->data() + offest + length));
    //                     std::unique_ptr<message> msg = cerealSerializationMethod::deserialize_message(std::move(msg_data));
    //                     offest += length;
    //                     if(msg != nullptr)
    //                     {

    //                     }
    //                     else
    //                     {
    //                         continue;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     LOG_ERROR("%s:%s:%d // 检验校验和失败",__FILE__, __FUNCTION__, __LINE__);
    //                     offest+=length;
    //                     continue;
    //                 }
    //             }
    //             else
    //             {
    //                 LOG_ERROR("%s:%s:%d // 消息长度为0",__FILE__,__FUNCTION__,__LINE__);
    //                 return;
    //             }
    //         }
    //         else
    //         {
    //             return;
    //         }
    //     }
    // }
}

bool has_enough_data(int recv_data_size, int offset, size_t required_bytes)
{
    return recv_data_size - offset >= static_cast<int>(required_bytes);
}

uint32_t read_message_length(const uint8_t *data, int &offset, int recv_data_size)
{
    if (!has_enough_data(recv_data_size, offset, sizeof(uint32_t)))
    {
        LOG_ERROR("%s:%s:%d // 数据不足，无法读取元数据长度", __FILE__, __FUNCTION__, __LINE__);
        throw std::runtime_error("Data insufficient for reading message length");
    }

    uint32_t meta_data_length = 0;
    std::memcpy(&meta_data_length, data + offset, sizeof(uint32_t));
    meta_data_length = ntohl(meta_data_length);
    offset += sizeof(uint32_t);

    return meta_data_length;
}

std::unique_ptr<messageMetaData> extract_and_deserialize_meta_data(
    const uint8_t *data, int &offset, int recv_data_size, uint32_t meta_data_length)
{

    if (!has_enough_data(recv_data_size, offset, meta_data_length))
    {
        LOG_ERROR("%s:%s:%d // 数据不足，无法读取元数据", __FILE__, __FUNCTION__, __LINE__);
        throw std::runtime_error("Data insufficient for reading metadata");
    }

    auto meta_data = std::make_unique<std::vector<uint8_t>>(
        data + offset,
        data + offset + meta_data_length);
    offset += meta_data_length;

    try
    {
        return cerealSerializationMethod::deserialize_message_metadata(std::move(meta_data));
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("%s:%s:%d // 元数据反序列化失败: %s", __FILE__, __FUNCTION__, __LINE__, e.what());
        return nullptr;
    }
}

// bool verify_crc32(const uint8_t *data, int &offset, int recv_data_size, const std::unique_ptr<messageMetaData> msg_meta_data)
bool msgAnalysis::verify_crc32(const uint8_t *data, int &offset, int recv_data_size, uint32_t length, uint32_t check_sum)
{
    // int length = msg_meta_data->get_length();
    if (length <= 0 || !has_enough_data(recv_data_size, offset, length))
    {
        LOG_ERROR("%s:%s:%d // 消息长度无效或数据不足", __FILE__, __FUNCTION__, __LINE__);
        throw std::runtime_error("Invalid message length or insufficient data");
        return false;
    }

    if (cerealSerializationMethod::calculateCRC32(data + offset, length) != check_sum)
    {
        LOG_ERROR("%s:%s:%d // 校验和验证失败", __FILE__, __FUNCTION__, __LINE__);
        throw std::runtime_error("Checksum verification failed");
        return false;
    }
    return true;
}

// void process_message(const uint8_t *data, int &offset, int recv_data_size, const std::unique_ptr<messageMetaData> msg_meta_data)
void msgAnalysis::process_message(const uint8_t *data, int &offset, int recv_data_size, uint32_t length, uint32_t sequense_num)
{
    if (length <= 0)
    {
        LOG_ERROR("%s:%s:%d // 消息长度为0", __FILE__, __FUNCTION__, __LINE__);
        throw std::runtime_error("Message length is zero");
    }

    auto msg_data = std::make_unique<std::vector<uint8_t>>(
        data + offset,
        data + offset + length);
    offset += length;

    auto msg = cerealSerializationMethod::deserialize_message(std::move(msg_data));

    if (msg != nullptr)
    {
        // 处理消息
    }
    else
    {
        LOG_ERROR("%s:%s:%d // 消息反序列化失败", __FILE__, __FUNCTION__, __LINE__);
        // todo 根据序列号重发消息

        throw std::runtime_error("Failed to deserialize message");
    }
}

void msgAnalysis::process_incoming_data(const std::unique_ptr<std::vector<uint8_t>> data)
{
    int offset = 0;
    int recv_data_size = data->size();

    while (offset < recv_data_size)
    {
        try
        {
            uint32_t meta_data_length = read_message_length(data->data(), offset, recv_data_size);

            if (meta_data_length > 0)
            {
                // auto msg_meta_data = extract_and_deserialize_meta_data(data.get(), offset, recv_data_size, meta_data_length);
                auto msg_meta_data = extract_and_deserialize_meta_data(data->data(), offset, recv_data_size, meta_data_length);
                if (msg_meta_data != nullptr)
                {
                    // verify_crc32(data.get(), offset, recv_data_size, msg_meta_data.get());
                    if (verify_crc32(data->data(), offset, recv_data_size, msg_meta_data->get_length(), msg_meta_data->get_check_sum()) == true)
                    {
                        process_message(data->data(), offset, recv_data_size, msg_meta_data->get_length(), msg_meta_data->get_sequense_num());
                    }
                    else
                    {

                    }
                }
            }
        }
        catch (const std::exception &e)
        {
            LOG_ERROR("%s:%s:%d // 异常: %s", __FILE__, __FUNCTION__, __LINE__, e.what());
            // 可选择跳过错误消息并继续处理剩余的数据
            continue;
        }
    }
}