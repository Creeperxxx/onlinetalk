#include <memory>
#include <string>
#include <mutex>
enum class logLevel
{
    LOG_LEVEL_NONE,
    LOG_LEVEL_ERROR,   // error
    LOG_LEVEL_WARNING, // warning
    LOG_LEVEL_DEBUG,   // debug
    LOG_LEVEL_INFO,    // info
};

enum class logTarget
{
    LOG_TAEGET_NONE = 0x00,
    LOG_TAEGET_CONSOLE = 0x01,
    LOG_TAEGET_FILE = 0x10,
};

class log
{
public:
    void init(logLevel level,logTarget target);
    int create_file();
    static std::shared_ptr<log> getInstance();
    logLevel get_loglevel();
    void set_loglevel(logLevel level);
    logTarget get_logtarget();
    void set_logtarget(logTarget target);
    static int write_log(logLevel level,unsigned char* filename,unsigned char *function,size_t line_num,char* format,...);
    static void output_log();
private:
    log(){};
    ~log(){};
    
private:
    static std::shared_ptr<log> m_instance;
    static std::once_flag init_once;
    static std::string log_buffer;
    logLevel m_level;
    logTarget m_target;
    // static HANDLE file_handle;
};