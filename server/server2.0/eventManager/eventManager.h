#include <unordered_map>
#include <string>
#include <functional>
#include <any>
#include "../logSystem/log.h"

class eventManager
{
private:
    std::unordered_map<std::string, std::any> data;
public:
    // eventManager* register_callback(std::string event_name,std::any callback);
    void register_callback(std::string event_name,std::any callback);
    std::any get_callback(std::string event_name);
};