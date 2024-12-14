#pragma once
#include <memory>
#include <string>
#include <mutex>
#include <stdarg.h>
#include <queue>
#include <condition_variable>
#include <atomic>
#include "../requirement/moodycamel/concurrentqueue.h"
#include <chrono>

// #define LOG_WRITE(level,format,...) \
//     log::get_instance().write_Log(level,format,##__VA_ARGS__)
#define LOG_INFO(format,...) log::get_instance().write_log(logLevel::LOG_LEVEL_INFO,format,##__VA_ARGS__)
#define LOG_DEBUG(format,...) log::get_instance().write_log(logLevel::LOG_LEVEL_DEBUG,format,##__VA_ARGS__)
#define LOG_ERROR(format,...) log::get_instance().write_log(logLevel::LOG_LEVEL_ERROR,format,##__VA_ARGS__)
#define LOG_WARING(format,...) log::get_instance().write_log(logLevel::LOG_LEVEL_WARNING,format,##__VA_ARGS__)

extern const std::string LOG_PATH;


enum class logLevel
{
    // LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,   // error
    LOG_LEVEL_WARNING, // warning
    LOG_LEVEL_DEBUG,   // debug
    LOG_LEVEL_INFO,    // info
};

class log
{
public:
    // void init(logTarget target);
    // static std::shared_ptr<log> getInstance();
    static log& get_instance();
    // bool write_log(logLevel level,const std::string& filename,const std::string& function,size_t line_num,const std::string& format,...);
    bool write_log(logLevel level,const std::string format,...);
    void flush_log();
    ~log(){ flushing.store(false);};
    // void output_log();
    log(const log&) = delete;
    log& operator=(const log&) = delete;
private:
    log(){};
private:
    moodycamel::ConcurrentQueue<std::string> m_log_queue;
    // std::queue<std::string> m_log_queue;
    // std::mutex m_log_mutex;
    // std::condition_variable m_log_cond;
    std::atomic<bool> flushing = true;
// private:
    //文件路径
    // std::string m_file_path;
    // static std::shared_ptr<log> m_instance;
    // static std::once_flag init_once;
    // logTarget m_target;
    // static HANDLE file_handle;
};

