#pragma once

#include "../msgHandle/msgHandle.h"
#include "../serializationMethod/serializationMethod.h"

class msgAnalysis
{
public:
    msgAnalysis() {}
    ~msgAnalysis() {}
    // static void analysis(const std::shared_ptr<msgHandle>& msg);
    // void analysis(const std::shared_ptr<std::vector<uint8_t>> msg);
    // static void socket_data_analysis(const std::shared_ptr<socketData> data);
    void socket_recv_data_analysis(std::unique_ptr<std::vector<uint8_t>> data);

private:
    bool has_enough_data(int recv_data_size, int offset, size_t required_bytes);
    // uint32_t read_message_length(const std::vector<uint8_t> *data, int &offset, int recv_data_size);
    uint32_t read_message_length(const uint8_t *data, int &offset, int recv_data_size);

    std::unique_ptr<messageMetaData> extract_and_deserialize_meta_data(const uint8_t *data, int &offset, int recv_data_size, uint32_t meta_data_length);
    // bool verify_crc32(const uint8_t *data, int &offset, int recv_data_size, const std::unique_ptr<messageMetaData> msg_meta_data);
    bool verify_crc32(const uint8_t *data, int &offset, int recv_data_size, uint32_t length, uint32_t check_sum);
    void process_message(const uint8_t *data, int &offset, int recv_data_size, uint32_t length, uint32_t sequence_num);
    void msgAnalysis::process_incoming_data(const std::unique_ptr<std::vector<uint8_t>> data);
};