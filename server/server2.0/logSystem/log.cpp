#include "log.h"
const std::string LOG_PATH = "./log.txt";

// void log::init(logTarget target)
// {
//     m_target = target;
// }

// std::shared_ptr<log> log::getInstance()
// {
//     std::call_once(init_once, []()
//                    { m_instance = std::make_shared<log>(); });
//     return m_instance;
// }

// bool log::write_log(logLevel level,const unsigned char* filename,unsigned char *function,size_t line_num,char* format,...)
// bool log::write_log(logLevel level,const std::string& filename,const std::string& function,size_t line_num,const std::string& format,...)
// {
//     va_list args;
//     va_start(args, format);
//     std::string log_str;
//     // 日志级别
//     switch (level)
//     {
//     case logLevel::LOG_LEVEL_ERROR:
//         log_str += " [ERROR]";
//         break;
//     case logLevel::LOG_LEVEL_WARNING:
//         log_str += " [WARNING]";
//         break;
//     case logLevel::LOG_LEVEL_DEBUG:
//         log_str += " [DEBUG]";
//         break;
//     case logLevel::LOG_LEVEL_INFO:
//         log_str += " [INFO]";
//         break;
//     default:
//         break;
//     }
//     // 时间
//     time_t now = time(0);
//     tm *ltm = localtime(&now);
//     char time_str[128];
//     strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ltm);
//     log_str += time_str;
//     // 文件名
//     log_str += " [" + filename + "]";
//     // 函数名
//     log_str += " [" + function + "]";
//     // 行号
//     log_str += " [" + std::to_string(line_num) + "]";
    
//     // 日志内容
//     char buf[1024];
//     vsnprintf(buf, sizeof(buf), format.c_str(), args);
//     log_str += " " + std::string(buf);
//     // 输出日志到文件中
//     // FILE *fp = fopen(m_file_path.c_str(), "a");
//     FILE *fp = fopen(LOG_PATH.c_str(), "a");
//     if (fp == nullptr)
//     {
//         return false;
//     }
//     fputs(log_str.c_str(), fp);
//     fclose(fp);
//     va_end(args);
//     return true;
// }

log& log::get_instance()
{
    static log instance;
    return instance;
}

bool log::write_log(logLevel level,const std::string format,...)
{
    va_list args;
    va_start(args, format);
    std::string log_str;
    // 日志级别
    switch (level)
    {
    case logLevel::LOG_LEVEL_ERROR:
        log_str += " [ERROR]";
        break;
    case logLevel::LOG_LEVEL_WARNING:
        log_str += " [WARNING]";
        break;
    case logLevel::LOG_LEVEL_DEBUG:
        log_str += " [DEBUG]";
        break;
    case logLevel::LOG_LEVEL_INFO:
        log_str += " [INFO]";
        break;
    default:
        break;

    }
    // 时间
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char time_str[128];
    strftime(time_str, sizeof(time_str), " [%Y-%m-%d] [%H:%M:%S]", ltm);
    log_str += time_str;
    // 日志内容
    char buf[1024];
    vsnprintf(buf, sizeof(buf), format.c_str(), args);
    log_str += " [" + std::string(buf) + "]";
    log_str += "\n";
    // std::lock_guard<std::mutex> lock(m_log_mutex);
    // m_log_queue.push(log_str);
    // m_log_cond.notify_one();

    m_log_queue.enqueue(log_str);
    // // 输出日志到文件中
    // FILE *fp = fopen(LOG_PATH.c_str(), "a");
    // if (fp == nullptr)
    // {
    //     return false;
    // }
    // fputs(log_str.c_str(), fp);
    // fclose(fp);
    va_end(args);
    return true;
}

void log::flush_log()
{
    std::string log_str;
    FILE* fp = fopen(LOG_PATH.c_str(), "a");
    if( nullptr == fp )
    {
        // LOG_ERROR("%s:%s:%d // 打开日志文件失败，无法写日志", __FILE__, __FUNCTION__, __LINE__);
        perror("打开日志文件失败，无法写日志");
        return;
    }
    while(flushing.load())
    {
        if(m_log_queue.try_dequeue(log_str))
        {
            fputs(log_str.c_str(), fp);
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }

        // std::unique_lock<std::mutex> lock(m_log_mutex);
        // while(m_log_queue.empty())
        // {
        //     m_log_cond.wait(lock);
        // }
        // for( int i = 0; i < m_log_queue.size(); i++)
        // {
        //     log_str = m_log_queue.front();
        //     m_log_queue.pop();
        //     fputs(log_str.c_str(), fp);
        // }
    }
    fclose(fp);
}