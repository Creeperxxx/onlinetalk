#pragma once
#include <vector>
#include <memory>
#include "../serializationMethod/serializationMethod.h"
#include <zlib.h>
#include <cstring>
#include "../eventHandler/eventHandler.h"
#include "../msgAnalysis/msgAnalysis.h"
#include "../threadPool/threadPool.h"

extern const int MSG_IDENTIFIER_SIZE;
extern const char MSG_IDENTIFIER[MSG_IDENTIFIER_SIZE];
extern const uint32_t MSG_MAX_LENGHT;

enum class analysisState
{
    initial_state,
    check_identifier_state,
    extract_length_state,
    extract_sequence_state,
    extract_message_state,
    check_crc_state,
    deserialize_state,
    process_msg_state,
    error_handling_state
};

enum class FSMErrorType
{
    invalid_identifier,
    invalid_length,
    crc_failed,
    deserialize_failed,
    invalid_sequence,
};


class msgAnalysisFSM
{
public:
    msgAnalysisFSM(){}
    void init(std::shared_ptr<IserializationMethod> serialization_method,IEventHandler* eventHandler,std::shared_ptr<ThreadPool> threadPool);
    void process_data(std::shared_ptr<std::vector<uint8_t>> data);
    std::shared_ptr<message> msg_analysis_handle(std::shared_ptr<message> msg);
    // std::shared_ptr<message> get_deserialized_msg();
    void enqueue_send_msg(std::shared_ptr<message> msg);
    void threadpool_commit()
private:
    void process();
    void initial_state();
    void check_identifier();
    void extract_length();
    void check_crc();
    void extract_sequence();
    void extract_message();
    void process_msg();
    void deserialize();
    void error_handling();
private:
    uint32_t length;
    analysisState current_state;
    size_t offset;
    uint32_t seq;
    uint32_t crc;
    std::shared_ptr<IserializationMethod> serialization_method;
    std::shared_ptr<std::vector<uint8_t>> data;
    std::shared_ptr<std::vector<uint8_t>> message_data;
    std::shared_ptr<message> msg;
    std::shared_ptr<msgAnalysis> msg_analysis;
    std::shared_ptr<ThreadPool> pool;
    IEventHandler* event_handler;
    FSMErrorType error_type;
};