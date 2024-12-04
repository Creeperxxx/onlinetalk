#include <hiredis/hiredis.h>
#include <memory>
#include <queue>
#include <mutex>
#include <vector>
#include <string>
#include "../requirement/moodycamel/concurrentqueue.h"

const std::string address;
const int port;
const size_t pool_size;

class redisPool
{
public:
    static std::shared_ptr<redisPool> getInstance();
    std::shared_ptr<redisContext> get_connection();
    void release_connection(std::shared_ptr<redisContext> conn);
    // void destroy_pool();
    // void redisfree();
private:
    void initialize_pool(const std::string& address, int port ,size_t pool_size);
    redisPool(){}
    ~redisPool(){}
    static std::shared_ptr<redisPool> m_instance;
    static std::mutex instance_mutex;
    // std::mutex conn_mutex;
    moodycamel::ConcurrentQueue<std::shared_ptr<redisContext>> available_connections;
    std::vector<std::shared_ptr<redisContext>> pool;
    std::mutex pool_mutex;
    size_t m_pool_size;
    std::string m_address;
    int m_port;
    
}