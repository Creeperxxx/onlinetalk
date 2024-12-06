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
            case analysisState::extract_sequence_state:
                extract_sequence();
                break;
            case analysisState::extract_message_state:
                extract_message();
                break;
            case analysisState::check_crc_state:
                check_crc();
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



void msgAnalysisFSM::initial_state()
{
    offset = 0;
    length = 0;
    seq = 0;
    crc = 0;
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

void msgAnalysisFSM::extract_sequence()
{
    memcpy(&seq,data->data() + offset,sizeof(seq));
    seq = ntohl(seq);
    offset += sizeof(seq);
    current_state = analysisState::check_crc_state;
}

void msgAnalysisFSM::extract_message()
{
    // std::shared_ptr<std::vector<uint8_t>> message_data;
    message_data = std::make_shared<std::vector<uint8_t>>(data->begin() + offset,data->begin() + offset + length);
    offset += length;
    current_state = analysisState::check_crc_state;
}

void msgAnalysisFSM::check_crc()
{
    memcpy(&crc,data->data() + offset,sizeof(crc));
    crc = ntohl(crc);
    if(crc == calculateCRC32(data->data() + offset - length,length))
    {
        // 校验通过
        offset += sizeof(crc);
        current_state = analysisState::deserialize_state;
    }
    else
    {
        // 错误处理
        error_type = FSMErrorType::crc_failed;
        current_state = analysisState::error_handling_state;
    }
}

void msgAnalysisFSM::deserialize()
{
    msg = serialization_method->deserialize(message_data);
    if(msg == nullptr)
    {
        // 错误处理
        error_type = FSMErrorType::deserialize_failed;
        current_state = analysisState::error_handling_state;
    }
    else
    {
        current_state = analysisState::process_msg_state;
    }
}

void msgAnalysisFSM::process_msg()
{
    auto lambda = [this](std::shared_ptr<message> msg){
        auto retmsg = this->msg_analysis_handle(msg);
        this->enqueue_send_msg(retmsg);
    } 
    pool->commit(lambda,msg);
    current_state = analysisState::initial_state;
}

void msgAnalysisFSM::process_data(std::shared_ptr<std::vector<uint8_t>> data)
{
    this->data = data;
    process();
}

void msgAnalysisFSM::init(std::shared_ptr<IserializationMethod> serialization_method,IEventHandler* eventHandler,std::shared_ptr<ThreadPool> threadPool)
{
    this->serialization_method = serialization_method;
    this->eventHandler = eventHandler;
    this->pool = threadPool;
    current_state = analysisState::initial_state;
    msg_analysis = std::make_shared<msgAnalysis>();
    msg_analysis->init();
}

std::shared_ptr<message> msgAnalysisFSM::msg_analysis_handle(std::shared_ptr<message> msg)
{
    return msg_analysis->handle(msg);
}

// std::shared_ptr<message> msgAnalysisFSM::get_deserialized_msg()
// {
//     return this->msg;
// }

void msgAnalysisFSM::enqueue_send_msg(std::shared_ptr<message> msg)
{
    auto temp_event_handler = dynamic_cast<ReactorEventHandler*>(eventHandler);
    temp_event_handler->enqueue_send_message(msg);
}