#include "msgAnalysisFSM.h"

const int MSG_IDENTIFIER_SIZE = 4;
const uint8_t MSG_IDENTIFIER[MSG_IDENTIFIER_SIZE] = {'M', 'S', 'G', '_'};
const uint32_t MSG_MAX_LENGHT = 2048;

void msgAnalysisFSM::process()
{
    while(offset < data->size())
    {
        switch(current_state)
        {
            case analysisState::initial_state:
                current_state = analysisState::check_identifier_state;
                break;
            case analysisState::check_identifier_state:
                check_identifier();
                break;
            case analysisState::extract_length_state:
                extract_length();
                break;
            case analysisState::check_crc_state:
                check_crc();
                break;
            case analysisState::extract_sequence_state:
                extract_sequence();
                break;
            case analysisState::extract_message_state:
                extract_message();
                break;
            case analysisState::deserialize_state:
                deserialize();
                break;
            case analysisState::process_msg_state:
                process_msg();
                break;
            case analysisState::error_handling_state:
                error_handling();
                break;
            default:
                break;
        }
    }
}

msgAnalysisFSM::msgAnalysisFSM()
{
    current_state = analysisState::initial_state;
    offset = 0;
    serialization_method = std::make_shared<serializationMethodV1>();
}

void msgAnalysisFSM::initial_state()
{
    offset = 0;
    current_state = analysisState::check_identifier_state;
}

void msgAnalysisFSM::check_identifier()
{
    if(memcmp(data->data() + offset, MSG_IDENTIFIER, MSG_IDENTIFIER_SIZE) == 0)
    {
        offset += MSG_IDENTIFIER_SIZE;
        current_state = analysisState::extract_length_state;
    }
    else
    {
        // 错误处理
        error_type = FSMErrorType::invalid_identifier;
        current_state = analysisState::error_handling_state;     
    }
}

void msgAnalysisFSM::process_data(std::shared_ptr<std::vector<uint8_t>> data)
{
    this->data = data;
    process();
}

void msgAnalysisFSM::extract_length()
{
    memcpy(&length,data->data() + offset,sizeof(length));
    length = ntohl(length);
    if(length > MSG_MAX_LENGHT )
    {
        // 错误处理
        error_type = FSMErrorType::invalid_length;
        current_state = analysisState::error_handling_state;
        return;
    }
    else
    {
        offset += sizeof(length);
        current_state = analysisState::check_crc_state;
        return;
    }
}