#include <string>
#include <mutex>
#include <queue>
#include <memory>

class dataManager
{
public:
    static dataManager& get_instance();
    dataManager(const dataManager&) = delete; 
    dataManager& operator=(const dataManager&) = delete;
    void pushData(const std::string& data);
    std::string popData();
private:
    void init();
   dataManager();
   ~dataManager(); 
    // std::vector<std::string> data;
    std::unique_ptr<std::queue<std::string>> data_queue;
    std::mutex data_mutex;
};