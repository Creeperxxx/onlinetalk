#include <vector>
#include <memory>
#include "../serializationMethod/serializationMethod.h"
#include <cstring>

extern const int MSG_IDENTIFIER_SIZE;
extern const char MSG_IDENTIFIER[MSG_IDENTIFIER_SIZE];
extern const uint32_t MSG_MAX_LENGHT;

enum class analysisState
{
    initial_state,
    check_identifier_state,
    extract_length_state,
    check_crc_state,
    extract_sequence_state,
    extract_message_state,
    deserialize_state,
    process_msg_state,
    error_handling_state
};

enum class FSMErrorType
{
    invalid_identifier,
    invalid_length,
    
};


class msgAnalysisFSM
{
public:
    msgAnalysisFSM();
    void process_data(std::shared_ptr<std::vector<uint8_t>> data);
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
    std::shared_ptr<IserializationMethod> serialization_method;
    std::shared_ptr<std::vector<uint8_t>> data;
    FSMErrorType error_type;
};