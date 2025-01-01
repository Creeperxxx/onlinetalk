#include <string>
#include <mutex>
#include <queue>
#include <memory>
#include <vector>
#include <condition_variable>
#include "../socketManager/socketManager.h"

inline const int READY_SOCKET_VEC_ISEMPTY = -1;

class dataManager
{
public:
    static dataManager& get_instance();
    dataManager(const dataManager&) = delete; 
    dataManager& operator=(const dataManager&) = delete;
    void pushData(const std::string& data);
    std::string popData();
    void pushReadySocketSingle(int socket);
    void pushReadySocketVec(const std::shared_ptr<std::vector<int>>& socket);
    int popReadySocketSingle();
    std::shared_ptr<std::vector<int>> popReadySocketVec();
    
    // bool isReadySocketEmpty();
private:
    void init();
   dataManager();
   ~dataManager(); 
    // std::vector<std::string> data;
    std::unique_ptr<std::queue<std::string>> data_queue;
    std::mutex data_mutex;
    std::condition_variable data_cond; 
    
    std::unique_ptr<std::vector<int>> ready_socket;
    std::mutex ready_socket_mutex;
    std::condition_variable ready_socket_cond;

    std::unique_ptr<oldSocketManager> socket_manager;
};
