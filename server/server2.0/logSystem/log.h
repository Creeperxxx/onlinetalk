#pragma once
#include <memory>
#include <string>
#include <mutex>
#include <stdarg.h>

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
    static log get_instance();
    // bool write_log(logLevel level,const std::string& filename,const std::string& function,size_t line_num,const std::string& format,...);
    bool write_log(logLevel level,const std::string& format,...);
    ~log(){};
    // void output_log();
private:
    log(){};
    
// private:
    //文件路径
    // std::string m_file_path;
    // static std::shared_ptr<log> m_instance;
    // static std::once_flag init_once;
    // logTarget m_target;
    // static HANDLE file_handle;
};

