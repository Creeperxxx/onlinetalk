#include "msgAnalysisFSM.h"

// const int MSG_IDENTIFIER_SIZE = 4;
const uint8_t MSG_IDENTIFIER[4] = {'M', 'S', 'G', '_'};
const uint32_t MSG_MAX_LENGHT = 2048;

const char *ENQUEUE_SEND_DATA = "enqueue_send_data";
const char *TASK_COMMIT = "task_commit";

void msgAnalysisFSM::process()
{
    offset = 0;
    while (offset < data->size())
    {
        bool flag = true;
        while (flag)
        {
            switch (current_state)
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
            case analysisState::end_state:
                initial_state();
                flag = false;
                break;
            default:
                break;
            }
        }
    }
}

void msgAnalysisFSM::initial_state()
{
    // offset = 0;
    length = 0;
    seq = 0;
    crc = 0;
    // error_type = FSMErrorType::none;
    current_state = analysisState::check_identifier_state;
}

void msgAnalysisFSM::check_identifier()
{
    if (memcmp(data->data() + offset, MSG_IDENTIFIER, sizeof(MSG_IDENTIFIER)) == 0)
    {
        offset += sizeof(MSG_IDENTIFIER);
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
    memcpy(&length, data->data() + offset, sizeof(length));
    length = ntohl(length);
    if (length > MSG_MAX_LENGHT)
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
    memcpy(&seq, data->data() + offset, sizeof(seq));
    seq = ntohl(seq);
    offset += sizeof(seq);
    current_state = analysisState::check_crc_state;
}

void msgAnalysisFSM::extract_message()
{
    // std::shared_ptr<std::vector<uint8_t>> message_data;
    message_data = std::make_shared<std::vector<uint8_t>>(data->begin() + offset, data->begin() + offset + length);
    offset += length;
    current_state = analysisState::check_crc_state;
}

void msgAnalysisFSM::check_crc()
{
    memcpy(&crc, data->data() + offset, sizeof(crc));
    crc = ntohl(crc);
    if (crc == std::static_pointer_cast<serializationMethodV1>(serialization_method)->calculateCRC32(data->data() + offset - length, length))
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
    msg = serialization_method->deserialize_message(message_data);
    if (msg == nullptr)
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
    std::function<std::shared_ptr<message>()> lambda = [this]() -> std::shared_ptr<message>
    {
        auto temp = this->msg_analysis_handle(this->get_deserialized_msg());
        if (temp != nullptr)
        {
            // this->enqueue_send_msg(temp);
            std::any_cast<std::function<void(std::shared_ptr<message>)>>(this->get_event_callback(ENQUEUE_SEND_DATA))(temp); // todo 这里要注册
        }
    };
    std::any_cast<std::function<void(std::function<void()>)>>(event_manager->get_callback(TASK_COMMIT))(lambda);
    current_state = analysisState::initial_state;
}

void msgAnalysisFSM::process_data(std::shared_ptr<std::vector<uint8_t>> data)
{
    this->data = data;
    process();
}

// void msgAnalysisFSM::init(std::shared_ptr<IserializationMethod> serialization_method,IEventHandler* eventHandler,std::shared_ptr<ThreadPool> threadPool)
void msgAnalysisFSM::init(std::shared_ptr<IserializationMethod> serialization_method)
{
    this->serialization_method = serialization_method;
    // this->event_handler = eventHandler;
    // this->pool = threadPool;
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

// void msgAnalysisFSM::enqueue_send_msg(std::shared_ptr<message> msg)
// {
//     auto temp_event_handler = dynamic_cast<ReactorEventHandler*>(event_handler);
//     temp_event_handler->enqueue_send_message(msg);
// }

// uint32_t msgAnalysisFSM::calculateCRC32(const uint8_t *data, size_t length)
// {
//     return
// }

msgAnalysisFSM *msgAnalysisFSM::register_event(const std::string &event_name, std::any callback)
{
    event_manager->register_callback(event_name, callback);
    return this;
}

std::shared_ptr<message> msgAnalysisFSM::get_deserialized_msg()
{
    return this->msg;
}

std::any msgAnalysisFSM::get_event_callback(const std::string &event_name)
{
    return event_manager->get_callback(event_name);
}